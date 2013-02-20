<?php
/**
 * @file
 * Omniture selectors admin settings
 */
 
/**
 * Form builder. Configure omniture tracking through jQuery selectors.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function omniture_general_selectors_admin_settings($form, &$form_state) {
  module_load_include('inc', 'omniture', 'inc/omniture.common');
  _omniture_include_code_snippet_syntax_highlight_feature();
  
  $module_path = drupal_get_path('module', 'omniture');
  drupal_add_js($module_path . '/scripts/omniture.general_selectors.js');
  drupal_add_css($module_path . '/theme/omniture.general_selectors.css');

  drupal_add_tabledrag('general-selectors-table', 'order', 'sibling', 'general-selectors-weight');
  
  $form = array();
  $form['howto'] = array(
    '#markup' => theme('omniture_selectors_admin_form_header'),
  );
  
  $header = array(
    'delete'    => t('Delete'),
    'selectors'  => t('Selectors'),
    'active'    => t('Active'),
    'weight'    => t('Weight'),
  );
  
  $form['general_selectors_table'] = array(
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => _omniture_selectors_get_form_table_rows(),
    '#theme' => 'table',
    '#attributes' => array('id' => 'general-selectors-table'),
    '#empty'  => t('No content available.'),
  );
  
  $form['#submit'][] = 'omniture_general_selectors_admin_settings_submit';
  
  return system_settings_form($form);
}

/**
 * Return an array of table rows, where each row contains each form field already rendered as HTML.
 *
 * @return array
 */
function _omniture_selectors_get_form_table_rows() {
  module_load_include('inc', 'omniture', 'omniture.common');
  $selectors = omniture_selectors_get_all_selectors();  

  $rows = array();
  $weight = 1;
  foreach ($selectors as $selector) {
    $selector->weight = $weight++;
    $rows[] = _omniture_selectors_get_row_definition_from_selector($selector);
  }
  
  $last_selector = _omniture_selectors_get_last_row($weight);
  $rows[] = _omniture_selectors_get_row_definition_from_selector($last_selector);

  return $rows;
}

/**
 * Return an associative array containing the minimum row definition required by theme_table
 *
 * @param stdClass $selector
 *
 * @return array
 */
function _omniture_selectors_get_row_definition_from_selector($selector) {
  $row = _omniture_selectors_get_filled_row($selector);
  $row = _omniture_selectors_get_rendered_row_fields($row);
  
  $row_definition = array(
    'data' => $row,
    'class' => array('draggable'),
  );
  
  return $row_definition;
}

/**
 * Return an stdClass containing the basic information for a selector.
 * 
 * @param int $weight
 *
 * @return stdClass
 */
function _omniture_selectors_get_last_row($weight = 9999) {
  $selector = new stdClass();
  $selector->osid = 0;
  $selector->weight = $weight;
  $selector->name = 'new selector';
  
  return $selector;
}

/**
 * Fill values of a row for omniture_general_selectors_settings using values in $selector.
 * Leave $selector NULL to create an empty (new) row.
 *
 * @param $selector
 *  Array containing all selector information.
 *
 * @return $row
 *  Array with new form row information.
 */
