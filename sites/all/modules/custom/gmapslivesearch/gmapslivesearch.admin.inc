<?php 

// $Id: gmapslivesearch.admin.inc $

/**
 * Validator for file field.
 */
function _gmapslivesearch_map_bullet_validate($element, &$form_state) {
  $form_key = implode('_', $element['#parents']);

  if (empty($_FILES['files']['name'][$form_key]) || form_get_error($element)) {
    return;
  }
  //Available extensions
  $extensions = array('jpg', 'png');
  
  $dot = strrpos($_FILES['files']['name'][$form_key], '.');
  $extension = drupal_strtolower(drupal_substr($_FILES['files']['name'][$form_key], $dot+1));
  $file_error = FALSE;
  if (!empty($extensions) && !in_array($extension, $extensions)) {
    form_error($element, 
            t("Files with the '%ext' extension are not allowed, please upload a file with a %exts extension.",
                    array('%ext' => $extension, '%exts' => implode(',', $extensions))));
    $file_error = TRUE;
  }
  
  // Save the file to a temporary location.
  if (!$file_error) {
    $upload_dir = file_build_uri(GMAPSLIVESEARCH_MAP_BULLETS_IMAGE_FOLDER);
    if (file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY)) {
      // Note that the $extensions list was already validated above but needs to
      // be passed into file_save_upload() for it to pass internal validation
      // and not use the default extension checklist.
      $file = file_save_upload($form_key, array('file_validate_extensions' => array(implode(' ', $extensions))),
              $upload_dir, FILE_EXISTS_REPLACE);
      if ($file->filename != $element['#file_name'] .'.'. $extension) {
        $file = file_move($file, $upload_dir .'/'. $element['#file_name'] .'.'. $extension, FILE_EXISTS_REPLACE);
      }
      if ($file) {
        // Set the hidden field value.
        $parents = $element['#array_parents'];
        array_pop($parents);
        $parents[] = $form_key . '_fid';
        form_set_value(array('#parents' => $parents), $file->fid, $form_state);
      }
      else {
        drupal_set_message(
                t('The uploaded file %filename was unable to be saved. The destination directory may not be writable.',
                        array('%filename' => $file->filename)), 'error');
      }
    }
    else {
      drupal_set_message(t('The uploaded file was unable to be saved. The destination directory does not exist.'),
              'error');
    }
  }
}

/**
 * Flush cache to rebuild gmapslivesearch menu based on the user input.
 */
function gmapslivesearch_admin_config_form($form, &$form_state) {
  for ($i = 1; $i <= GMAPSLIVESEARCH_GOOGLE_SEARCH_TOTAL_RESULTS_RETURNED; $i++) {
    $image_path = NULL;
    if (isset($form_state['values'][GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS]['gmapslivesearch_map_bullet_' . $i . '_fid'])) {
      $file = file_load($form_state['values']
              [GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS]['gmapslivesearch_map_bullet_' . $i . '_fid']);
      $file->status = FILE_STATUS_PERMANENT;
      // Save any new files permanently.
      file_save($file);
      $image_path = $file->uri;
    }
    else if (isset($form_state['values']['bullet_default_value' . $i])){
      $image_path = $form_state['values']['bullet_default_value' . $i];
    }
    $form_state['values'][GMAPSLIVESEARCH_MAP_BULLETS_IMAGE_PREFIX . $i] = $image_path;
  }
  
  drupal_flush_all_caches();
}

function gmapslivesearch_admin_config_form_stores_by_content_validate($form, &$form_state) {
  $field = field_info_field('gmapslivesearch_stores_options');
  $has_data = field_has_data($field);
  $element = $form[GMAPSLIVESEARCH_FIELD_SEARCHSTRING];

  $values = list_extract_allowed_values($form_state['values'][GMAPSLIVESEARCH_FIELD_SEARCHSTRING], 'list_text', FALSE);

  if (!is_array($values)) {
    form_error($element, t('Allowed values list: invalid input.'));
  }
  else {
    // Prevent removing values currently in use.
    if ($has_data) {
      $lost_keys = array_diff(array_keys($field['settings']['allowed_values']), array_keys($values));
      
      if (_list_values_in_use($field, $lost_keys)) {
        form_error($element, t('Allowed values list: some values are being removed while currently in use: :lost_keys.', array(':lost_keys' => implode(', ', $lost_keys))));
      }
    }

  }
}

