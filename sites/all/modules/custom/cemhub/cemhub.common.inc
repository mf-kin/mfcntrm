<?php
// $Id$

/**
 * @file
 * Common functions used by CEMHUB module
 */

/**
 * Return an array containing the configuration of integrated webforms
 *
 * @param boolean $only_active
 * @param int $webform_id
 *   
 * @return array $webforms
 */
function cemhub_get_integrated_webforms_settings($only_active = FALSE, $webform_id = NULL) {
  $webforms = cemhub_retrieve_integrated_webforms_settings();
  $webforms = cemhub_parse_integrated_webforms_results($webforms, $only_active, $webform_id);
  
  return $webforms;
}

/**
 * Retrive configuration of integrated webforms from static cache or from databse
 * 
 * @return array $webforms
 */
function cemhub_retrieve_integrated_webforms_settings() {
  $cache = &drupal_static(__FUNCTION__);
  
  if ($cache) {
    $webforms = $cache;
  }
  else {
    // For the first call, get all results and set in cache
    $db_select = db_select('cemhub_forms_config', 'cf')->fields('cf');
    $webforms = $db_select->execute()->fetchAll();
    
    if (!empty($webforms)) {
      // set cache
      $cache = $webforms;
    }
  }
  
  return $webforms;
}

/**
 * Parse the results of the integrated webforms
 * 
 * @param array $webforms
 * @param boolean $only_active
 *   If set to true, just the enabled webforms are returned
 * @param int $webform_id
 *   If is set, returns only the specified webform
 * 
 * @return array $parsed_webforms
 */
function cemhub_parse_integrated_webforms_results($webforms, $only_active, $webform_id) {
  $parsed_webforms = $webforms;
  
  if ($only_active && is_null($webform_id)) {
    $active_webforms = array();
    
    foreach ($webforms as $key => $webform) {
      if ($webform->active) {
        $active_webforms[] = $webform;
      }
    }
  
    $parsed_webforms = $active_webforms;
  }
  elseif (!is_null($webform_id)) {
    foreach ($webforms as $key => $webform) {
      if ($webform->nid == $webform_id) {
        $specific_webform = $webform;
      }
    }
    
    if (isset($specific_webform)) {
      $parsed_webforms = $specific_webform;
    }
    else {
      $parsed_webforms = NULL;
    }
  }
  
  return $parsed_webforms;
}

/**
 * Returns whether or not the CEMHUB integration is enabled
 * @return bool
 */
function cemhub_is_integration_enabled() {
  return (bool)variable_get('cemhub_integration_enabled');
}

/**
 * Updates the CEMHUB configuration table
 *
 * @param array $webform_config
 */
function cemhub_update_webform_config($webform_config) {
  db_update('cemhub_forms_config')
          ->fields($webform_config)
          ->condition('nid', $webform_config['nid'], '=')
          ->execute();
}

/**
 * Return the limit of submissions per day expected by the CEM HUB by webform
 *
 * @param int $webform_id
 * @return integer $limit
 */
function cemhub_get_limit_submissions_per_day($webform_nid) {
  $webforms_settings = cemhub_get_integrated_webforms_settings(FALSE, $webform_nid);
  $limit = isset($webforms_settings->limit_submissions) ? $webforms_settings->limit_submissions : CEMHUB_DEFAULT_LIMIT_SUBMISSION;

  return $limit;
}

/**
 * Save information of integrated webform in config database table
 *
 * @param array $webform_config
 */
function cemhub_insert_new_entry_of_webform_config($webform_config) {
  db_insert('cemhub_forms_config')
  ->fields($webform_config)
  ->execute();
}

/**
 * Return an array with the DOB years
 * 
 * @return array $years
 */
function cemhub_get_dob_years() {
  //Based on the current date we get the first year going back 100 years.
  $current_date = getdate();
  $first_year = $current_date['year'] - 100;

  $years = array('' => t('YYYY'));
  for($year = $current_date['year']; $year >= $first_year; $year--) {
    $years[$year] = $year;
  }

  return $years;
}

/**
 * Check all the required environment settings
 * 
 * @param boolean $set_message
 * Set to TRUE if want to return a warning message in case of errors on validation
 * @param boolean $log_error
 * 
 * @return boolean $return
 *  return TRUE if ALL settings are configured correctly, FALSE otherwise
 */
