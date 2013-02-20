<?php

// $Id: gmapslivesearch.buynow.inc $

/**
 * @file
 * Buy now functionality.
 */
define('GMAPSLIVESEARCH_VALID_FORM', 'valid_form');

/**
 * Build buy now page.
 */
function gmapslivesearch_buy_now_page() {
  drupal_add_js(array('gmapslivesearch_remove_buy_now_button' => variable_get(GMAPSLIVESEARCH_FIELD_REMOVE_BUY_NOW_BUTTON, FALSE)), 'setting');
  $device_type = gmapslivesearch_return_device_type();   

  
  if($device_type == 'mobile') {
    $enabled_default_result   = variable_get('gmapslivesearch_enable_default_result_mobile');
    $default_result_latitude  = variable_get('gmapslivesearch_default_map_result_latitude_mobile');
    $default_result_longitude = variable_get('gmapslivesearch_default_map_result_longitude_mobile');
    $default_result_zoom      = variable_get('gmapslivesearch_default_map_result_zoom_mobile', '4');
  }
  else {
    $enabled_default_result   = variable_get('gmapslivesearch_enable_default_result');
    $default_result_latitude  = variable_get('gmapslivesearch_default_map_result_latitude');
    $default_result_longitude = variable_get('gmapslivesearch_default_map_result_longitude');
    $default_result_zoom      = variable_get('gmapslivesearch_default_map_result_zoom', '4');
  }
  
  $buy_now_form = drupal_get_form('gmapslivesearch_buy_now_form');
  $results_box = '';
  
  if ($buy_now_form[GMAPSLIVESEARCH_VALID_FORM]['#value']) {
    $_SESSION['gmapslivesearch_has_results'] = TRUE;
    $results_box = gmapslivesearch_build_results_box();
  } elseif (!empty($enabled_default_result) && !empty($default_result_latitude) && !empty($default_result_longitude)) {
    $results_box = gmapslivesearch_build_default_map($enabled_default_result, $default_result_latitude, $default_result_longitude, $default_result_zoom);
  }
  return theme('gmapslivesearch_buynow_page', array(
          'search_box' => drupal_render(drupal_get_form('gmapslivesearch_buy_now_form')), 
          'results_box' => $results_box,
         ));
}

/**
 * Generate form for search stores feature, based on the given $form_type.
 */
function gmapslivesearch_buy_now_form($form, &$form_state) {
  $form = gmapslivesearch_get_all_available_fields($form_state);
  $is_milesearch_within_enabled = variable_get(GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED, FALSE);
  $mobile_type = gmapslivesearch_return_device_type();

  
  if (!empty($form_state['input'])) {
   if (isset($form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]) &&
      (isset($form_state['input'][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]) ||
           !$is_milesearch_within_enabled) &&
      gmapslivesearch_is_valid_address_input($form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH])) { 
        $gmapslivesearch_js_input_data = array(
          GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH => $form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH],
          GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN => $is_milesearch_within_enabled ? $form_state['input'][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN] : GMAPSLIVESEARCH_DEFAULT_MILESEARCH_VALUE,  
          GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED => $is_milesearch_within_enabled,
        );
    }
  }
  
  if (isset($_GET['address'])) {
    $form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]['#default_value'] = $_GET['address'];
    if (!gmapslivesearch_is_valid_address_input($_GET['address'])) {
  
      form_set_error(GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH, variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD, ''));
    }
    else {
      if (!$is_milesearch_within_enabled) {
        $default_mile_range = GMAPSLIVESEARCH_DEFAULT_MILESEARCH_VALUE;
      }
      else {
        $default_mile_range = array_keys($form[GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]['#options']);
        $default_mile_range = $default_mile_range[0];
      }
      
      $gmapslivesearch_js_input_data = array(
        GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH => $_GET['address'],
        GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN => $default_mile_range,
        GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED => $is_milesearch_within_enabled,
      );
    }
  }
  
  if (isset($_GET['content'])) {
    $node = node_load(intval($_GET['content']));
    if ($node && $node->type === variable_get(GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE, '0')) {
      $form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE]['#default_value'] = $_GET['content'];
    }
  }
  
  $is_valid_form = FALSE;
  if (isset($gmapslivesearch_js_input_data)) {
    $is_valid_form = TRUE;
    drupal_add_js(array('gmapslivesearch_input_data' => $gmapslivesearch_js_input_data), 'setting');
  }
  
  unset($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE]);
  unset($form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE]);
  unset($form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN_FIND_ONLINE]);
  $form['#theme'] = 'gmapslivesearch_search_box_form';
  $form['#action'] = url(GMAPSLIVESEARCH_FORM_BUY_NOW_PAGE_PATH);
  if ($mobile_type == 'mobile') {
    $form['#action'] = url(GMAPSLIVESEARCH_FORM_BUY_NOW_PAGE_PATH_MOBILE);
  }
  $form['#id'] = 'gmapslivesearch-buy-now-form';
  
  $form[GMAPSLIVESEARCH_VALID_FORM] = array(
    '#type' => 'value',
    '#value' => $is_valid_form,  
  );
  return $form; 
}

