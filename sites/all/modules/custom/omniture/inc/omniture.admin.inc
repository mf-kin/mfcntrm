<?php
/**
 * @file
 * Omniture admin settings
 */

/**
 * Form builder. Configure omniture settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function omniture_admin_settings($form, &$form_state) {
  $form = array();
  
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Omniture tracking account configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -10,
  );
  $form['account']['omniture_s_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking account. Report suite.'),
    '#description' => t('Enter your Omniture tracking account here (s_account variable). Notice that nothing will be tracked if this is not set.'),
    '#default_value' => variable_get('omniture_s_account', ''),
  );
  $form['account']['omniture_siteID'] = array(
    '#type' => 'textfield',
    '#title' => t('siteID'),
    '#description' => t('Leftmost value in pagename (s.siteID variable).'),
    '#default_value' => variable_get('omniture_siteID', ''),
  );
  $form['account']['omniture_visitor_namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Visitor namespace'),
    '#description' => t('Visitor namespace. Generally it is the name which identifies your company. Maps to s.visitorNamespace variable.'),
    '#default_value' => variable_get('omniture_visitor_namespace', ''),
  );
  $form['account']['omniture_dc'] = array(
    '#type' => 'textfield',
    '#title' => t('Data center'),
    '#description' => t('Identifies the Omniture data center. Do not change this value unless so instructed by your Omniture representative. Maps to s.dc variable.'),
    '#default_value' => variable_get('omniture_dc', '122'),
  );
  $form['account']['omniture_tracking_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking server'),
    '#description' => t('Tracking server if any. Leave blank if you do not have a specific tracking server. Maps to s.trackingServer variable.'),
    '#default_value' => variable_get('omniture_tracking_server', ''),
  );
  $form['account']['omniture_tracking_server_secure'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking server secure'),
    '#description' => t('Tracking server secure if any. Leave blank if you do not have a specific tracking server secure. Maps to s.trackingServerSecure variable.'),
    '#default_value' => variable_get('omniture_tracking_server_secure', ''),
  );
  $form['account']['omniture_404_page_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Page not found name'),
    '#description' => t('Enter the page name to be set if a 404 error occurs. Defaults to "404".'),
    '#default_value' => variable_get('omniture_404_page_name', '404'),
  );
  
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('User role tracking'),
    '#collapsible' => TRUE,
    '#description' => t('Define which user roles should be tracked by SiteCatalyst.'),
    '#weight' => -5,
  );
  $result = db_select('role','r')
    ->fields('r')
    ->orderBy('name', 'ASC')
  ->execute();
  foreach ($result as $role) {
    // Can't use empty spaces in varname.
    $role_varname = str_replace(' ', '_', $role->name);
    // Only the basic roles are translated.
    $role_name = in_array($role->rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID)) ? t($role->name) : $role->name;
    $form['roles']["omniture_{$role_varname}"] = array(
      '#type' => 'checkbox',
      '#title' => $role_name,
      '#default_value' => variable_get("omniture_{$role_varname}", FALSE),
    );
  }
  
  return system_settings_form($form);
}

 /**
 * Form builder. Configure omniture js settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function omniture_js_admin_settings($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');
  _omniture_include_code_snippet_syntax_highlight_feature();
  
  $form = array();
  $common_attributes = array(
    'style' => 'font-family: monospace;',
    'class' => array('use-snippet'),
  );
  
  $form['config_section'] = array(
    '#type' => 'fieldset',
    '#title' => t('Config section'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['config_section']['omniture_config_section'] = array(
    '#title' => t('Configuration section'),
    '#type' => 'textarea',
    '#description' => t('Default configuration variables inserted in all pages.'),
    '#rows' => 10,
    '#default_value' => variable_get('omniture_config_section', ''),
    '#attributes' => $common_attributes,
  );
  
  $form['omniture_default_js'] = array(
    '#title' => t('Default Javascript'),
    '#type' => 'textarea',
    '#description' => t('Insert your default javascript code here. This code will be added into all pages in the document.ready event. You can override these settings for a specific page by defining a custom configuration. This is normally the only field you\'ll ever need to change.'),
    '#rows' => 20,
    '#default_value' => variable_get('omniture_default_js', ''),
    '#attributes' => $common_attributes,
  );
  
  $form['global_scope'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Scope'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['global_scope']['omniture_global_scope_js'] = array(
    '#title' => t('Global Scope Javascript'),
    '#type' => 'textarea',
    '#description' => t('Insert your javascript with global scope. This code will be added into all pages. This configuration is useful for video tracking functions.'),
    '#rows' => 10,
    '#default_value' => variable_get('omniture_global_scope_js', ''),
    '#attributes' => $common_attributes,
  );
  
  $form['plugins_section'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plugins section'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['plugins_section']['omniture_plugins_section'] = array(
    '#title' => t('Plugins section'),
    '#type' => 'textarea',
    '#description' => t('Insert/update your omniture plugins here. You can find more plugins at http://webanalyticsland.com/sitecatalyst-plugins/'),
    '#rows' => 20,
    '#default_value' => variable_get('omniture_plugins_section', ''),
    '#attributes' => $common_attributes,
  );
  
  $form['modules_section'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modules section'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['modules_section']['omniture_modules_section'] = array(
    '#title' => t('Modules section'),
    '#type' => 'textarea',
    '#description' => t('Insert/update your omniture modules here.'),
    '#rows' => 20,
    '#default_value' => variable_get('omniture_modules_section', ''),
    '#attributes' => $common_attributes,
  );
  
  $form['s_code_core'] = array(
    '#type' => 'fieldset',
    '#title' => t('CORE'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['s_code_core']['omniture_s_code_core'] = array(
    '#title' => t('Core (s_code)'),
    '#type' => 'textarea',
    '#description' => t('Omniture core code. JUST CHANGE IT IF YOU REAAAALLY KNOW WHAT YOU ARE DOING.'),
    '#rows' => 20,
    '#default_value' => variable_get('omniture_s_code_core', ''),
    '#attributes' => $common_attributes,
  );
  
  return system_settings_form($form);
}

/**
 * Form builder. Configure omniture Url settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function omniture_url_admin_settings($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');
  _omniture_include_code_snippet_syntax_highlight_feature();
  
  $form = array();
  $common_attributes = array(
    'style' => 'font-family: monospace;',
    'class' => array('use-snippet'),
  );
  
  $query = db_select('omniture', 'o')
    ->fields('o')
    ->condition('o.type', 'path', '=');
  $custom_config_paths = $query->execute();
  foreach ($custom_config_paths as $page) {
    $form['pages'][$page->oid] = array(
      '#type' => 'fieldset',
      '#title' => $page->value,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['pages'][$page->oid]['custom_js_' . $page->oid] = array(
      '#type' => 'textarea',
      '#title' => $page->value,
      '#description' => t('Insert your custom javascript code here.'),
      '#default_value' => $page->source,
      '#attributes' => $common_attributes,
    );
    
    $form['pages'][$page->oid]['delete_' . $page->oid] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete'),
      '#description' => t('Select this checkbox to delete this configuration'),
      '#default_value' => 0,
    );
  }
  
  $new_page_description = t('Enter a new page url to define its specific configuration. DO NOT include the base_path here.<br />
    Valid entries:<br />
    1. normal path         (e.g. node/5)<br />
    2. path alias          (e.g. products/product-name-1)<br />
    3. menu router path    (e.g. products/%)<br />
  ');
  $form['new_page'] = array(
    '#type' => 'textfield',
    '#title' => t('New page'),
    '#description' => $new_page_description,
    '#attributes' => array('style' => 'font-family: monospace;'),
  );
  
  $form['#validate'][] = 'omniture_url_admin_settings_validate';
  $form['#submit'][] = 'omniture_url_admin_settings_submit';
  
  return system_settings_form($form);
}

/**
 * Validate omniture url settings.
 */
