<?php

/**
 * @file
 * general Utils functions 
 */

function couponsinc_watchdog($msg, $vars = array()) {
  if (couponsinc_is_debug_enabled()) {
    $vars_log = '<pre>';
    
    foreach ($vars as $var) {
      $vars_log .= var_export($var, TRUE);
    }
    
    $vars_log .= '</pre>';
    watchdog('couponsinc', $msg . ':<br />' . $vars_log);
  }
}

/**
 * Get Coupons General Settings
 * 
 * @return array
 */
function couponsinc_get_general_settings() {
  return variable_get('couponsinc_general_settings');
}

/**
 * Return debug option value
 * 
 * @return boolean $is_enabled
 */
function couponsinc_is_debug_enabled() {
  $general_settings = couponsinc_get_general_settings();
  
  $is_enabled = isset($general_settings['debug']) ? $general_settings['debug'] : FALSE;
  
  return $is_enabled;
}

/**
 * Get base Url
 *
 * @return string $url
 */
function couponsinc_get_url() {
  $general_settings = couponsinc_get_general_settings();

  $url = isset($general_settings['base_url']) ? $general_settings['base_url'] : COUPONS_INC_BASE_URL;

  return $url;
}

/**
 * Generates the coupon link
 * 
 * @param string $offer_code
 * @param string $check_code
 * @param string $short_key
 * @param string $long_key
 * @param string $pin_code
 * 
 * @return string
 */
function couponsinc_generate_link($offer_code, $check_code, $short_key, $long_key, $pin_code = '') {
  $pin_code = !empty($pin_code) ? $pin_code : uniqid(); 
  $query_string = couponsinc_generate_link_query_string($pin_code, $offer_code, $check_code, $short_key, $long_key);
  
  $url = couponsinc_get_url() . $query_string;
  
  return $url;
}

/**
 * Generate coupon link query string
 * 
 * @param string $pin_code
 * @param string $offer_code
 * @param string $check_code
 * @param string $short_key
 * @param string $long_key
 * 
 * @return string
 */
function couponsinc_generate_link_query_string($pin_code, $offer_code, $check_code, $short_key, $long_key) {
  $query_string = '';
  
  $parameters = couponsinc_get_link_parameters($pin_code, $offer_code, $check_code, $short_key, $long_key);
  
  if (is_array($parameters) && (count($parameters) > 0)) {
    foreach ($parameters as $parameter => $value) {
      $query_string .= '&' . $parameter . '=' . $value;
    }
    
    $query_string = '?' . substr($query_string, 1);
  }
  
  return $query_string;
}

/**
 * Get Link parameters
 * 
 * @param string $pin_code
 * @param string $offer_code
 * @param string $check_code
 * @param string $short_key
 * @param string $long_key
 * 
 * @return array $parameters
 *   Keys -> Values
 *     eb  -> default 1
 *     o   -> offer code, 4 to 5 digits
 *     c   -> check code, 2 digits
 *     p   -> pin code, string
 *     cpt -> encrypted string
 *   
 */
function couponsinc_get_link_parameters($pin_code, $offer_code, $check_code, $short_key, $long_key) {
  module_load_include('inc', 'couponsinc', 'inc/couponsinc.encryption');
  
  $encrypted_string = couponsinc_generate_cpt($pin_code, $offer_code, $short_key, $long_key);
  
  // Mount parameter array
  $parameters = array(
    'eb'  => 1,
    'o'   => $offer_code,
    'c'   => $check_code,
    'p'   => $pin_code,
    'cpt' => $encrypted_string,
  );
  
  return $parameters;
}

/**
 * Get cupon settings by node
 * 
 * @param int $nid
 * 
 * @return array $settings
 */
