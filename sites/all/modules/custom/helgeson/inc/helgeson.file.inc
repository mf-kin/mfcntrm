<?php
// $Id$

/**
 * @file
 * Files functionality
 */

/**
 * Deletes file from Drupal's file_managed table (if exists) and the file itself.
 *
 * @param $file_name, the file will be removed
 * 
 * @return bool TRUE on success. FALSE otherwise
 */
function helgeson_delete_file($file_name, $file_path = NULL) {
  if (empty($file_name) || !variable_get('helgeson_delete_flatfile')) {
    watchdog('helgeson', 'The file "' . $file_name . '" was not deleted. Please verify admin interface settings for Helgeson.');
    return FALSE;
  }

  $successfully_deleted = FALSE;

  $managed_file = helgeson_get_managed_file_by_file_name($file_name);

  if (!empty($managed_file)) {
    $successfully_deleted = file_delete($managed_file, TRUE);
  }
  elseif (!empty($file_path)) {
    $file_path = $file_path . '/' . $file_name;
    $successfully_deleted = file_unmanaged_delete($file_path);
  }

  if ($successfully_deleted) {
    watchdog('helgeson', 'The file "' . $file_name . '" was deleted successfully.');
  } else {
    watchdog('helgeson', 'It was not possible to delete "' . $file_name . '".');
  }

  return $successfully_deleted;
}

/**
 * Given a filename, returns its path.
 *
 * @param string $filename
 * 
 * @return string
 */
function helgeson_get_managed_file_by_file_name($filename) {
  $file_info = db_select('file_managed', 'f')
                        ->fields('f', array('fid'))
                        ->condition('filename', $filename, '=')
                        ->execute()
                        ->fetchAssoc();

  $file = NULL;
  if (!empty($file_info['fid'])) {
    $file = file_load($file_info['fid']);
  }

  return $file;
}

/**
 * Return Drupal's private file path for the site
 *
 * @return string
 */
function helgeson_get_private_files_path() {
  $path = getcwd() . '/' . variable_get('file_private_path');

  return $path;
}

/**
 * Return the local files repository path
 *
 * @return string
 */
function helgeson_get_files_repository_path() {
  $private_files_path = helgeson_get_private_files_path();
  $files_repository_path = $private_files_path . '/' . variable_get('helgeson_repository_files');

  return $files_repository_path;
}

/**
 * Return the filename to be used when saving the file before sending to Helgeson
 *
 * @return string
 */
function helgeson_get_new_file_name() {
  $file_name = 'PFIZER_' . date('Ymd_His') . '.csv';

  return $file_name;
}

/**
 * Helper function to make sure file data doesn't break generated CSV file due to
 * quotes in the content
 *
 * @param type $string
 * 
 * @return type
 */
function helgeson_replace_double_quotes_by_single_quotes($string) {
  return str_replace('"', "'", $string);
}

/**
 * Get helgeson csv file contents
 * 
 * @param string $data_to_send
 */
function helgeson_get_csv_file_contents($data_to_send) {
  // give other modules a chance to change data through this hook
  foreach (module_implements('helgeson_csv_data_alter') as $module) {
    call_user_func_array($module . '_' . 'helgeson_csv_data_alter', array(&$data_to_send));
  }

  $file_contents = '';
  if (is_array($data_to_send)) {
    foreach ($data_to_send as $item) {
      $line = implode('"' . variable_get('helgeson_csv_field_delimiter', ';') . '"', $item);
      $line = '"' . $line . '"';

      $file_contents .= $line . "\n";
    }
  }

  return $file_contents;
}

/**
 * Generates a PHP encrypted file and deletes the unencrypted source file.
 *
 * @param $file_name, the file name will be remove
 * @param $path, the source folder where the original file is located at and where it is generated
 *
 * @return The path of encrypted generated file on SUCCESS. FALSE otherwise.
 */
function helgeson_encrypt_file($file_name, $path) {
  if (!extension_loaded(HELGESON_EXTENSION_GNUPG_NAME)) {
    watchdog('helgeson', HELGESON_EXTENSION_GNUPG_NAME . ' extension not loaded.');
    return FALSE;
  }

  $input_file_path = $path . '/' . $file_name;
  $output_file_path = $path . '/' . $file_name . '.pgp';
  $pubkey = variable_get('file_private_path', '') . '/' . variable_get('helgeson_pgp_key_name');
  $gnupg_home = drupal_get_path('module', 'helgeson') . '/.gnupg';

  // Setting environment var for apache user
  putenv("GNUPGHOME={$gnupg_home}");

  $encrypted_file = NULL;

  try {
    $gnupg = new gnupg();
    $gnupg->seterrormode(gnupg::ERROR_WARNING);
    $pubkey_content = file_get_contents($pubkey);

    $input_content = file_get_contents($input_file_path);

    /*  Fix temporary files creation inside module dir
     *  after read the keychain from inside module directory, 
     *  re-set gpg home directory to were apache has write permission
    */
    putenv('GNUPGHOME='.variable_get('file_temporary_path', '/tmp'));
  
    // Import key at every encryption to not rely on GPG
    $pubkey_info = $gnupg->import($pubkey_content);

    $gnupg->addencryptkey($pubkey_info['fingerprint']);
    // setarmor=1 would include header information at the content
    $gnupg->setarmor(0);

    $output_content = $gnupg->encrypt($input_content);
    if (file_put_contents($output_file_path, $output_content)) {
      helgeson_delete_file($file_name);
      $encrypted_file = $file_name . '.pgp';
    }
  }
  catch (Exception $ex) {
    watchdog('helgeson', $ex->getMessage());
  }

  return $encrypted_file;
}