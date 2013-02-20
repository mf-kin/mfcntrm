<?php
// $Id$

/**
 * @file
 * Responsible for generating, encrypting and sending the file to CEMHUB
 */

/**
 *
 * function to generate the file to be sent to the CEMHUB
 * - This function must be called only by hook_cron at production. The manual
 * execution should be disabled.
 */
function cemhub_process_submissions() {
  // Double check for integration_enabled flag
  if (!cemhub_is_integration_enabled()) {
    watchdog('cemhub', 'The file was not generated because the variable "Integration enabled" is unset. The data are still at the database for future integration layer.');
    return FALSE;
  }

  $integrated_webforms = cemhub_get_integrated_webforms_settings(TRUE);
  if (empty($integrated_webforms)) {
    watchdog('cemhub', 'The file was not generated because there is no form selected to be integrated with CEM');
    return FALSE;
  }

  $vendor_id = variable_get('cemhub_vendor_id');

  $record_count = 0;
  $total_lines  = 0;
  $file_lines   = '';
  $last_submissions_by_webform = array();

  foreach ($integrated_webforms as $webform) {
    $last_submissions_by_webform[$webform->nid] = _cemhub_get_last_webform_submission_id($webform->nid, $webform->last_submission_id);
    
    // WARNING: $record_count and $total_lines are passed by referece below
    $file_lines .= _cemhub_get_file_lines_from_webform_submissions($webform, $vendor_id, $total_lines, $record_count);
  }
  
  if (!empty($file_lines)) {
    $file_data = _cemhub_get_file_contents_with_trailer_records($vendor_id, $total_lines, $record_count, $file_lines);

    $files_repository_path = variable_get('cemhub_repository_files');
    $inbound_folder_path = 'private://' . $files_repository_path . '/';
    $file_name = _cemhub_get_new_file_name($vendor_id);

    $saved_file = file_save_data($file_data, $inbound_folder_path . $file_name, FILE_EXISTS_REPLACE);

    if ($saved_file) {
      watchdog('cemhub', 'The file "' . $file_name . '" was successfully generated in the folder: ' . $inbound_folder_path);
      if (cemhub_send_file($file_name)) {
        _cemhub_update_config_with_last_submission_ids($last_submissions_by_webform);
        cemhub_delete_submissions($last_submissions_by_webform);
      }
      else {
        // The generated file should always be deleted. No matter if it was successfully tranfered or not
        cemhub_delete_file($file_name, $files_repository_path);
      }

    }
    else {
      watchdog('cemhub', 'The file "' . $file_name . '" can not be generated, make sure the permissions and folders are set properly. Folder: ' . $inbound_folder_path);
    }
  }
  else {
    watchdog('cemhub', 'The file was not generated and sent because does not exist data to integrate.');
  }
}

/**
 * Given a string containing the records to send to CEMHUB, return it with the
 * trailer records (first and last lines) expected by the CEMHUB parser
 *
 * @param string $vendor_id
 * @param int $record_count
 * @param int $total_lines
 * @param string $file_lines
 * @return string
 */
function _cemhub_get_file_contents_with_trailer_records($vendor_id, $total_lines, $record_count, $file_lines) {
  $trailer_record = _cemhub_get_trailer_file_record($vendor_id, $total_lines, $record_count);
  $file_content = $trailer_record . $file_lines . $trailer_record;

  return $file_content;
}

/**
 * Builds the trailer record used to wrap the registries sent to CEMHUB
 *
 * @param string $vendor_id
 * @param int $record_count
 * @param int $total_lines
 * @return string
 */
function _cemhub_get_trailer_file_record($vendor_id, $total_lines, $record_count) {
  $trailer_record = $vendor_id . "|" . $total_lines . "|" . $record_count . "|" . date('Ymd') . "|" . date('His') . "|\r\n";

  return $trailer_record;
}

/**
 * Return a string containing the registries to be sent to CEMHUB, based on the
 * webform submissions
 *
 * @param stdClass $webform
 * @param string $vendor_id
 * @param int $record_count
 * @param int $total_lines
 * @return string
 */
