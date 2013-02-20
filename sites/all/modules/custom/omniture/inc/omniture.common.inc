<?php
/**
 * @file
 * Omniture common functions.
 */
 
function _omniture_get_request_variables() {
  $variables = array(
    'sAccount'              => variable_get('omniture_s_account', ''),
    'siteID'                => variable_get('omniture_siteID', ''),
    'visitorNamespace'      => variable_get('omniture_visitor_namespace', ''),
    'dc'                    => variable_get('omniture_dc', '122'),
    'trackingServer'        => variable_get('omniture_tracking_server', ''),
    'trackingServerSecure'  => variable_get('omniture_tracking_server_secure', ''),
    'pageNotFoundTitle'     => variable_get('omniture_404_page_name', '404'),
    'isHomePage'            => drupal_is_front_page(),
    'pageTitle'             => _omniture_get_page_title(),
    'breadcrumb'            => _omniture_get_formatted_breadcrumb(),
    'channel'               => _omniture_get_channel(),
    'object'                => _omniture_get_current_object(),
  );
  
  _omniture_html_entity_decode_recursive($variables);
  
  return $variables;
}

function _omniture_html_entity_decode_recursive(&$array) {
  foreach ($array as $key => $item) {
    if (is_string($item)) {
      $array[$key] = html_entity_decode($item, ENT_QUOTES, 'UTF-8');
    }
    elseif (is_array($item)) {
      _omniture_html_entity_decode_recursive($item);
      $array[$key] = $item;
    }
  }
}

function _omniture_get_page_title() {
  $valid_item = menu_get_item();
  
  $page_title = variable_get('omniture_404_page_name', '404');
  if (!empty($valid_item) && $valid_item['access']) {
    $page_title = drupal_get_title();
  }
  
  return $page_title;
}

function _omniture_get_formatted_breadcrumb() {
  $active_breadcrumb = drupal_get_breadcrumb();
  
  $breadcrumb = array();
  
  if (!empty($active_breadcrumb)) {
    foreach ($active_breadcrumb as $link) {
      $breadcrumb[] = strip_tags($link);
    }
  }
  
  $breadcrumb[] = _omniture_get_page_title();
  
  return $breadcrumb;
}

function _omniture_get_channel() {
  $channel = NULL;
  $breadcrumb = _omniture_get_formatted_breadcrumb();
  
  if (count($breadcrumb) > 2) {
    array_pop($breadcrumb);
  }
  
  $channel = end($breadcrumb);
  
  return $channel;
}

function _omniture_get_current_object() {
  $object = menu_get_object();
  if (!empty($object)) {
    $object = array(
      'type' => $object->type,
      'title' => $object->title,
    );
  }
  
  return $object;
}

function _omniture_define_multiple_values($variables) {
  if (is_array($variables)) {
    foreach ($variables as $name => $value) {
      omniture_set_variable($name, $value);
    }
  }
}

function _omniture_set_default_js_variables() {
  drupal_add_js(array('omniture' => omniture_get_variables()), 'setting');
}

/**
 * Include current page js.
 * It gets the s_code structure and replace each placeholder with its own configuration.
 */
function _omniture_include_current_page_js() {
  _omniture_set_default_js_variables();
  
  // default javascript code
  $default_js_code = variable_get('omniture_default_js', '');
  $default_js_code .= _omniture_get_current_page_js();
  $default_js_code .= _omniture_generate_code_from_active_common_selectors();
  
  // general selectors code
  $general_selectors = _omniture_generate_code_from_active_behavior_selectors();
  
  // s_code structure with placeholders
  $s_code_content = '';
  $s_code_content .= file_get_contents(drupal_get_path('module', 'omniture') . '/scripts/s_code.js', 'r');
  
  $patterns = array(
    '//@CONFIG_SECTION_PLACEHOLDER@',
    '//@CONFIGURABLE_JS_TRACKING_CODE_PLACEHOLDER@',
    '//@GENERAL_SELECTORS_CODE_PLACEHOLDER@',
    '//@GLOBAL_SCOPE_CONFIGURABLE_JS_TRACKING_CODE_PLACEHOLDER@',
    '//@PLUGINS_SECTION_PLACEHOLDER@',
    '//@MODULES_SECTION_PLACEHOLDER@',
    '//@S_CODE_CORE_PLACEHOLDER@',
  );
  $replaces = array(
    variable_get('omniture_config_section', ''),
    $default_js_code,
    $general_selectors,
    variable_get('omniture_global_scope_js', ''),
    variable_get('omniture_plugins_section', ''),
    variable_get('omniture_modules_section', ''),
    variable_get('omniture_s_code_core', ''),
  );
  
  $output = str_replace($patterns, $replaces, $s_code_content);

  drupal_add_js($output, array('type' => 'inline', 'scope' => 'footer', 'weight' => 5000));
}
 