/**
* Process Stores options for gmapslivesearch settings submission.
*/
function gmapslivesearch_admin_config_form_stores_by_content_submit($form, &$form_state) {
  $types = node_type_get_types();
  foreach($types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }

  // Loop through each defined content type
  foreach ($types as $key => $value) {
    $instance = field_info_instance('node', 'gmapslivesearch_stores_options', $key);
    
    if ($key !== $form_state['values'][GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE]) {
    
      if (!empty($instance)) {
        $delete_field_if_last_instance = FALSE;
        field_delete_instance($instance, $delete_field_if_last_instance);
        watchdog("Gmapslivesearch", 'Deleted gmapslivesearch_stores_options field from content type: %key', array('%key' => $key));
      }
    }
    else {
    
      // If the check box for a content type is checked, look to see whether
      // the field is associated with that content type. If not then add the
      // stores selection field to the content type.
      if (empty($instance)) {
      
        $instance = array(
          'field_name' => 'gmapslivesearch_stores_options',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => 'Selected Stores',
          'widget' => array(
            'type' => 'options_buttons',
            'weight' => 5,
          ),
        );
        
        $instance = field_create_instance($instance);
        watchdog('Gmapslivesearch', 'Added gmapslivesearch_stores_options field to content type: %key', array('%key' => $key));
      }
      
      // update field allowed values
      $field = field_info_field('gmapslivesearch_stores_options');
      $field['settings']['allowed_values'] = list_extract_allowed_values($form_state['values'][GMAPSLIVESEARCH_FIELD_SEARCHSTRING], 'list_text', FALSE);
      field_update_field($field);
    }
  
  } // End foreach loop.
  
}