/**
 * Generate form for FIND ONLINE block.
 */
function gmapslivesearch_find_online_form($form, &$form_state) {
  $form = gmapslivesearch_get_all_available_fields($form_state);

  unset($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]);
  unset($form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH]);
  unset($form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN]);
  unset($form[GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]);

  $form['#theme'] = 'gmapslivesearch_find_store_form';

  $form['#attributes']['class'] = array('gmapslivesearch-find-online-form');

  return $form;
}

/**
 * Handle validation of FIND ONLINE block.
 */
function gmapslivesearch_find_online_form_validate($form, &$form_state) {

  if (variable_get(GMAPSLIVESEARCH_SHOW_ERROR_INLINE_FIND_ONLINE_BLOCK, 1)) {
    if (!gmapslivesearch_is_valid_address_input($form_state['input']
            [GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE])) {
      form_error($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE], variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD, ''));

    }
  }
}

/**
 * Handle submission of FIND ONLINE block.
 */
function gmapslivesearch_find_online_form_submit($form, &$form_state) {
  $mobile_type = gmapslivesearch_return_device_type();
  $query = array(
    'address' => $form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE],
  );

  if (isset($form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE])) {
    $query['content'] = $form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE];
  }
  if ($mobile_type == 'mobile') {
    drupal_goto(GMAPSLIVESEARCH_FORM_BUY_NOW_PAGE_PATH_MOBILE, array('query' => $query));
  }
  else {
    drupal_goto(GMAPSLIVESEARCH_FORM_BUY_NOW_PAGE_PATH, array('query' => $query));
  }
}

/**
 * Check whether the input string is valid or not against the zipcode pattern 12345.
 * @param type $zip_code
 * @return boolean True if valid otherwise return false; 
 */
function gmapslivesearch_is_valid_zipcode($zip_code) {
  return preg_match('/^[0-9]{5}$/', $zip_code);
}

/**
 * Check whether the input string is valid or not against the pattern City,State.
 * @param type $city_state Parsed string
 * @return boolean True if valid otherwise return false;
 */
function gmapslivesearch_is_valid_city_state($city_state) {
  return preg_match('/^[A-Za-z]+(\s[A-Za-z]+)?\s*,\s*[A-Za-z]+(\s[A-Za-z]+)?\s*$/', $city_state);
}

/**
 * Validate whether the input address is in valid format.
 */
function gmapslivesearch_is_valid_address_input($address_input) {
  return gmapslivesearch_is_valid_zipcode($address_input) || gmapslivesearch_is_valid_city_state($address_input);
}

/**
 * Handle validation of a search over the stores.
 */
function gmapslivesearch_buy_now_form_validate(&$form, &$form_state) {
  if (!gmapslivesearch_is_valid_address_input($form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH])) {
    $device_type = gmapslivesearch_return_device_type();   
    if($device_type == 'mobile') {
      $gmapslivesearch_error_message_address_field = variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD_MOBILE, '');;
    } else {
      $gmapslivesearch_error_message_address_field = variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD, '');
    }
    form_error($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH], $gmapslivesearch_error_message_address_field);
  }
}

/**
 * Handle submission of a search over the stores, returning the available stores for 
 * the filters seleceted.
 */
function gmapslivesearch_buy_now_form_submit($form, &$form_state) {
  $form_state['redirect'] = FALSE;
}

/**
 * Append error message to form element
 *
 * @param string $content Form element HTML
 * @param array $element Form element properties
 * 
 * @return string Return form element with appended error message, in case this element has error
 */