function _omniture_selectors_get_filled_row($selector = NULL) {
  if (!isset($selector->osid)) {
    $selector = _omniture_selectors_get_last_row();
  }
  
  $row = array(
    'delete' => array(
      '#name'     => 'delete' . $selector->osid,
      '#type'     => 'checkbox',
      '#checked'  => FALSE,
      '#attributes'   => array('class' => array('delete-checkbox')),
    ),
    'selectors' => array(
      '#title'        => $selector->name,
      '#type'         => 'fieldset',
      '#collapsible'  => TRUE,
      '#collapsed'    => TRUE,
      '#attributes'   => array('class' => array('selectors-fieldset')),
      'name' => array(
        '#title'  => 'Selector name',
        '#name'   => 'name' . $selector->osid,
        '#description'  => 'Selector name for better identification.',
        '#type'   => 'textfield',
        '#size'   => 100,
        '#value'  => isset($selector->name) ? $selector->name : '',
      ),
      'selector' => array(
        '#title'  => 'jQuery selector',
        '#name'   => 'selector' . $selector->osid,
        '#description'  => 'jQuery selector to identify to which elements the event shall be binded to.',
        '#type'   => 'textfield',
        '#size'   => 100,
        '#value'  => isset($selector->selector) ? $selector->selector : '',
      ),
      'event' => array(
        '#title'    => 'Trigger on',
        '#name'     => 'event' . $selector->osid,
        '#description'  => 'Which event will trigger omniture tracking.',
        '#type'     => 'select',
        '#options'  => _omniture_get_selector_trigger_options(),
        '#value'    => isset($selector->event) ? $selector->event : '',
      ),
      'code_snippet' => array(
        '#title'        => 'Code snippet',
        '#name'         => 'code_snippet' . $selector->osid,
        '#description'  => 'Code to insert inside the binded event. Process anything needed before sending data to omniture here. This field have access to <strong>$</strong> jQuery variable.<br />All variables defined here are accessible to "Variables values" field.',
        '#type'         => 'textarea',
        '#value'        => isset($selector->code_snippet) ? $selector->code_snippet : '',
        '#attributes'   => array(
          'class' => array('use-snippet'),
          'style' => 'font-family: monospace;',
        ),
      ),
      'variables_values' => array(
        '#title'  => 'Variables values',
        '#name'   => 'variables_values' . $selector->osid,
        '#description'  => 'All variables will be set with this value. omnitureInfo object is accessible here. Variables defined previously in Code Snippet field are accessible here.',
        '#type'   => 'textfield',
        '#size'   => 100,
        '#value'  => isset($selector->variables_values) ? $selector->variables_values : '',
        '#attributes'   => array('class' => array('use-snippet')),
      ),
      'variables' => array(
        '#title'  => 'Variables',
        '#name'   => 'variables' . $selector->osid,
        '#description'  => 'Variables to track when event is triggered. Insert all variables separated by comma, be careful with case sensitive variable names (e.g., e<strong>V</strong>ar, page<strong>N</strong>ame).',
        '#type'   => 'textfield',
        '#size'   => 100,
        '#value'  => isset($selector->variables) ? $selector->variables : '',
      ),
      'events' => array(
        '#title'  => 'Events',
        '#name'   => 'events' . $selector->osid,
        '#description'  => 'Events to track when trigger event occurs. Insert all events here separated by comma.',
        '#type'   => 'textfield',
        '#size'   => 100,
        '#value'  => isset($selector->events) ? $selector->events : '',
      ),
      'is_behavior' => array(
        '#title'  => 'Is behavior?',
        '#name'   => 'is_behavior' . $selector->osid,
        '#description'  => 'Whether this rule should create a behavior or be placed inside document.ready().',
        '#type'     => 'checkbox',
        '#checked'  => isset($selector->is_behavior) ? $selector->is_behavior : TRUE,
      ),
      'updated' => array(
        '#name'           => 'updated' . $selector->osid,
        '#type'           => 'hidden',
        '#default_value'  => 0,
      ),
    ),
    'active' => array(
      '#name'     => 'active' . $selector->osid,
      '#type'     => 'checkbox',
      '#checked'  => isset($selector->active) ? $selector->active : TRUE,
    ),
    'weight' => array(
      '#name'       => 'weight' . $selector->osid,
      '#type'       => 'textfield',
      '#size'       => 10,
      '#value'      => isset($selector->weight) ? $selector->weight : 1,
      '#attributes' => array('class' => array('general-selectors-weight')),
    ),
  );
  

  return $row;
}

/**
 * Return rendered HTML of $row.
 *
 * @param $row
 *  Array with form api definition of all fields inside the $row
 *
 * @return
 *  Array with fields already rendered
 */
function _omniture_selectors_get_rendered_row_fields($row) {
  foreach($row as $field => $row_data) {
    if ($row_data['#type'] == 'fieldset') {
      _omniture_process_collapsed_fieldset_elements_inside_table($row_data);
    }
    drupal_render($row_data);
    $row[$field] = $row_data['#children'];
  }
  
  return $row;
}

function _omniture_process_collapsed_fieldset_elements_inside_table(&$element) {
  // Collapsible fieldsets
  if (!empty($element['#collapsible'])) {
    $element['#attached']['library'][] = array('system', 'drupal.collapse');
    $element['#attributes']['class'][] = 'collapsible';
    if (!empty($element['#collapsed'])) {
      $element['#attributes']['class'][] = 'collapsed';
    }
  }
}

/**
 * Return events that trigger a selector.
 *
 * @return
 *  Array with events
 */