/**
 * Setting the API configurations
 */
 function gmapslivesearch_general_config() {
  $form = array();

  $form[GMAPSLIVESEARCH_ERROR_MESSAGES] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Validation Messages'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
  );
  
  $form[GMAPSLIVESEARCH_ERROR_MESSAGES][GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD] = array(
      '#type'           => 'textfield',
      '#title'          => t('Address Error Message'),
      '#description'    => t('Error message for the address field.'),
      '#default_value'  => variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD, 'Please enter a valid City, State or Zip Code'), 
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_GOOGLEAPIKEY] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Google Search API'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
  );

  $form[GMAPSLIVESEARCH_FIELDSET_GOOGLEAPIKEY][GMAPSLIVESEARCH_FIELD_GOOGLEAPIKEY] = array(
    '#type'           => 'textfield',
    '#title'          => t('Google Search API key'),
    '#description'    => t('Google Search API key. To generate it go to http://code.google.com/intl/en/apis/loader/signup.html'),  
    '#size'           => 110,  
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_GOOGLEAPIKEY,
            'ABQIAAAANfhoe0INRhmAE6PvohZqUxT2yXp_ZAY8_ufC3CFXhHIE1NvwkxTopbkOmnwf3iP4JAZtWKM2LGWOwg'),   
  );

  $form[GMAPSLIVESEARCH_FIELDSET_MILES_SEARCH] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Miles search'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );

  $form[GMAPSLIVESEARCH_FIELDSET_MILES_SEARCH][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Use miles filter'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN_ENABLED, FALSE),
  );

  $description = _gmapslivesearch_get_default_description();
  $form[GMAPSLIVESEARCH_FIELDSET_MILES_SEARCH][GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN] = array(
    '#type'           => 'textarea',
    '#title'          => t('Miles to search results'),
    '#description'    => t($description),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN, ''),
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Great Distance Value'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE][GMAPSLIVESEARCH_FIELDSET_ENABLE_DISTANCE_MSG] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable Great distance message'),
    '#description'    => t('If this is enabled, a message will appear to the visitor if no stores are found withing the distance specified.'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELDSET_ENABLE_DISTANCE_MSG, FALSE),
  );
  
  
  $form[GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE][GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_VALUE] = array(
      '#type'           =>  'textfield',
      '#title'          =>  t('Great Distance Value'),
      '#description'    =>  t('Define the mileage from which the "great distance" message will be shown. This value is in miles.'),  
      '#size'           =>  50,
      '#default_value'  =>  variable_get(GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_VALUE,50),
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE][GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_MESSAGE] = array(
      '#type'           =>  'textfield',
      '#title'          =>  t('Great Distance Warning Message'),
      '#description'    =>  t('This message will be shown when all stores located are further away than the "Great distance" setting above. You can use the #distance# placeholder to be replaced with the max allowed mileage.'),  
      '#size'           =>  110,
      '#default_value'  =>  t(variable_get(GMAPSLIVESEARCH_FIELDSET_MAPS_GREAT_DISTANCE_MESSAGE , 'There are no results within #distance# miles. Please try our Buy Online options')),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Default Result'),
    '#description'    => t('Enable a Default Result for the Map'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT][GMAPSLIVESEARCH_FIELDSET_DEFAULT_MILE_RADIUS] = array(
    '#type'           => 'textfield',
    '#title'          => t('Default Mile Radius'),
    '#description'    => t('This will be the radius of the circle drawn on the map. The default value for this is 1.'),
    '#size'           =>  50,
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELDSET_DEFAULT_MILE_RADIUS, 1),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT][GMAPSLIVESEARCH_FIELD_ENABLE_DEFAULT_RESULT] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable default result'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_ENABLE_DEFAULT_RESULT, FALSE),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LATITUDE] = array(
    '#type'           => 'textfield',
    '#title'          => t('Latitude'),
    '#size'           => 10,
    '#description'    => t('Set the default Latitude address to the Map. Ex.: -34.397'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LATITUDE),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LONGITUDE] = array(
    '#type'           => 'textfield',
    '#title'          => t('Longitude'),
    '#size'           => 10,
    '#description'    => t('Set the default Longitude address to the Map. Ex.: 150.644'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LONGITUDE),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_ZOOM] = array(
    '#type'           => 'textfield',
    '#title'          => t('Map Zoom'),
    '#size'           => 5,
    '#description'    => t('Set the Zoom distance for the Map. The default value is "4".'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_ZOOM, '4'),
  );

  $form[GMAPSLIVESEARCH_SHOW_ERROR_INLINE_FIND_ONLINE_BLOCK] = array(
    '#type'            => 'checkbox',
    '#title'           => t('Show error inline find online'),
    '#description'     => t('Whether or not to show validation form error on find online block at the current page otherwise redirects to where yo buy page'),
    '#default_value'   => variable_get(GMAPSLIVESEARCH_SHOW_ERROR_INLINE_FIND_ONLINE_BLOCK, 1),
  );
  
  $form[GMAPSLIVESEARCH_JQTRANSFORM_OPTION] = array(
    '#type'            => 'checkbox',
    '#title'           => t('Apply jqTransform'),
    '#description'     => t('Enable this field to apply jqTranform plugin to form fields.'),
    '#default_value'   => variable_get(GMAPSLIVESEARCH_JQTRANSFORM_OPTION, 0),
  );

  return system_settings_form($form);
 }

 /**
 * Setting the API configurations for mobile
 */
 function gmapslivesearch_mobile_config() {
  $form = array();

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Result for Mobile'),
    '#description' => t('Enable a Default Result for the Map on Mobile devices'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE][GMAPSLIVESEARCH_FIELDSET_DEFAULT_MILE_RADIUS_MOBILE] = array(
    '#type' => 'textfield',
    '#title' => t('Default Mile Radius on Mobile devices'),
    '#description' => t('This will be the radius of the circle drawn on the map. The default value for this is 1.'),
    '#size' => 50,
    '#default_value' => variable_get(GMAPSLIVESEARCH_FIELDSET_DEFAULT_MILE_RADIUS_MOBILE, 1),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE][GMAPSLIVESEARCH_FIELD_ENABLE_DEFAULT_RESULT_MOBILE] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable default result on Mobile devices'),
    '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_ENABLE_DEFAULT_RESULT_MOBILE, FALSE),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LATITUDE_MOBILE] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude on Mobile devices'),
    '#size' => 10,
    '#description' => t('Set the default Latitude address to the Map. Ex.: -34.397'),
    '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LATITUDE_MOBILE),
  );
    
  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LONGITUDE_MOBILE] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude on Mobile devices'),
    '#size' => 10,
    '#description' => t('Set the default Longitude address to the Map. Ex.: 150.644'),
    '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_LONGITUDE_MOBILE),
  );

  $form[GMAPSLIVESEARCH_FIELDSET_DEFAULT_RESULT_MOBILE][GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_ZOOM_MOBILE] = array(
    '#type' => 'textfield',
    '#title' => t('Map Zoom on Mobile devices'),
    '#size' => 5,
    '#description' => t('Set the Zoom distance for the Map. The default value is "4".'),
    '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_DEFAULT_MAP_RESULT_ZOOM_MOBILE, '4'),
  );
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE_MOBILE] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Where to buy PAGE on mobile device'),
      '#collapsed'      => TRUE,
      '#collapsible'    => TRUE,  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE_MOBILE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE_MOBILE] = array(
      '#type'           => 'textfield',
      '#title'          => t('Gmaps live search page title on mobile page'),
      '#description'    => t('Set the title for Gmaps live search page on mobile device'),
      '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE_MOBILE, variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE,'Buy Now')),
  );
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE_MOBILE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH_MOBILE] = array(
      '#type'           => 'textfield',
      '#title'          => t('Path to Gmaps live search page on mobile'),
      '#description'    => t('Set the url for Gmaps live search page on mobile'),
      '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH_MOBILE, variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH, '')),
  );
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE_MOBILE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_SHOW_SHARE_MOBILE] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Show share button'),
      '#description'    => t('Enables the share button (send to a friend) by email'),
      '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_SHOW_SHARE_MOBILE, FALSE),
  );
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE_MOBILE][GMAPSLIVESEARCH_FIELD_BUY_NOW_ADDRESS_SEARCH_LABEL_MOBILE] = array(
    '#type'           => 'textfield',
    '#title'          => t('TextBox address label on mobile'),
    '#description'    => t('Label for field address search on buy online on mobile'),  
    '#default_value'  => variable_get('gmapslivesearch_buy_online_address_search_label_mobile', 'Enter Zip Code or City, State'),
  );
  $form[GMAPSLIVESEARCH_ERROR_MESSAGES_MOBILE] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Validation Messages'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  
  $form[GMAPSLIVESEARCH_ERROR_MESSAGES_MOBILE][GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD_MOBILE] = array(
      '#type'           => 'textfield',
      '#title'          => t('Address Error Message on mobile'),
      '#description'    => t('Error message for the address field on mobile.'),
      '#default_value'  => variable_get(GMAPSLIVESEARCH_ERROR_MESSAGE_ADDRESS_FIELD_MOBILE, 'Please enter a valid City, State or Zip Code'), 
  );
  return system_settings_form($form);
}

 /**
  * Setting the available stores
  */
