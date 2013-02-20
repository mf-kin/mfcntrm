<?php
/**
 * @file
 * Special Replace admin settings
 */

/**
 * Form builder. Configure Special Replace settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function special_replace_settings_form($form, &$form_state) {
  drupal_add_tabledrag('patterns-table', 'order', 'sibling', 'patterns-weight');

  $header = array(
    'delete'        => t('Delete'),
    'pattern' => t('Original Text'),
    'replacement' => t('Replaced Text'),
    'active'        => t('Active'),
    'weight'        => t('Weight'),
  );
  
  $form['patterns_table'] = array(
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => _special_replace_get_form_table_rows(),
    '#theme' => 'table',
    '#attributes' => array('id' => 'patterns-table'),
    '#empty'  => t('No content available.'),
  );
  
  $form['#submit'][] = 'special_replace_settings_form_submit';
  
  return system_settings_form($form);
}

/**
 * Return an array of table rows, where each row contains each form field already rendered as HTML.
 *
 * @return array
 */
function _special_replace_get_form_table_rows() {  
  module_load_include('inc', 'special_replace', 'special_replace.common');
  $replace_patterns = special_replace_get_all_patterns();  
  
  $rows = array();
  $weight = 1;
  foreach ($replace_patterns as $replace_pattern) {
    $replace_pattern->weight = $weight++;
    $rows[] = _special_replace_get_row_definition_from_pattern($replace_pattern);
  }
  
  $last_replace_pattern = _special_replace_get_last_row($weight);
  $rows[] = _special_replace_get_row_definition_from_pattern($last_replace_pattern);
  
  return $rows;
}

/**
 * Return an stdClass containing the basic information for a replace pattern.
 * 
 * @param int $weight
 *
 * @return stdClass
 */
function _special_replace_get_last_row($weight = 9999) {
  $replace_pattern = new stdClass();
  $replace_pattern->srid = 0;
  $replace_pattern->weight = $weight;
  
  return $replace_pattern;
}

/**
 * Return an associative array containing the minimum row definition required by theme_table
 *
 * @param stdClass $replace_pattern
 *
 * @return array
 */
function _special_replace_get_row_definition_from_pattern($replace_pattern) {
  $row = _special_replace_get_filled_row($replace_pattern);
  $row = _special_replace_get_rendered_row_fields($row);
  
  $row_definition = array(
    'data' => $row,
    'class' => array('draggable'),
  );
  
  return $row_definition;
}

/**
 * Form submit handler of special_replace_settings_form.
 * @ingroup forms
 */
function special_replace_settings_form_submit($form, &$form_state) {
  module_load_include('inc', 'special_replace', 'special_replace.common');
  $replace_patterns = special_replace_get_all_patterns();
  foreach ($replace_patterns as $replace_pattern) {
  
    if (isset($form_state['input']['delete' . $replace_pattern->srid])) {
      $success = _special_replace_delete_entry_by_id($replace_pattern->srid);
      
      if ($success) {
        drupal_set_message(t('The configurations for original text: :pattern and replaced text: :replacement pattern were successfully deleted.', array(':pattern' => $replace_pattern->pattern, ':replacement' =>  $replace_pattern->replacement)));
      }
    }
    else {
      _special_replace_update_entry_by_id($replace_pattern->srid, $form_state);
    }
    
  }
  
  if (!empty($form_state['input']['pattern0'])) {
    _special_replace_insert_new_entry(_special_replace_get_entry_fields_from_form_state_by_id($form_state));
  }
}

/**
 *  Get array of values of entry with $srid from $form_state.
 * 
 * @param $form_state
 *  Form submited values.
 * @param $srid
 *  Entry id.
 * 
 * @return String
 *  Entry information array.
 */
function _special_replace_get_entry_fields_from_form_state_by_id(&$form_state, $srid = 0) {
  return array(
    'pattern' => $form_state['input']['pattern' . $srid],
    'replacement' => $form_state['input']['replacement' . $srid],
    'active' => isset($form_state['input']['active' . $srid]) ? $form_state['input']['active' . $srid] : 0,
    'weight' => $form_state['input']['weight' . $srid],
  );
}