function cemhub_check_required_environment_settings($set_message = TRUE, $log_error = FALSE) {
  // Clear all error messages
  drupal_get_messages('warning');

  $return = FALSE;
  
  if (cemhub_get_private_file_system_path()) {

    $checked_values = array(
      cemhub_check_temporary_repository_files($set_message),
      cemhub_check_sftp_keys($set_message),
      cemhub_check_pgp_public_key($set_message),
      cemhub_check_required_php_extensions($set_message),
    );

    $return = in_array(FALSE, $checked_values) ? FALSE : TRUE;
  }
  elseif ($set_message) {
    drupal_set_message(t('This module Requires Drupal Private file system path where private files will be stored. <br> Set on: Configuration > Media > File system.'), 'error');
  }
  
  if (empty($return) && $log_error) {
    watchdog('cemhub', 'Error on integration! Some environment settings are not configured correctly.');
  }
  
  return $return;
}

/**
 * Checks if required PHP extensions are loaded. 
 * If not, print warnings to the screen
 * 
 * @param array $extensions_list
 *   An array with the required extensions
 * @param boolean $set_message
 *   If the parameter is TRUE and the validation fail, returns a warning message
 *   
 * @return array $return
 */
function cemhub_check_required_php_extensions($set_message = TRUE, $extensions_list = array(CEMHUB_EXTENSION_SSH_NAME, CEMHUB_EXTENSION_GNUPG_NAME)) {
  $return = FALSE;
  $return_extensions_status = array();

  foreach ($extensions_list as $extension) {
    $extension_loaded = extension_loaded($extension);
    $return_extensions_status[$extension] = $extension_loaded;

    if (empty($extension_loaded) && $set_message) {
      drupal_set_message(t('The extension "' . $extension . '" must be installed and enabled on the server.'), 'warning');
    }
  }

  // Check results
  if (!empty($return_extensions_status)) {
    $return = in_array(FALSE, $return_extensions_status) ? FALSE : TRUE;
  }
  
  return $return;
}

/**
 * Check if PGP public key exist and is readable
 * 
 * @param boolean $set_message
 *  If the parameter is TRUE and the validation fail, returns a warning message
 *  
 * @return boolean $is_readable
 */
function cemhub_check_pgp_public_key($set_message = TRUE) {
  $key_location = 'private://' . variable_get('cemhub_pgp_key_name');
  $is_readable = is_readable($key_location);
  
  if (empty($is_readable) && $set_message) {
    drupal_set_message(t('The PGP Public Key "' . $key_location . '" does not exist or is not readable, please fix this.'), 'warning');
  }
  
  return $is_readable;
}

/**
 * Check if temporary repository files exist and is writable
 * 
 * @param boolean $set_message
 *  If the parameter is TRUE and the validation fail, returns a warning message
 *  
 *  @return boolean $checked_temporary_repository
 */
function cemhub_check_temporary_repository_files($set_message = TRUE) {
  $temp_files_path = cemhub_get_files_repository_path(TRUE);
  $checked_temporary_repository = file_prepare_directory($temp_files_path);
  
  if (empty($checked_temporary_repository) && $set_message) {
    drupal_set_message(t('Temporary repository files "' . $temp_files_path . '" does not exist or is not writable, please fix this.'), 'warning');
  }
  
  return $checked_temporary_repository;
}

/**
 * Checks if the SFTP keys are in their location
 * 
 * @param array $keys
 * @param boolean $set_message
 *  If the parameter is TRUE and the validation fail, returns a warning message
 *  
 * @return boolean $return
 */
function cemhub_check_sftp_keys($set_message = TRUE, $keys = array('public', 'private')) {
  $return = FALSE;
  $return_key_status = array();
  
  foreach ($keys as $key_type) {
    $retrieved_stored_key = variable_get('cemhub_sftp_' . $key_type . '_key');
    $key_exists = $retrieved_stored_key ? file_exists('private://' . $retrieved_stored_key) : 'No variable to stored key';
    
    $return_key_status[$key_type] = $key_exists;
    
    if (empty($key_exists) && $set_message) {
      drupal_set_message(t('SFTP ' . $key_type . ' Key does not exist in "' . $retrieved_stored_key . '", provide the key in the correct location.'), 'warning');
    }
  }
  
  // Check results
  if (!empty($return_key_status)) {
    $return = in_array(FALSE, $return_key_status) ? FALSE : TRUE;
  }
  
  return $return;
}

/**
 * Return relative Drupal's private file path for the site
 * If the first parameter is TRUE, return the absolute path
 *
 * @param boolean $absolute_path
 * 
 * @return string $return_path
 */