/**
 * Return current page specific omniture javascript mixing all path match types.
 * Following this precedence:
 *   1. nothing
 *   2. content type definition
 *   3. menu router specification
 *   4. path exact match
 */
function _omniture_get_current_page_js() {
  $current_page = _omniture_get_path_info();
  if (empty($current_page)) {
    return '';
  }
  
  // 1
  $omniture_source = '';
  
  // 2
  $current_object = menu_get_object();
  if (!empty($current_object)) {
    $items = _omniture_get_items_by_content_type($current_object->type, 'content_type');
    if (!empty($items)) {
      $omniture_source .= $items[0]->source;
    }
  }
  
  // 3
  $items = _omniture_get_items_by_path($current_page['path_alias']);
  if (!empty($items)) {
    $omniture_source .= $items[0]->source;
  }
  
  // 4
  if ($current_page['router'] != $current_page['path_alias'] && $current_page['router'] != $current_page['normal_path']) {
    $items = _omniture_get_items_by_path($current_page['router']);
    if (!empty($items)) {
      $omniture_source .= $items[0]->source;
    }
  }
  
  return $omniture_source;
}

/**
 * Return all already defined content types in omniture table.
 */
function _omniture_get_defined_content_types() {
  $content_types = array();
  
  $query = db_select('omniture', 'o')
    ->fields('o')
    ->condition('o.type', 'content_type', '=');
  $result = $query->execute();
  
  foreach ($result as $row) {
    $content_types[$row->value] = $row->source;
  }
  
  return !empty($content_types) ? $content_types : FALSE;
}

/**
 * Insert new entry into omniture table.
 *
 * @param $fields
 *  Array containing all column fields.
 */
function _omniture_insert_new_entry($fields) {
  try {
    $entry = db_insert('omniture')
      ->fields($fields)
      ->execute();
      
    return $entry;
  }
  catch (Exception $e) {
    drupal_set_message(t('Omniture table insertion failed with the message: :msg', array(':msg' => $e->getMessage())), 'error');
  }
}

/**
 * Get path information.
 *
 * @param $path
 *   String of path to be checked, check current Url if NULL.
 *  
 * @return $path_info
 *   if menu is valid, return its:
 *    - normal path (e.g. node/3)
 *    - path alias  (e.g. products/product_name_1
 *    - menu router (e.g. products/%)
 *   empty array otherwise.
 */
function _omniture_get_path_info($path = NULL) {
  $path_info = array();
  
  // if path is empty, get current url
  if (empty($path)) {
    $path = menu_get_item();
    $path = $path['href'];
  }
  
  $normal_path = drupal_get_normal_path($path);
  $menu_router_path = _menu_find_router_path($normal_path);

  if (!empty($menu_router_path)) {
    $path_info = array(
      'normal_path' => $normal_path,
      'path_alias'  => drupal_get_path_alias($normal_path),
      'router'      => $menu_router_path,
    );
  }
  
  return $path_info;
}

/**
 * Load all items for the given $path. Check both normal path and possible aliases.
 *
 * @parm $path
 *   Path to be loaded.
 *
 * @return
 *   Entries in omniture table for the given $path. Should be only one item if used properly.
 */
function _omniture_get_items_by_path($path = NULL) {
  $items = NULL;

  $path_info = _omniture_get_path_info($path);
  
  $result = db_select('omniture', 'o')
    ->fields('o')
    ->condition(
      db_or()
      ->condition('o.value', $path_info['normal_path'], '=')
      ->condition('o.value', $path_info['path_alias'], '=')
    )
    ->condition('o.type', 'path', '=')
  ->execute();
  
  foreach ($result as $row) {
    $items[] = $row;
  }
  
  return $items;
}

/**
 * Load all items for the given $content_type.
 *
 * @parm $content_type
 *   Content type to be loaded.
 *
 * @return
 *   Entries in omniture table for the given $content_type. Should be only one item if used properly.
 */
function _omniture_get_items_by_content_type($content_type) {
  $items = array();
  
  if (empty($content_type)) {
    return $items;
  }
  
  $result = db_select('omniture', 'o')
    ->fields('o')
    ->condition('o.value', $content_type, '=')
    ->condition('o.type', 'content_type', '=')
  ->execute();
  
  foreach ($result as $row) {
    $items[] = $row;
  }
  
  return $items;
}

/**
 * Perform a selection in the database to return the whole omniture_selectors table ordered by weight.
 *
 * @return
 *  Object with resulting query.
 */
function omniture_selectors_get_all_selectors() {
  return db_select('omniture_selectors', 'os')
          ->fields('os')
          ->orderBy('os.weight')
          ->execute();
}