/**
 *  Get entry with $srid.
 * 
 * @param $srid
 *  Entry id.
 * 
 * @return stdClass
 *  Entry object with all its fields.
 */
function _special_replace_get_pattern_by_id($srid) {
  return db_select('special_replace', 'sr')
                  ->fields('sr')
                  ->condition('srid', $srid, '=')
                  ->execute()
                  ->fetchAssoc();
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
function _special_replace_get_rendered_row_fields($row) {
  foreach($row as $field => $row_data) {
    drupal_render($row_data);
    $row[$field] = $row_data['#children'];
  }
  
  return $row;
}

/**
 * Fill values of a row for the special_replace_settings_form using values in $replace_pattern.
 * Leave $replace_pattern NULL to create an empty (new) row.
 *
 * @param $replace_pattern
 *  Array containing all pattern information.
 *
 * @return $row
 *  Array with new form row information.
 */
function _special_replace_get_filled_row($replace_pattern = NULL) {
  if (!isset($replace_pattern->srid)) {
    $replace_pattern = _special_replace_get_last_row();
  }
  
  $row = array(
    'delete' => array(
      '#name' => 'delete' . $replace_pattern->srid,
      '#type' => 'checkbox',
      '#checked' => FALSE,
    ),
    'pattern' => array(
      '#name' => 'pattern' . $replace_pattern->srid,
      '#type' => 'textfield',
      '#size' => 30,
      '#value' => isset($replace_pattern->pattern) ? $replace_pattern->pattern : '',
    ),
    'replacement' => array(
      '#name' => 'replacement' . $replace_pattern->srid,
      '#type' => 'textfield',
      '#size' => 30,
      '#value' => isset($replace_pattern->replacement) ? $replace_pattern->replacement : '',
    ),
    'active' => array(
      '#name' => 'active' . $replace_pattern->srid,
      '#type' => 'checkbox',
      '#checked' => isset($replace_pattern->active) ? $replace_pattern->active : TRUE,
    ),
    'weight' => array(
      '#name' => 'weight' . $replace_pattern->srid,
      '#type' => 'textfield',
      '#size' => 30,
      '#value' => isset($replace_pattern->weight) ? $replace_pattern->weight : 1,
      '#attributes' => array('class' => array('patterns-weight')),
    ),
  );

  return $row;
}

/**
 * Insert new entry into special_replace table.
 *
 * @param $fields
 *  Array containing all column fields from special_replace DB table.
 */
function _special_replace_insert_new_entry($fields) {
  try {
    $entry = db_insert('special_replace')
      ->fields($fields)
      ->execute();
      
    return $entry;
  }
  catch (Exception $e) {
    drupal_set_message(t('Special replace table insertion failed with the message: :msg', array(':msg' => $e->getMessage())), 'error');
  }
}

/**
 * Delete an entry from special_replace table with srid equals $srid.
 *
 * @param $srid
 *  Integer with entry id
 *
 * @return boolean
 *  Whether or not the deletion was successful.
 */
function _special_replace_delete_entry_by_id($srid) {
  $success = db_delete('special_replace')
    ->condition('srid', $srid)
    ->execute();
  
  return $success;
}

/**
 * Update an entry from special_replace table with srid equals $srid, using the values inside $form_state.
 *
 * @param $srid
 *  Integer with entry id
 */
function _special_replace_update_entry_by_id($srid, &$form_state) {
  db_update('special_replace')
    ->fields(array(
      'pattern' => $form_state['input']['pattern' . $srid],
      'replacement' => $form_state['input']['replacement' . $srid],
      'active' => isset($form_state['input']['active' . $srid]) ? 1 : 0,
      'weight' => $form_state['input']['weight' . $srid],
    ))
    ->condition('srid', $srid, '=')
    ->execute();
}