function cemhub_get_private_file_system_path($absolute_path = FALSE) {
  $stored_path = variable_get('file_private_path', FALSE);
  
  $return_path = $stored_path;
  
  if ($stored_path && $absolute_path) {
    $return_path = getcwd() . '/' . $stored_path;
  }
  
  return $return_path;
} 

/**
 * Return the local CEMHUB files repository path
 *
 * @param boolean $drupal_internal_path
 * @return string $files_repository_path
 */
function cemhub_get_files_repository_path($drupal_internal_path = FALSE) {
  $private_files_path = cemhub_get_private_file_system_path(TRUE);
  $stored_path = variable_get('cemhub_repository_files');
  
  if ($drupal_internal_path) {
    $files_repository_path = 'private://' . $stored_path;
  }
  else {
    $files_repository_path = $private_files_path . '/' . $stored_path;
  }

  return $files_repository_path;
}

/**
 * Return an array with all the webforms available in the system
 *
 * @return array
 */
function cemhub_get_available_webforms() {
  $available_webforms = db_select('node', 'nd')
  ->fields('nd', array('title','nid'))
  ->condition('type', 'webform')
  ->execute()
  ->fetchAll();

  return $available_webforms;
}

/**
 * Returns an array with the form data formatted in the format expected by functions of database integration
 *
 * @see cemhub_insert_new_entry_of_webform_config
 * @see cemhub_update_webform_config
 *
 * @param array $form_state
 * @param array $nid
 *
 * @return array $formated_data
 */
function cemhub_forms_settings_formats_data_to_database_integration($form_state, $nid) {
  $fields_names = cemhub_get_integrated_webforms_names_fields_settings();

  $formated_data = array();
  foreach ($fields_names as $key => $column) {
    if ($column != 'id' && $column != 'nid') {
      $formated_data[$column] = $form_state['values']['cemhub_form_nid' . $nid . '_' . $column];
    }
  }
  $formated_data['nid'] = $nid;

  return $formated_data;
}

/**
 * Get the name of the columns of the table form config
 *
 * @see cemhub_forms_settings_formats_data_to_database_integration
 */
function cemhub_get_integrated_webforms_names_fields_settings() {
  $cache = &drupal_static(__FUNCTION__);

  if ($cache) {
    $fields_names = $cache;
  }
  else {
    $fields_names = db_query('SHOW COLUMNS FROM {cemhub_forms_config}')->fetchCol();

    if (!empty($fields_names)) {
      // set cache
      $cache = $fields_names;
    }
  }

  return $fields_names;
}

/**
 * Return an array containing the 'title' and 'type' properties of the given
 * field id (identified by $component_id)
 *
 * @param int $nid The fields's parent webform nid
 * @param int $component_id Webform field component id
 * @return array
 */
function _cemhub_get_webform_field_properties($nid, $component_id) {
  $field_properties = db_select('webform_component', 'wc')
  ->fields('wc', array('form_key', 'type'))
  ->condition('wc.nid', $nid, '=')
  ->condition('wc.cid', $component_id, '=')
  ->execute()
  ->fetchAssoc();

  return $field_properties;
}

/**
 * Return the mapped webform mapped identified by CEMHUB module
 *
 * @param int $nid webform nid
 * @return array
 */
function _cemhub_get_mapped_webform_fields_by_nid($nid) {
  $fields = db_select('cemhub_fields', 'cf')
  ->fields('cf', array('cid', 'nid', 'question_code', 'answer_code', 'active'))
  ->condition('cf.nid', $nid, '=')
  ->execute();

  return $fields;
}

/**
 * Return an array containing the options (hours) available for scheduling
 * the cron job execution time
 *
 * @return array
 */
function _cemhub_get_batch_run_time_options() {
  $hours = array();

  for ($hour = 0; $hour <= 23; $hour++) {
    $formatted_hour = str_pad($hour, 2, '0', STR_PAD_LEFT);
    $hours[$formatted_hour] = $formatted_hour . ' hr';

    if ($hour > 1) {
      $hours[$formatted_hour] .= 's';
    }
  }

  return $hours;
}

/**
 * Creates a CEMHUB mapping for the given field
 *
 * @param int $cid
 * @param int $nid
 * @param string $question_code
 * @param string $answer_code
 * @param int $active
 */
