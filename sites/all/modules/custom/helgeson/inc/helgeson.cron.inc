<?php
// $Id$

/**
 * @file
 * Cronjob functionality
 */

/**
 * Generates and send file to Helgeson platform
 * 
 * @return boolean
 */
function helgeson_generate_and_send_file() {
  watchdog('helgeson', 'Running cron.');

  module_load_include('inc', 'helgeson', 'inc/helgeson.file');
  module_load_include('inc', 'helgeson', 'inc/helgeson.common');

  if (!helgeson_is_integration_enabled()) {
    watchdog('helgeson', 'The file was not generated because the variable "Integration enabled" is unset.');
    return FALSE;
  }

  $current_time = time();

  $data_to_send = array();
  helgeson_populate_data_for_generating_csv($data_to_send);
  $file_contents = helgeson_get_csv_file_contents($data_to_send);

  if (empty($file_contents)) {
    watchdog('helgeson', 'The file was not sent because it was empty.');
    return FALSE;
  }

  $files_repository_path = variable_get('helgeson_repository_files');
  $inbound_folder_path = 'private://' . $files_repository_path . '/';
  $file_name = helgeson_get_new_file_name();

  $saved_file = file_save_data($file_contents, $inbound_folder_path . $file_name, FILE_EXISTS_REPLACE);

  if ($saved_file) {
    watchdog('helgeson', 'The file "' . $file_name . '" was successfully generated at ' . $inbound_folder_path);

    module_load_include('inc', 'helgeson', 'inc/helgeson.sftp');
    if (helgeson_encrypt_and_send_file($file_name)) {
      helgeson_set_last_successful_file_transfer_timestamp($current_time);
    }

    // The generated file should always be deleted. No matter if it was successfully tranfered or not
    helgeson_delete_file($file_name, $files_repository_path);
  }
}

function helgeson_populate_data_for_generating_csv(&$data_to_send) {
  foreach (module_implements('helgeson_populate_data_for_generating_csv') as $module) {
    call_user_func_array($module . '_' . 'helgeson_populate_data_for_generating_csv', array(&$data_to_send));
  }
}