/** 
 * Perform a selection in the database to return all active common selectors (i.e., not behavior) from omniture_selectors table ordered by weight.
 *
 * @return
 *  Object with resulting query.
 */
function omniture_selectors_get_all_active_common_selectors() {
  return db_select('omniture_selectors', 'os')
          ->fields('os')
          ->condition('os.active', 1, '=')
          ->condition('os.is_behavior', 0, '=')
          ->orderBy('os.weight')
          ->execute();
}

/**
 * Generate full common selectors code by concatenating all active common selectors code.
 *
 * @return
 *  String with full omniture drupal behavior code.
 */
function _omniture_generate_code_from_active_common_selectors() {
  $selectors = omniture_selectors_get_all_active_common_selectors();
  
  $output = '';
  $output .= "// ------------------------------------------------------[common selectors]\n\n";
  foreach ($selectors as $selector) {
    $output .= _omniture_generate_common_selector_code($selector);
  }
  $output .= "// ------------------------------------------------------------------------\n\n";
  
  return $output;
}

/**
 * Generate one common selector code.
 *
 * @return
 *  String selector code.
 */
function _omniture_generate_common_selector_code($selector) {
  $selector_body_code = _omniture_generate_selector_body_code($selector);
  
  $code = "";
  $code .= "    // $selector->name\n";
  $code .= "    $('$selector->selector').bind('$selector->event', function() {\n";
  $code .= "      $selector_body_code";
  $code .= "    });\n\n";
  
  return $code;
}

/** 
 * Perform a selection in the database to return all active selectors from omniture_selectors table ordered by weight.
 *
 * @return
 *  Object with resulting query.
 */
function omniture_selectors_get_all_active_behavior_selectors() {
  return db_select('omniture_selectors', 'os')
          ->fields('os')
          ->condition('os.active', 1, '=')
          ->condition('os.is_behavior', 1, '=')
          ->orderBy('os.weight')
          ->execute();
}

/**
 * Generate full drupal behavior by concatenating all active behavior selectors code.
 *
 * @return
 *  String with full omniture drupal behavior code.
 */
function _omniture_generate_code_from_active_behavior_selectors() {
  $selectors = omniture_selectors_get_all_active_behavior_selectors();
  
  $output = '';
  $output .= "  Drupal.behaviors.omnitureSelectors = {\n";
  $output .= "    attach: function(context) {\n";
  foreach ($selectors as $selector) {
    $output .= _omniture_generate_behavior_selector_code($selector);
  }
  $output .= "    }\n";
  $output .= "  };\n\n";
  
  return $output;
}

/**
 * Generate one behavior selector code.
 *
 * @return
 *  String selector code.
 */
function _omniture_generate_behavior_selector_code($selector) {
  $selector_specific_class = 'omniture-processed' . $selector->osid;
  
  $selector_body_code = _omniture_generate_selector_body_code($selector);
  
  $code = "";
  $code .= "    // $selector->name\n";
  $code .= "    $('$selector->selector', context)\n";
  $code .= "      .not('.$selector_specific_class').addClass('$selector_specific_class')\n";
  $code .= "      .bind('$selector->event', function() {\n";
  $code .= "      $selector_body_code";
  $code .= "    });\n\n";
  
  return $code;
}

function _omniture_generate_selector_body_code($selector) {
  $omniture_data_json = _omniture_get_selector_omniture_data_json($selector);
  
  $code = "";
  $code .= "      $selector->code_snippet\n";
  $code .= "      var json = $omniture_data_json;\n";
  $code .= "      $.each(json.variables, function(index, element) {
                    try {
                      json.variables[index] = eval(element);
                    } catch(e) {
                      // if eval fails, use current value
                    }
                  });\n";
  $code .= "      omniture.sendData(json);\n";
  
  return $code;
}

/**
 * Return selector's variables values JSON.
 *
 * @return
 *  String with json.
 */
function _omniture_get_selector_omniture_data_json($selector) {
  $omniture_data = array(
    'variables' => array(
      $selector->variables => $selector->variables_values,
    ),
    'events' => array(
      $selector->events,
    ),
  );
  
  return drupal_json_encode($omniture_data);
}

/**
 * Include files to provide syntax highlight functionality.
 * To syntax hightlight be applied to a textarea it must have 'use-snippet' class.
 */
function _omniture_include_code_snippet_syntax_highlight_feature() {
  $module_path = drupal_get_path('module', 'omniture');
  drupal_add_css($module_path . '/scripts/plugins/jquery.snippet.2.0.0/jquery.snippet.css');
  drupal_add_js($module_path . '/scripts/plugins/jquery.snippet.2.0.0/jquery.snippet.js');
  drupal_add_js($module_path . '/scripts/omniture.code_snippet.js');
}