function _cemhub_create_webform_fields_mapping($field_cid, $webform_nid, $question_code, $answer_code, $active) {
  db_insert('cemhub_fields')
  ->fields(
    array(
      'cid'           => $field_cid,
      'nid'           => $webform_nid,
      'question_code' => $question_code,
      'answer_code'   => $answer_code,
      'active'        => $active,
    )
  )
  ->execute();
}

/**
 * Updates a CEMHUB mapping for the given field
 *
 * @param int $cid
 * @param int $nid
 * @param string $question_code
 * @param string $answer_code
 * @param int $active
 */
function _cemhub_update_webform_fields_mapping($field_cid, $webform_nid, $question_code, $answer_code, $active) {
  db_update('cemhub_fields')
  ->fields(
    array(
      'question_code' => $question_code,
      'answer_code'   => $answer_code,
      'active'        => $active,
    )
  )
  ->condition('cid', $field_cid, '=')
  ->condition('nid', $webform_nid, '=')
  ->execute();
}

/**
 * Deletes the CEMHUB mapping for the given field
 *
 * @param int $cid
 * @param int $nid
 */
function _cemhub_delete_webform_field_mapping($cid, $nid) {
  db_delete('cemhub_fields')
  ->condition('cid', $cid)
  ->condition('nid', $nid)
  ->execute();
}

/**
 * Compare the two tables ({webform_component} and {cemhub_fields}) and synchronizes them
 *
 * @param webform id to compare
 */
function cemhub_sync_tables($webform_nid) {
  if (!empty($webform_nid)) {
    $webform_fields_cids = _cemhub_get_webform_field_ids($webform_nid);
    $cemhub_mapped_fields_cids = _cemhub_get_mapped_field_ids($webform_nid);

    $fields_to_insert = array_diff($webform_fields_cids, $cemhub_mapped_fields_cids);
    $fields_to_delete = array_diff($cemhub_mapped_fields_cids, $webform_fields_cids);

    if (!empty($fields_to_insert)) {
      foreach ($fields_to_insert as $cid_to_insert) {
        $question_code = '';
        $answer_code   = '';
        $active        = 0;

        _cemhub_create_webform_fields_mapping(
          $cid_to_insert,
          $webform_nid,
          $question_code,
          $answer_code,
          $active
        );
      }
    }

    if (!empty($fields_to_delete)) {
      foreach ($fields_to_delete as $cid_to_delete) {
        _cemhub_delete_webform_field_mapping($cid_to_delete, $webform_nid);
      }
    }
  }
}

/**
 * Return the IDs of the fields that remain to the given webform
 *
 * @param int $webform_nid
 * @return array $webform_fields_cids
 */
function _cemhub_get_webform_field_ids($webform_nid) {
  $webform_fields_cids = db_select('webform_component', 'wc')
  ->fields('wc', array('cid'))
  ->condition('nid', $webform_nid, '=')
  ->execute()
  ->fetchCol();

  return $webform_fields_cids;
}

/**
 * Return the IDs of the fields mapped by CEMHUB module
 *
 * @param int $webform_nid
 * @return array $cemhub_mapped_fields_cids
 */
function _cemhub_get_mapped_field_ids($webform_nid) {
  $cemhub_mapped_fields_cids = db_select('cemhub_fields', 'cf')
  ->fields('cf', array('cid'))
  ->condition('nid', $webform_nid, '=')
  ->execute()
  ->fetchCol();

  return $cemhub_mapped_fields_cids;
}

/**
 * Delete all webform references from CEMHUB
 *
 * @param int $nid
 */
function cemhub_cleanup_webform_references($nid) {
  db_delete('cemhub_forms_config')->condition('nid', $nid)->execute();
  db_delete('cemhub_fields')->condition('nid', $nid)->execute();
}

/**
 * Checks if the current time is in the same hour range specified in admin form
 * and if the integration by cron has not ran today
 *
 * @return boolean $should_perform_job
 *  True if the integration must be performed, False otherwise
 */
function cemhub_cron_should_perform_job() {
  $last_cron_run_day = variable_get('cemhub_last_cron_run', date('Ymd', strtotime("yesterday")));
  $current_day = date('Ymd');

  $is_in_scheduled_hour_range = (variable_get('cemhub_batch_run_time') == date('H', REQUEST_TIME));
  $hasnt_ran_today = ($last_cron_run_day < $current_day);

  $should_perform_job = ($is_in_scheduled_hour_range && $hasnt_ran_today);

  return $should_perform_job;
}