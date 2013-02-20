<?php
/**
* @file
* Coupon functions file
*/

/**
 * Generate the timestamp(code) when the form is submitted
 */
function coupons_submit_generate_timestamp($form, &$form_state) {
  $timestamp = time(); // get the timestamp
  $form_state['values']['submitted']['field_coupon_timestamp'] = $timestamp;
}

/**
 * Send the user to coupon page if the coupon isn't valid or it's expired
 */
function coupons_redirect_to_coupon_form_on_invalid_coupon_code($coupon_code, $node) {
  $is_valid_coupon_code = coupons_is_valid_coupon_code($coupon_code, $node);
  if (!$is_valid_coupon_code) {
    $coupons_email_updates_url = variable_get('coupons_email_updates_url', 'email-updates');
    drupal_goto($coupons_email_updates_url. '/' .$node->nid);
    exit();
  }  
}


/**
 * Get the coupon code via GET from url, if it exists
 */
function coupons_get_coupon_code_from_url() {
  $coupon_code = 0;
  if (isset($_GET['coupon_code'])) {      
    $coupon_code = coupons_decode_coupon_code($_GET['coupon_code']);
  }
  return $coupon_code;
}

/**
 * decode the coupon code
 */
function coupons_decode_coupon_code($coupon_code) {
  return intval(base64_decode($coupon_code));
}

/**
 * encode coupon code using base64
 */
function coupons_encode_coupon_code($coupon_code) {
  return base64_encode($coupon_code);
}


/**
 * validate a coupon code
 */
function coupons_is_valid_coupon_code($coupon_code, $node) {
  $is_valid_coupon_code = FALSE;
  $minutes_before_coupon_expire = variable_get('coupon_minutes_before_code_expire', 10);
    
  if($node->type == CONTENT_TYPE_COUPON) {    
    if(!empty($coupon_code) && is_numeric($coupon_code) && ($coupon_code+($minutes_before_coupon_expire*60)) > time()) {
      $is_valid_coupon_code = TRUE;
    }    
  }
  
  return $is_valid_coupon_code;
}


/**
 * verify if the user is an admin
 */
function coupons_is_user_admin($user) {
  $admin_roles = array('administrator', 'content admin');

  $is_admin = (count(array_intersect($user->roles, $admin_roles)) > 0);
  
  return $is_admin;  
}