function couponsinc_get_coupon_settings_by_node($nid) {
  $entity = entity_load('node', array($nid), array(), TRUE);
  
  $offer_code = field_get_items('node', $entity[$nid], 'field_couponsinc_offer_code');
  $check_code = field_get_items('node', $entity[$nid], 'field_couponsinc_check_code');
  $short_key  = field_get_items('node', $entity[$nid], 'field_couponsinc_short_key');
  $long_key   = field_get_items('node', $entity[$nid], 'field_couponsinc_long_key');
  
  $settings = array(
    'offer_code' => isset($offer_code[0]['value']) ? $offer_code[0]['value'] : NULL,
    'check_code' => isset($check_code[0]['value']) ? $check_code[0]['value'] : NULL,
    'short_key'  => isset($short_key[0]['value']) ? $short_key[0]['value'] : NULL,
    'long_key'   => isset($long_key[0]['value']) ? $long_key[0]['value'] : NULL,
  );
  
  return in_array(NULL, $settings) ? FALSE : $settings;
}

/**
 * Create custom filter rule to generate link by related settings of coupon node
 * 
 * @param object $node
 */
function couponsinc_create_custom_filter_rule_by_node($node) {
  if (couponsinc_custom_filter_type_is_already_created()) {
    $rule_id     = (integer) db_query("SELECT MAX(rid) FROM {customfilter_rule}")->fetchField() + 1;
    $pattern     = couponsinc_generate_custom_filter_pattern($node->nid);
    $replacement = couponsinc_generate_custom_filter_replacement_by_node($node->nid);
    
    $item = array(
      'rid'         => $rule_id,
      'prid'        => 0,
      'fid'         => couponsinc_get_filter_type_id(),
      'name'        => 'Link to coupon: ' . $node->title,
      'description' => '',
      'enabled'     => 1,
      'matches'     => 1,
      'pattern'     => $pattern,
      'replacement' => $replacement,
      'code'        => 1,
      'weight'      => 0,
    );
    
    drupal_write_record('customfilter_rule', $item);
  }
  else {
    couponsinc_create_custom_filter_type();
  }
}

/**
 * Generate Coupon Custom filter replacement value by node id 
 * 
 * @param int $nid
 */
function couponsinc_generate_custom_filter_replacement_by_node($nid) {
  return 'module_load_include(\'inc\', \'couponsinc\', \'inc/couponsinc.utils\'); $result = couponsinc_generate_link_by_coupon_node(' . $nid . ');';
}

/**
 * Verify if coupon filter type is already created
 * 
 * @return boolean
 */
function couponsinc_custom_filter_type_is_already_created() {
  $already_created = FALSE;
  
  if (couponsinc_get_filter_type_id()) {
    $already_created = TRUE;
  }

  return $already_created;
}

/**
 * Delete custom filter rule by node id
 * 
 * @param unknown_type $nid
 */
function couponsinc_delete_custom_filter_rule_by_pattern($pattern) {
  db_delete('customfilter_rule')
  ->condition('pattern', $pattern)
  ->execute();
}

/**
 * Generate custom filter pattern
 * 
 * @param string $identifier
 * 
 * @return string
 */
function couponsinc_generate_custom_filter_pattern($identifier) {
  return '/@couponsinc_link_nid-' . $identifier . '@/';
}

/**
 * Get Coupon Custom Filter Id
 *
 * @return int $fid
 */
function couponsinc_get_filter_type_id() {
  $cache = &drupal_static(__FUNCTION__);

  if ($cache) {
    $fid = $cache;
  }
  else {
    $fid = db_query('SELECT fid FROM {customfilter_filter} WHERE type = :type', array(':type' => COUPONS_INC_CUSTOM_FILTER_TYPE))->fetchField();

    if ($fid) {
      $cache = $fid;
    }
  }

  return $fid;
}

/**
 * Generate coupon link by node
 *
 * @param int $nid
 * @param string $pin_code
 *
 * @return $link
 */
function couponsinc_generate_link_by_coupon_node($nid, $pin_code = '') {
  $link = '';
  $coupon_settings = couponsinc_get_coupon_settings_by_node($nid);

  if ($coupon_settings) {
    $link = couponsinc_generate_link($coupon_settings['offer_code'], $coupon_settings['check_code'], $coupon_settings['short_key'], $coupon_settings['long_key'], $pin_code);
  }

  return $link;
}

/**
 * Disable text formats cache
 */
function couponsinc_disable_text_formats_cache() {
  $update = db_update('filter_format')
  ->fields(array(
    'cache' => 0,
  ))
  ->execute();
}