function gmapslivesearch_stores_config() {
  $form = array();
  
  $description = _gmapslivesearch_get_default_description();
  $form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH] = array(
    '#type'           => 'textarea',
    '#title'          => t('Stores to search'),
    '#description'    => t($description),  
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_STORES_SEARCH, ''),
  );
  
  $form[GMAPSLIVESEARCH_FIELD_SEARCHSTRING] = array(
    '#type'           => 'textarea',
    '#title'          => t('Search string'),  
    '#description'    => t('Search string used to get store results. Enter one value per line.'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_SEARCHSTRING, ''),
  );
  
  $form['gmapslivesearch_maps_stores_by_content'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Stores by content'),
    '#description'    => t('Content type specific stores configuration.'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
  );
  
  $types = node_type_get_types();
  foreach($types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }
  array_unshift($options, 'none');
  $form['gmapslivesearch_maps_stores_by_content'][GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE] = array(
    '#type'           => 'select',
    '#title'          => t('Stores by content'),
    '#description'    => t('Select a content type to make specific stores configuration by content. It will load a new field for that type of content and display a new configuration based on the stores entered in "Search string" field.'),
    '#options'        => $options,
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE, 0),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
  );
  
  $form['#validate'][] = 'gmapslivesearch_admin_config_form_stores_by_content_validate';
  $form['#submit'][] = 'gmapslivesearch_admin_config_form_stores_by_content_submit';
  
  return system_settings_form($form);
}

/**
 * Setting the content and layout details
 */