function omniture_url_admin_settings_validate($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');
  
  // check if new page is a valid url
  // valid url examples:
  //  - normal path  (e.g. node/3)
  //  - path aliases (e.g. products/product_name_1
  //  - menu routers (e.g. products/%)
  if (!empty($form_state['values']['new_page'])) {
    $path_info = _omniture_get_path_info($form_state['values']['new_page']);
    if (!empty($path_info)) {
      $defined_items = _omniture_get_items_by_path($path_info['normal_path']);
      
      if (!empty($defined_items)) {
        form_set_error('new_page', t('This path is already defined.'));

        if ($defined_items[0]->value != $form_state['values']['new_page']) {
          drupal_set_message(t('Note that :normal_path and :path_alias are the same page.', 
            array(
              ':normal_path' => $path_info['normal_path'], 
              ':path_alias'  => $path_info['path_alias'],
            )
          ), 'warning');
        }
      }
    }
    else {
      form_set_error('new_page', t('Please enter a valid path.'));
    }
  }
  
}

/**
 * Process omniture url settings.
 */
function omniture_url_admin_settings_submit($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');

  // update or delete page settings
  if (isset($form['pages']) && is_array($form['pages'])) {
    foreach ($form['pages'] as $id => $page) {
      if (is_numeric($id)) {
        if ($form_state['values']['delete_' . $id] == 1) {
          $success = db_delete('omniture')
            ->condition('oid', $id)
          ->execute();
          
          if ($success) {
            drupal_set_message(t('The configurations for :url page were successfully deleted.', array(':url' => $form['pages'][$id]['custom_js_' . $id]['#title'])));
          }
        }
        else {
          $result = db_update('omniture')
            ->fields(array(
              'source' => $form_state['values']['custom_js_' . $id],
            ))
            ->condition('oid', $id, '=')
          ->execute();
        }
      }
    }
  }
  
  // add new page config
  if (!empty($form_state['values']['new_page'])) {
    
    $fields = array(
      'oid' => 0,
      'value' => $form_state['values']['new_page'],
      'type' => 'path',
      'source' => '',
    );
    
    _omniture_insert_new_entry($fields);
  }

}