function _omniture_get_selector_trigger_options() {
  return array(
    'click'      => 'Click',
    'change'     => 'Change',
    'focus'      => 'Focus',
    'blur'       => 'Blur',
    'mouseenter' => 'Mouse enter',
    'mouseleave' => 'Mouse leave',
    'mousedown'  => 'Mouse down',
  );
}

/**
 * Form submit handler of omniture_general_selectors_admin_settings.
 * @ingroup forms
 */
function omniture_general_selectors_admin_settings_submit($form, &$form_state) {
  module_load_include('inc', 'omniture', 'omniture.common');
  
  $selectors = omniture_selectors_get_all_selectors();
  foreach ($selectors as $selector) {
  
    if (isset($form_state['input']['delete' . $selector->osid])) {
      $success = _omniture_selectors_delete_entry_by_id($selector->osid);
      
      if ($success) {
        drupal_set_message(t('The configurations for original selector: ":selector" were successfully deleted.', array(':selector' => $selector->selector)));
      }
    }
    else if ($form_state['input']['updated' . $selector->osid] == 1) {
      _omniture_selectors_update_entry_by_id($selector->osid, $form_state);
    }
    else {
      // no need for update
    }
    
  }
  
  if (!empty($form_state['input']['selector0'])) {
    $new_selector = _omniture_selectors_get_entry_fields_from_form_state_by_id($form_state);
    _omniture_selectors_insert_new_entry($new_selector);
  }
  
}

/**
 *  Get array of values of entry with $osid from $form_state.
 * 
 * @param $form_state
 *  Form submited values.
 * @param $osid
 *  Entry id.
 * 
 * @return String
 *  Entry information array.
 */
function _omniture_selectors_get_entry_fields_from_form_state_by_id(&$form_state, $osid = 0) {
  return array(
    'name'              => $form_state['input']['name' . $osid],
    'selector'          => $form_state['input']['selector' . $osid],
    'event'             => $form_state['input']['event' . $osid],
    'code_snippet'      => $form_state['input']['code_snippet' . $osid],
    'variables'         => $form_state['input']['variables' . $osid],
    'variables_values'  => $form_state['input']['variables_values' . $osid],
    'events'            => $form_state['input']['events' . $osid],
    'is_behavior'       => isset($form_state['input']['is_behavior' . $osid]) ? $form_state['input']['is_behavior' . $osid] : 0,
    'active'            => isset($form_state['input']['active' . $osid]) ? $form_state['input']['active' . $osid] : 0,
    'weight'            => $form_state['input']['weight' . $osid],
  );
}

/**
 *  Get entry with $osid.
 * 
 * @param $osid
 *  Entry id.
 * 
 * @return stdClass
 *  Entry object with all its fields.
 */
function _omniture_selectors_get_pattern_by_id($osid) {
  return db_select('omniture_selectors', 'os')
                  ->fields('os')
                  ->condition('osid', $osid, '=')
                  ->execute()
                  ->fetchAssoc();
}

/**
 * Insert new entry into omniture_selectors table.
 *
 * @param $fields
 *  Array containing all column fields from omniture_selectors DB table.
 */
function _omniture_selectors_insert_new_entry($fields) {
  try {
    $entry = db_insert('omniture_selectors')
      ->fields($fields)
      ->execute();
      
    return $entry;
  }
  catch (Exception $e) {
    drupal_set_message(t('Omniture selector insertion failed with the message: :msg', array(':msg' => $e->getMessage())), 'error');
  }
}

/**
 * Delete an entry from omniture_selectors table with osid equals $osid.
 *
 * @param $osid
 *  Integer with entry id
 *
 * @return boolean
 *  Whether or not the deletion was successful.
 */
function _omniture_selectors_delete_entry_by_id($osid) {
  $success = db_delete('omniture_selectors')
    ->condition('osid', $osid)
    ->execute();
  
  return $success;
}

/**
 * Update an entry from omniture_selectors table with osid equals $osid, using the values inside $form_state.
 *
 * @param $osid
 *  Integer with entry id
 */
function _omniture_selectors_update_entry_by_id($osid, &$form_state) {
  $entry_fields = _omniture_selectors_get_entry_fields_from_form_state_by_id($form_state, $osid);
  db_update('omniture_selectors')
    ->fields($entry_fields)
    ->condition('osid', $osid, '=')
    ->execute();
}