function gmapslivesearch_content_config() {
  $form = array();
   
  $form[GMAPSLIVESEARCH_FIELD_REMOVE_BUY_NOW_BUTTON] = array(
    '#type'           => 'checkbox',
    '#prefix'         => t('<b>Remove Button</b>'),
    '#title'          => t('Remove the Button in Buy Now select field'),
    '#description'    => t('If this option is selected, removes the "GO" Button in Buy now field, leaving the automatic redirection to link when selecting a store.'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_REMOVE_BUY_NOW_BUTTON, FALSE),  
  );
  
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_SELECT] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Default fields label'),
    '#collapsed'      => TRUE,
    '#collapsible'    => TRUE,  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_SELECT][GMAPSLIVESEARCH_FIELD_BUY_NOW_ADDRESS_SEARCH_LABEL] = array(
    '#type'           => 'textfield',
    '#title'          => t('TextBox address label'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_address_search_label', 'Enter Zip Code or City, State'),
    '#description'    => t('Label for field address search on buy online'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_SELECT][GMAPSLIVESEARCH_FIELD_BUY_NOW_STORE_SEARCH_LABEL] = array(
    '#type'           => 'textfield',
    '#title'          => t('Select stores label'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_store_search_label', 'BUY ONLINE'),
    '#description'    => t('Label for field store on buy online'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_SELECT][GMAPSLIVESEARCH_FIELD_FIND_STORE_BTN] = array(
    '#type'           => 'textfield',
    '#title'          => t('Find a Store: \'Search\' button text and title'),
    '#default_value'  => variable_get('gmapslivesearch_field_find_store_btn', 'Search'),
    '#description'    => t('\'Search\' button text and title for the Find a Store box'),  
  );  
    
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Where to buy BLOCK'),
    '#collapsed'      => TRUE,
    '#collapsible'    => TRUE,  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK][GMAPSLIVESEARCH_FIELD_BUY_NOW_ADDRESS_SEARCH_DESC] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Buy online address description'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_address_search_desc'),
    '#description'    => t('Description for field address on buy online box'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK][GMAPSLIVESEARCH_FIELD_BUY_NOW_STORE_SEARCH_DESC] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Buy online store description'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_store_search_desc'),
    '#description'    => t('Description for field store on buy online box'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK][GMAPSLIVESEARCH_FIELD_BUY_NOW_STORE_ADDRESS_FIELD_LABEL] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Buy online address field label'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_store_address_field_label'),
    '#description'    => t('Label for address field on buy online box.'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK][GMAPSLIVESEARCH_FIELD_BUY_NOW_STORE_SEARCH_FIELD_LABEL] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Buy online store field label'),
    '#default_value'  => variable_get('gmapslivesearch_buy_online_store_search_field_label'),
    '#description'    => t('Label for store field on buy online box.'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_BLOCK][GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN] = array(
    '#type'           => 'textfield',
    '#title'          => t('Buy Online: \'Go\' button text and title'),
    '#default_value'  => variable_get('gmapslivesearch_field_buy_online_btn', 'Find'),
    '#description'    => t('\'Go\' button text and title for the Buy Online box'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Where to buy PAGE'),
    '#collapsed'      => TRUE,
    '#collapsible'    => TRUE,  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE] = array(
    '#type'           => 'textfield',
    '#title'          => t('Gmaps live search page title'),
    '#description'    => t('Set the title for Gmaps live search page'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE, 'Buy Now'),
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH] = array(
    '#type'           => 'textfield',
    '#title'          => t('Path to Gmaps live search page'),
    '#description'    => t('Set the url for Gmaps live search page'),
    '#default_value'  => variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH, 'buy-now'),
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_ADDRESS_SEARCH_DESC] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy page address description'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_address_search_desc'),
    '#description'    => t('Description for field address on where to buy page'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_STORE_SEARCH_DESC] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy page store description'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_store_search_desc'),
    '#description'    => t('Description for field store on where to buy page'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_TITLE_TEXT] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy page title'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_title'),
    '#description'    => t('Page title for where to buy section'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_SUBTITLE_TEXT] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy page subtitle'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_subtitle'),
    '#description'    => t('Page subtitle for where to buy section'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_ADDRESS_FIELD_LABEL] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy address field label'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_address_field_label'),
    '#description'    => t('Label for address field on Where to buy page.'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_STORE_FIELD_LABEL] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy store field label'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_store_field_label'),
    '#description'    => t('Label for store field on Where to buy page.'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_RESULTS_SCROLL_JS_DRAG_HEIGHT] = array(
    '#type'           =>  'textfield',
    '#title'          => t('Where to buy results scroll jsDrag height'),
    '#default_value'  => variable_get('gmapslivesearch_where_to_buy_page_results_scroll_js_drag_height', 73),
    '#description'    => t('Store results list jsDrag scroll height in pixels. Do NOT include "px" in its value, it expects only an integer.'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_BUY_ONLINE_PAGE][GMAPSLIVESEARCH_FIELD_BUY_ONLINE_BTN_PAGE] = array(
    '#type'           => 'textfield',
    '#title'          => t('Buy Online: \'Go\' button text and title'),
    '#default_value'  => variable_get('gmapslivesearch_field_buy_online_btn_page', 'Go'),
    '#description'    => t('\'Go\' button text and title for the Buy Online box'),  
  );
  
  $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Maps bullets'),
    '#description'    => t('Bullets for returned results on google maps'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
    '#element_validate' => array(),
  );
  
  //Google Search always return 8 results so is necessary 8 fields for the bullets
  for ($i = 1; $i <= GMAPSLIVESEARCH_GOOGLE_SEARCH_TOTAL_RESULTS_RETURNED; $i++) {
    $bullet_path = variable_get(GMAPSLIVESEARCH_MAP_BULLETS_IMAGE_PREFIX . $i);
    
    $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS][GMAPSLIVESEARCH_MAP_BULLETS_IMAGE_PREFIX . $i] = array(
      '#type'           => 'file',
      '#title'          => t('Image bullet'),
      '#description'    => t('Image should be on the format '. $i . '.jpg'),
      '#element_validate' => array('_gmapslivesearch_map_bullet_validate'),
      '#file_name' => $i,
    );
    
    if ($bullet_path != NULL) {
      $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS]['bullet_preview' . $i] = array(
        '#type'   => 'item',
        '#markup' => t('Uploaded Image') . ': ' . theme('image',
                                                        array('path' => $bullet_path,
                                                              'alt'   => 'Bullet ' . $i,
                                                              'title' => 'Bullet ' . $i,
                                                        )
                                                  )
      );

      $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS]['bullet_default_value' . $i] = array(
        '#type'  => 'hidden',
        '#value' =>  $bullet_path,
      );
    }
  }
  if (module_exists('mobile_tools')) {
    $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS][GMAPSLIVESEARCH_FIELD_RETINA_IMAGE_WIDTH] = array(
          '#type' => 'textfield',
          '#title' => t('Image width to use on markers'),
          '#size' => 5,
          '#description' => t('Set the width of marker because the retina image'),
          '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_RETINA_IMAGE_WIDTH, '35'),
    );
    $form[GMAPSLIVESEARCH_FIELDSET_MAPS_BULLETS][GMAPSLIVESEARCH_FIELD_RETINA_IMAGE_HEIGHT] = array(
            '#type' => 'textfield',
            '#title' => t('Image height to use on markers'),
            '#size' => 5,
            '#description' => t('Set the height of marker because the retina image'),
            '#default_value' => variable_get(GMAPSLIVESEARCH_FIELD_RETINA_IMAGE_HEIGHT, '40'),
    );
  }
  
  $form['#attributes'] = array('enctype' => "multipart/form-data");
    
  $form['#submit'][] = 'gmapslivesearch_admin_config_form';  
  
  return system_settings_form($form);
}


/**
 * Get a default descript used in many fields
 */
function _gmapslivesearch_get_default_description() {
  $description = '<p>' . 
      t('The possible values this field can contain. Enter one value per line, in the format key|label.');
  $description .= '<br/>' . 
      t('The key is the stored value, and must be numeric. The label will be used in displayed values and edit forms.');
  $description .= '<br/>' . 
      t('The label is optional: if a line contains a single number, it will be used as key and label.');
  $description .= '<br/>' . t('Lists of labels are also accepted (one label per line), only if the field does not '.
          'hold any values yet. Numeric keys will be automatically generated from the positions in the list.');
  return $description;
}