/**
 * Form builder. Configure omniture content type page settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function omniture_content_type_admin_settings($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');
  _omniture_include_code_snippet_syntax_highlight_feature();
  
  $form = array();
  $common_attributes = array(
    'style' => 'font-family: monospace;',
    'class' => array('use-snippet'),
  );

  $content_types = _omniture_get_defined_content_types();
  
  $types = node_type_get_types();
  foreach ($types as $node_type) {
    $form[$node_type->type] = array(
      '#type' => 'fieldset',
      '#title' => $node_type->name,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form[$node_type->type]['textarea_'. $node_type->type] = array(
      '#type' => 'textarea',
      '#title' => $node_type->name,
      '#description' => t("Insert your custom javascript for the {$node_type->name} content type here."),
      '#default_value' => isset($content_types[$node_type->type]) ? $content_types[$node_type->type] : '',
      '#attributes' => $common_attributes,
    );
  }
  
  $form['#submit'][] = 'omniture_content_type_admin_settings_submit';
  
  return system_settings_form($form);
}

/**
 * Process omniture content type settings.
 */
function omniture_content_type_admin_settings_submit($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');

  $content_types = _omniture_get_defined_content_types();
  
  $types = node_type_get_types();
  foreach ($types as $node_type) {
    if (isset($content_types[$node_type->type])) {
      $result = db_update('omniture')
        ->fields(array(
          'source' => $form_state['values']['textarea_' . $node_type->type],
        ))
        ->condition('type', 'content_type', '=')
        ->condition('value', $node_type->type, '=')
        ->execute();
    }
    else {
      $entry = array(
        'oid' => 0,
        'value' => $node_type->type,
        'type' => 'content_type',
        'source' => $form_state['values']['textarea_' . $node_type->type],
      );
      
      _omniture_insert_new_entry($entry);
    }
    
  }
  
}