function _cemhub_get_file_lines_from_webform_submissions($webform, $vendor_id, &$total_lines, &$record_count) {
  $root_uii = variable_get('cemhub_organization_code');

  $file_lines = '';

  $webform_submissions = _cemhub_get_webform_submissions_since_last_submission_id($webform->nid, $webform->last_submission_id);
  if (!empty($webform_submissions)) {
    foreach ($webform_submissions as $webform_submission) {
      $rows = _cemhub_get_submission_fields($webform->nid, $webform_submission->sid);
      
      if ($rows) {
        // updated by reference. used outside
        $record_count++;

        $uii = _cemhub_get_uii($root_uii, $record_count);


        //This is the default campaign source
        $campaign_src = $webform->campaign_source;

        if(isset($rows['Q_SRC'])){

          //If we have a campaign Src and it is different from the default one (overriden), we have to use it instead
          //The overriden campaign is passed on the Q_SRC question.
          if($rows['Q_SRC']['data'] != $webform->campaign_source) {
            $campaign_src = $rows['Q_SRC']['data'];
          }

        }

        foreach ($rows as $row)  {
          // Can not be sent to an empty answer
          if (!empty($row['data']) && $row['question_code']!='Q_SRC') {
            // updated by reference. used outside
            $total_lines++;

            $file_lines .= _cemhub_build_file_line(
              $vendor_id,
              date("Ymd", $webform_submission->submitted),
              $campaign_src,
              $uii,
              $webform->survey_id,
              $row['question_code'],
              $row['answer_code'],
              $row['data']
            );
          }
        }
      }
    }
  }

  return $file_lines;
}

/**
 * Return an array with the submission fields
 * 
 * @param int $webform_nid
 * @param int $webform_submission_id
 * 
 * @return array $rows
 */
function _cemhub_get_submission_fields($webform_nid, $webform_submission_id) {
  $rows = array();

  $submitted_fields_result_set = cemhub_get_webform_submission_fields_as_result_set($webform_nid, $webform_submission_id);
  while ($submitted_field = $submitted_fields_result_set->fetchAssoc()) {
    $answer_details = _cemhub_get_answer_details($submitted_field);

    $submitted_field['answer_code'] = $answer_details['code'];
    $submitted_field['data'] = $answer_details['value'];

    if($submitted_field['question_code']=='Q_SRC') {
      $rows['Q_SRC'] = $submitted_field;
    }
    else{
      $rows[$submitted_field['form_key']] = $submitted_field;
    }
  }

  // implements hook_cemhub_data_alter
  foreach (module_implements('cemhub_data_alter') as $module) {
    call_user_func_array($module . '_' . 'cemhub_data_alter', array(&$rows, $webform_nid));
  }

  return $rows;
}

/**
 * Return an array containing the Answer code/Answer value
 *
 * @param array $submitted_field
 * @return array
 */
function _cemhub_get_answer_details($submitted_field) {
  $answer_value = '';
  $answer_code  = '';

  $answer_map = list_extract_allowed_values($submitted_field['answer_code'], 'list_text', FALSE);

  $is_select_field = count($answer_map) > 1;
  if ($is_select_field) {
    if (empty($submitted_field['data'])) {
      // define default value if no data submited
      $answer_value = array_keys($answer_map);
      $answer_value = $answer_value[0];
      $answer_code = $answer_map[$answer_value];
    }
    else {
      // set the value selected according to the options webform
      $answer_value = $submitted_field['data'];
      $answer_code = $answer_map[$submitted_field['data']];
    }
  }
  else {
    // answer field type is textfield
    $answer_value = $submitted_field['data'];
    $answer_code = array_shift($answer_map);
  }

  $answer_details = array(
    'code'  => $answer_code,
    'value' => $answer_value,
  );

  return $answer_details;
}

/**
 * Return the last submission id for the given webform nid
 *
 * @param int $nid
 * @param int $last_sid
 * @return int
 */
function _cemhub_get_last_webform_submission_id($nid, $latest_submission) {
  $limit = cemhub_get_limit_submissions_per_day($nid);

  $query = <<<QUERY
    SELECT COALESCE(MAX(sid), -1) AS last_sid
      FROM ( 
        SELECT sid 
          FROM {webform_submissions}
         WHERE nid = :nid
           AND sid > :sid
         LIMIT $limit
      ) AS total
QUERY;

  $args = array(
    ':nid' => $nid,
    ':sid' => $latest_submission,
  );

  $last_submission = db_query($query, $args)->fetchField();
  
  if ($last_submission == '-1') {
    $last_submission = $latest_submission;
  }
  
  return $last_submission;
}

/**
 * Return the filename to be used when saving the file before sending to CEMHUB
 *
 * @param string $vendor_id
 * @return string
 */
function _cemhub_get_new_file_name($vendor_id) {
  $file_id = variable_get('cemhub_file_sequence');
  if (empty($file_id)) {
    $file_id = 1;
  }

  $file_name = 'PFIZ_' . $vendor_id . '_CHNLIN_' . date('Ymd') . '_' . $file_id . '.txt';

  return $file_name;
}

/**
 * Return a formatted file line to be sent to CEMHUB
 *
 * @param string $vendor_id
 * @param int $submission_timestamp Must be a UNIX timestamp converted by the function date("Ymd",$date)
 * @param string $campain_source
 * @param int $uii
 * @param string $survey_id
 * @param string $question_code
 * @param string $answer_code
 * @param string $value
 * @return string
 */