function gmapslivesearch_post_render_form_element_append_error($content, $element) {

  if ($error = form_get_error($element)) {
    $content .= '<div class="error inline-error"><p>' . ($error) . '</p></div>';
    drupal_get_messages('error', TRUE);
  }
  
  return $content;
}

/**
 * Return an array containing all available fields for buy now search forms.
 * 
 * @param array $form_state
 * 
 * @return array All available buy now search form fields.
 */
function gmapslivesearch_get_all_available_fields($form_state) {
  
  // add the default address field label value to a js
  // the js manage when display the default label or the content input by user on a search result page
  $device_type = gmapslivesearch_return_device_type();   
  
  if($device_type == 'mobile') {
    $gmapslivesearch_buy_online_address_search_label = variable_get('gmapslivesearch_buy_online_address_search_label_mobile');
  } else {
    $gmapslivesearch_buy_online_address_search_label = variable_get('gmapslivesearch_buy_online_address_search_label');
  }
  drupal_add_js(array('gmapslivesearch_field_address_default_label' => $gmapslivesearch_buy_online_address_search_label), 'setting');
  
  //New implementation for max distance. If there's a store that is more than this value of miles away, then we'll have to show the message
  $gmaps_max_distance = variable_get(GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_VALUE, 50);
  $enable_distance_message = variable_get(GMAPSLIVESEARCH_FIELDSET_ENABLE_DISTANCE_MSG, FALSE);
  $gmaps_error_msg    = variable_get(GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_MESSAGE , 'There are results further than #distance# miles. You can try checking the Buy Online options.');
  
  drupal_add_js(array('gmapslivesearch_max_distance' => $gmaps_max_distance ), 'setting');
  drupal_add_js(array('gmapslivesearch_max_distance_message' => t( $gmaps_error_msg )), 'setting');
  drupal_add_js(array('gmapslivesearch_max_distance_enabled' => $enable_distance_message), 'setting');
  drupal_add_js(array('gmapslivesearch_radius_limit' => GMAPSLIVESEARCH_DEFAULT_MILESEARCH_VALUE ), 'setting');
  
  $form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH] = array(
    '#type'   => 'textfield',
    '#default_value' => isset($form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]) ?
      $form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH] : '',
    '#post_render' => array('gmapslivesearch_post_render_form_element_append_error'),
    '#attributes' => array('class' => array('gmapslivesearch-field-address-to-search')),  
  );
  
  //This field is copy from the field above as the two forms can appear in the same page. So it's necessary
  //a different field name for one of the forms.  
  $form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE] = array(
    '#type'   => 'textfield',
    '#default_value' => isset($form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE]) ?
      $form_state['input'][GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE] : '',
    '#post_render' => array('gmapslivesearch_post_render_form_element_append_error'),
    '#attributes' => array('class' => array('gmapslivesearch-field-address-to-search')),  
  );
  
  if (variable_get(GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED, FALSE)) {
    $form[GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN] = array(
      '#type'           => 'select',
      '#title'          => t('Within:'),
      '#options'        => list_extract_allowed_values(variable_get(GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN),
              'list_text', FALSE),
      '#default_value'  => isset($form_state['input'][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]) ?
        $form_state['input'][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN] : '', 
      '#attributes' => array('class' => array('gmapslivesearch-field-milessearch-within')),   
    );
  }
  
  $stores_to_search = array('' => variable_get('gmapslivesearch_buy_online_store_search_label')) +
    list_extract_allowed_values(variable_get(GMAPSLIVESEARCH_FIELD_STORES_SEARCH), 'list_text', FALSE);
  
  $form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH] = array(
    '#type'           => 'select',
    '#options'        => $stores_to_search,
    '#default_value'  => isset($form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_SEARCH]) ?
      $form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_SEARCH] : '', 
    '#attributes' => array('class' => array('gmapslivesearch-field-stores-search')),    
  );
  
  $form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN] = array(
    '#type'           => 'link',
    '#title'          => t(variable_get(GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN_PAGE, 'Go')),
    '#href'           => gmapslivesearch_get_default_store_to_search($form, $stores_to_search,
            GMAPSLIVESEARCH_FIELD_STORES_SEARCH),
    '#attributes'     => array('class' => 'gmapslivesearch-goto-online-store',
                               'title' => t(variable_get(GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN_PAGE, 'Go'))),
  );
  
  $form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE] = array(
    '#type'           => 'select',
    '#options'        => $stores_to_search,
    '#default_value'  => isset($form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE]) ?
      $form_state['input'][GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE] : '', 
    '#attributes' => array('class' => array('gmapslivesearch-field-stores-search')),    
  );
  
  
  $form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN_FIND_ONLINE] = array(
    '#type'           => 'link',
    '#title'          => t(variable_get(GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN, 'Find')),
    '#href'           => gmapslivesearch_get_default_store_to_search($form, $stores_to_search,
            GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE),
    '#attributes'     => array('class' => 'gmapslivesearch-goto-online-store',
                               'title' => t(variable_get(GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN, 'Find'))),
  );

  if (variable_get(GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE, '0') !== '0') {
    $stores = gmapslivesearch_fetch_stores_by_content();
    $has_stores = !empty($stores);
    if ($has_stores) {
      $form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE] = array(
        '#type'	   => 'select',
        '#title'   => t('Content with stores'),
        '#options' => $stores
      );
    }
  }

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t(variable_get(GMAPSLIVESEARCH_FIELD_FIND_STORE_BTN, 'Search')),
    '#attributes'     => array('title' => t(variable_get(GMAPSLIVESEARCH_FIELD_FIND_STORE_BTN, 'Search'))),
  );
  
  //In case this become accessible for logged users this might need some fix.
  if (user_is_logged_in()) {
    //Please check form.inc line 958 for explanation of this. As this is a search form we don't need tokens.
    $form['#token'] = FALSE;
  }
  
  return $form;
}

