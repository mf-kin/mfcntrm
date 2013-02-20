<?php
// $Id$

/**
 * @file
 * SFTP functionality
 */

/**
 * Encrypt and send helgeson file
 * 
 * @param string $file_name
 */
function helgeson_encrypt_and_send_file($file_name) {
  $is_successful = FALSE;

  module_load_include('inc', 'helgeson', 'inc/helgeson.file');

  $files_repository_path = helgeson_get_files_repository_path();
  $file_name_encrypted = helgeson_encrypt_file($file_name, $files_repository_path);

  if ($file_name_encrypted) {
    $file_destination = '/' . variable_get('helgeson_destination_repository_files') . '/' . $file_name_encrypted;
    $source_file_path = $files_repository_path . '/' . $file_name_encrypted;

    if (_helgeson_sftp_send_file($file_destination, $source_file_path)) {
      $is_successful = TRUE;

      watchdog('helgeson', 'The file ' . $file_name_encrypted . ' was successfully sent to Helgeson SFTP');
    }
    else {
      watchdog('helgeson', 'Failed sending file to Helgeson SFTP');
    }

    helgeson_delete_file($file_name_encrypted, $files_repository_path);
  }

  return $is_successful;
}

/**
 * Send file to Helgeson using a SFTP connection
 *
 * @param string $file_destination
 * @param string $source_file_path
 * 
 * @return boolean
 */
function _helgeson_sftp_send_file($file_destination, $source_file_path) {
  if (!extension_loaded(HELGESON_EXTENSION_SSH_NAME)) {
    watchdog('helgeson', HELGESON_EXTENSION_SSH_NAME . ' not loaded.');
    return FALSE;
  }

  $server   = variable_get('helgeson_sftp_address');
  $username = variable_get('helgeson_sftp_userid');
  $password = variable_get('helgeson_sftp_password');

  $is_successful = FALSE;

  $connection_id = ssh2_connect($server, 22);
  
  if (ssh2_auth_password($connection_id, $username, $password)) {
    watchdog('helgeson', 'Successfuly connected to Helgeson SFTP.');

    // Create SFTP session
    $sftp = ssh2_sftp($connection_id);
    $sftp_stream = @fopen('ssh2.sftp://' . $sftp . $file_destination, 'w');

    $is_successful = TRUE;
    try {
      if (!$sftp_stream) {
        throw new Exception("Could not open remote file: $file_destination");
      }

      $data_to_send = @file_get_contents($source_file_path);
      if ($data_to_send === FALSE) {
        throw new Exception("Could not open local file: $source_file_path.");
      }

      if (@fwrite($sftp_stream, $data_to_send) === FALSE) {
        throw new Exception("Could not send data from file: $source_file_path.");
      }
    } catch (Exception $e) {
      watchdog('helgeson', 'Exception: ' . $e->getMessage());
      $is_successful = FALSE;
    }

    fclose($sftp_stream);
  }
  else {
    watchdog('helgeson', 'Failed connecting to Helgeson SFTP.');
  }

  return $is_successful;
}