function _cemhub_build_file_line($vendor_id, $submission_timestamp, $campain_source,
                                 $uii, $survey_id, $question_code, $answer_code, $value) {
  $line = $vendor_id            . '|' .
          $submission_timestamp . '|' .
          trim($campain_source) . '|' .
          $uii                  . '|' .
          $survey_id            . '|' .
          $question_code        . '|' .
          $answer_code          . '|' .
          $value                . "|\r\n";

  return $line;
}

/**
 * Return the UII value based on the Root UII, plus the current iteration (when
 * generating the file)
 *
 * @param string $root_uii
 * @param int $record_count
 * @return string
 */
function _cemhub_get_uii($root_uii, $record_count) {
  $uii = $root_uii . date('mdYHis') . str_pad($record_count, 4, '0', STR_PAD_LEFT);

  return $uii;
}

/**
 * Encrypt file and send to CEMHUB
 *
 * @param File name
 * @param Last submission id
 */
function cemhub_send_file($file_name) {
  $is_successful = FALSE;

  $files_repository_path = cemhub_get_files_repository_path();
  $file_name_encrypted = cemhub_encrypt_file($file_name, $files_repository_path);

  if ($file_name_encrypted) {
    $file_destination = '/' . variable_get('cemhub_destination_repository_files') . '/' . $file_name_encrypted;
    $source_file_path = $files_repository_path . '/' . $file_name_encrypted;

    if (_cemhub_sftp_send_file($file_destination, $source_file_path)) {
      $is_successful = TRUE;

      watchdog('cemhub', 'The encrypted file was successfully sent to CEMHUB: ' . $file_name_encrypted);
    }
    else {
      watchdog('cemhub', 'Failed to connect CEM SFTP');
    }

    cemhub_delete_file($file_name_encrypted, $files_repository_path);
  }

  return $is_successful;
}

/**
 * Send file to CEMHUB using a SFTP connection
 *
 * @param string $file_destination
 * @param string $source_file_path
 * @return boolean
 */
function _cemhub_sftp_send_file($file_destination, $source_file_path) {
  if (!extension_loaded(CEMHUB_EXTENSION_SSH_NAME)) {
    watchdog('cemhub', CEMHUB_EXTENSION_SSH_NAME . ' not loaded.');
    return FALSE;
  }

  $server = variable_get('cemhub_sftp_address');
  $username = variable_get('cemhub_sftp_userid');

  $private_files_path = cemhub_get_private_file_system_path(TRUE);
  $certpub = $private_files_path . '/' . variable_get('cemhub_sftp_public_key');
  $cert = $private_files_path . '/' . variable_get('cemhub_sftp_private_key');
  $passphrase = variable_get('cemhub_sftp_pass_ppk');

  $is_successful = FALSE;

  $connection_id = ssh2_connect($server, 22);
  if (ssh2_auth_pubkey_file($connection_id, $username, $certpub, $cert, $passphrase)) {
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
      watchdog('cemhub', 'Exception: ' . $e->getMessage());
      $is_successful = FALSE;
    }

    fclose($sftp_stream);
  }

  return $is_successful;
}

/**
 * Update CEMHUB with the last webform submission ids
 *
 * @param array $last_submissions_by_webform
 */
function _cemhub_update_config_with_last_submission_ids($last_submissions_by_webform) {
  foreach ($last_submissions_by_webform as $nid => $last_submission_id) {
    $config = array(
      'nid' => $nid,
      'last_submission_id' => $last_submission_id,
    );
    cemhub_update_webform_config($config);
  }
}

/**
 * Delete webform submissions in the tables: webform_submissions and webform_submitted_data
 *
 * @param last submission id
 */
function cemhub_delete_submissions($submissions) {
  if (empty($submissions) || !variable_get('cemhub_delete_entries')) {
    watchdog('cemhub', 'Submissions were not deleted. Please verify admin interface settings for CEM HUB.');
    return FALSE;
  }

  foreach ($submissions as $nid => $last_submission) {
    db_delete('webform_submitted_data')
            ->condition('sid', $last_submission, '<=')
            ->condition('nid', $nid)
            ->execute();

    db_delete('webform_submissions')
            ->condition('sid', $last_submission, '<=')
            ->condition('nid', $nid)
            ->execute();
  }

  watchdog('cemhub', 'The submissions were successfully excluded');
}

/**
 * Deletes file from Drupal's file_managed table (if exists) and the file itself.
 *
 * @param $file_name, the file will be removed
 * @return bool TRUE on success. FALSE otherwise
 */