function gmapslivesearch_get_default_store_to_search($form, $stores_to_search, $field_name) {
  
  //In case the form has been submitted the value selected should be assigned to GO button.
  if (!empty($form[$field_name]['#default_value']) && array_key_exists($form[$field_name]['#default_value'],
          $form[$field_name]['#options'])) {
    $stores_to_search = check_plain($form[$field_name]['#default_value']);
  }
  else {
    //Get the first stores on the list of 'stores to search' and assign to GO button button href.
    $stores_to_search = array_keys($stores_to_search);
    $stores_to_search = $stores_to_search[0];
  }
  
  return $stores_to_search;
}

/**
 * 
 * Obtain the list of content which has relation with the stores
 * @return array
 */
function gmapslivesearch_fetch_stores_by_content() {
  $select= db_select('field_data_gmapslivesearch_stores_options', 'f');
  $select->join('node', 'n', 'n.nid = f.entity_id');
  $select->fields('n', array('nid','title'))->condition('f.deleted', 0, '=');
  $result = $select->execute();
  
  $items = array();
  
  foreach ($result as $row) {
    $items[$row->nid] = $row->title;
  }
  return $items;
}

/**
 * Return valid content object defined by $nid, FALSE if it is not valid.
 *
 * @param $nid
 * Node id to be checked and loaded.
 *
 * @return
 * Loaded object for a valid $nid of a content for the correct content type. FALSE otherwise.
 */
function _gmapslivesearch_get_valid_content($nid) {
  $valid_content = FALSE;
  $node = node_load(intval($nid));
  
  if ($node && $node->type === variable_get(GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE, '0')) {
    $valid_content = $node;
  }
  
  return $valid_content;
}

/**
 * Return the correct stores to be searched by Google Search API for the current content.
 * Check from where the current content is being passed, getting the current content of the posted form or from the $_GET parameter, so it works for both the buy-now page and the buy-now block.
 *
 * @return
 * Return all stores defined in the current content.
 * If the current content is not defined or have its stores empty, return all the defined stores in the administration form.
 */
function _gmapslivesearch_get_stores_to_search() {
  $stores_to_search = array();
  $current_content_id = NULL;
  
  if (!empty($_POST)) {
    $current_content_id = isset($_POST[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE]) ? $_POST[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE] : NULL;
  }
  else {
    $current_content_id = isset($_GET['content']) ? $_GET['content'] : NULL;
  }
  
  $current_content = _gmapslivesearch_get_valid_content($current_content_id);
  if ($current_content && !empty($current_content->gmapslivesearch_stores_options['und'])) {
    $stores = $current_content->gmapslivesearch_stores_options['und'];
    foreach ($stores as $id => $store_name) {
      $stores_to_search[] = $store_name['value'];
    }
  }
  else {
    $stores_to_search = list_extract_allowed_values(variable_get(GMAPSLIVESEARCH_FIELD_SEARCHSTRING, ''), 'list_text', FALSE);
  }
  
  return $stores_to_search;
}