function cemhub_delete_file($file_name, $file_path = NULL) {
  if (empty($file_name) || !variable_get('cemhub_delete_flatfile')) {
    watchdog('cemhub', 'The file "' . $file_name . '" was not deleted. Please verify admin interface settings for CEM HUB.');
    return FALSE;
  }

  $successfully_deleted = FALSE;

  $managed_file = _cemhub_get_managed_file_by_file_name($file_name);

  if (!empty($managed_file)) {
    $successfully_deleted = file_delete($managed_file, TRUE);
  }
  elseif (!empty($file_path)) {
    $file_path = $file_path . '/' . $file_name;
    
    $successfully_deleted = file_unmanaged_delete($file_path);
  }

  if ($successfully_deleted) {
    watchdog('cemhub', 'The file "' . $file_name . '" was deleted successfully.');
  } else {
    watchdog('cemhub', 'It was not possible to delete file "' . $file_name . '" he may already have been deleted.');
  }

  return $successfully_deleted;
}

/**
 * Given a filename, returns its path.
 *
 * @param string $filename
 * @return string
 */
function _cemhub_get_managed_file_by_file_name($filename) {
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
 * PGP encrypts the file and delete the source file
 *
 * @param $file_name, the file name will be remove
 * @param $path, the source folder where the original file is located and where it is generated
 * @return The path of encrypted generated file on SUCCESS. FALSE otherwise.
 */
function cemhub_encrypt_file($file_name, $path) {
  if (!extension_loaded(CEMHUB_EXTENSION_GNUPG_NAME)) {
    watchdog('cemhub', CEMHUB_EXTENSION_GNUPG_NAME . ' extension not loaded.');
    return FALSE;
  }

  $input_file_path = $path . '/' . $file_name;
  $output_file_path = $path . '/' . $file_name . '.pgp';
  $pubkey = cemhub_get_private_file_system_path() . '/' . variable_get('cemhub_pgp_key_name');
  $gnupg_home = drupal_get_path('module', 'cemhub') . '/.gnupg';

  // Setting environment var for apache user
  putenv("GNUPGHOME={$gnupg_home}");

  $gnupg = new gnupg();
  
  // Set errormode to PHP WARNING rather than raise an exception
  $gnupg->seterrormode(GNUPG_ERROR_WARNING);
  
  $pubkey_content = file_get_contents($pubkey);
  $input_content = file_get_contents($input_file_path);

  // Fix temporary files creation inside module dir
  putenv('GNUPGHOME=' . variable_get('file_temporary_path', '/tmp'));
  
  // Import key at every encryption to not rely on GPG
  $pubkey_info = $gnupg->import($pubkey_content);
  $gnupg->addencryptkey($pubkey_info['fingerprint']);
  // setarmor=1 will include header information at the content
  $gnupg->setarmor(0);

  $encrypted_file = NULL;
  $output_content = $gnupg->encrypt($input_content);
  if (file_put_contents($output_file_path, $output_content)) {
    cemhub_delete_file($file_name);
    $encrypted_file = $file_name . '.pgp';
    watchdog('cemhub', 'The encrypted file was successfully generated: ' . $output_file_path);
  }

  return $encrypted_file;
}

/**
 * Return all the webform submissions since the last submission id passed as argument
 *
 * @param int $webform_nid
 * @param int $last_submission_id
 * @return array
 */
function _cemhub_get_webform_submissions_since_last_submission_id($webform_nid, $last_submission_id) {
  $limit_per_day = cemhub_get_limit_submissions_per_day($webform_nid);
  
  $webform_submissions = db_select('webform_submissions', 's')
                                  ->fields('s', array('nid', 'sid', 'submitted'))
                                  ->condition('s.sid', $last_submission_id, '>')
                                  ->condition('s.nid', $webform_nid, '=')
                                  ->range(0, $limit_per_day)
                                  ->execute()
                                  ->fetchAll();

  return $webform_submissions;
}

/**
 * Return all the data of a webform submission
 *
 * @param int $webform_nid
 * @param int $submission_id
 * @return DatabaseStatementInterface
 */
function cemhub_get_webform_submission_fields_as_result_set($webform_nid, $submission_id) {
  $query = db_select('webform_submitted_data', 'w');
  $query->join('cemhub_fields', 'f', 'f.cid = w.cid');
  $query->join('webform_component', 'c', 'c.cid = w.cid AND c.nid = f.nid');
  $query->fields('w', array('data'))
  ->fields('f', array('question_code', 'answer_code'))
  ->fields('c', array('form_key'))
  ->condition('f.nid', $webform_nid, '=')
  ->condition('f.active', 1, '=')
  ->condition('w.sid', $submission_id, '=');

  $result_set = $query->execute();

  return $result_set;
}