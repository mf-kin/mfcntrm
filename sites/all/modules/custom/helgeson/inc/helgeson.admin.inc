<?php
// $Id$

/**
 * @file
 * Admin settings
 */

/**
 * Form builder. General settings form.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function helgeson_general_settings_form() {
  module_load_include('inc', 'helgeson', 'inc/helgeson.common');
  _helgeson_warn_on_missing_required_php_extensions();

  $form = array();

  $form['file_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSV File Settings'),
  );

  $form['file_settings']['helgeson_delete_flatfile'] = array(
    '#type' => 'checkbox',
    '#prefix' => t('<strong>Generated flat file</strong>'),
    '#title' => t('Delete the flat file after generating the encrypted one.'),
    '#default_value' => variable_get('helgeson_delete_flatfile'),
    '#description' => t('That configuration still depends if the Integration enabled variable is set and, if unchecked, will persist the file at the %helgeson_repository_files folder.',
                        array('%helgeson_repository_files' => variable_get('helgeson_repository_files', 'path/to/helgeson/private/files'))
                      ),
  );

  $form['file_settings']['helgeson_csv_field_delimiter'] = array(
    '#type' => 'select',
    '#title' => t('CSV delimiter'),
    '#default_value' => variable_get('helgeson_csv_field_delimiter', ';'),
    '#options' => array(
      ';' => ';',
      ',' => ',',
      "\t" => '\t',
    ),
    '#description' => t('The character used to delimit CSV file fields.'),
  );

  $form['file_settings']['helgeson_csv_include_column_headers'] = array(
    '#type' => 'checkbox',
    '#prefix' => t('<strong>CSV column headers</strong>'),
    '#title' => t('Include CSV column headers'),
    '#default_value' => variable_get('helgeson_csv_include_column_headers'),
    '#description' => t('Whether or not to include column headers as the first line of the generated CSV file'),
  );

  return system_settings_form($form);
}

/**
 * Form builder. Transmission settings form.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function helgeson_file_transmission_settings_form() {
  $file_private_path = variable_get('file_private_path', '');
  if (!$file_private_path){
    drupal_set_message(t(HELGESON_MESSAGE_ERROR_PRIVATE_FILE_SYSTEM), 'error');
  }
  else {
    $form = array();

    $form['file_transmission_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('File Transmission Settings'),
    );

    $form['file_transmission_settings']['helgeson_integration_enabled'] = array(
      '#type' => 'checkbox',
      '#prefix' => t('<strong>Integration with Helgeson</strong>'),
      '#title' => t('Enable Integration with Helgeson to generate the file and submit it through the SFTP channel.'),
      '#default_value' => variable_get('helgeson_integration_enabled'),
    );

    $form['file_transmission_settings']['helgeson_run_cron_hour'] = array(
      '#type' => 'select',
      '#title' => t('Run Cron hour'),
      '#options' => _helgeson_get_batch_run_time_options(),
      '#default_value' => variable_get('helgeson_run_cron_hour', '02'),
      '#description' => t('The hour that the Helgeson file should be generated and sent.
          You must configure the CRONTAB on the Web Server to call the cron.php file in the Drupal folder at the same time that is selected here.'),
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_repository_files'] = array(
      '#type' => 'textfield',
      '#title' => t('Temporary repository Files'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#description' => t('Folder where the .csv files will be generated in Drupal Private file system path. Ex.: helgeson/files'),
      '#default_value' => variable_get('helgeson_repository_files'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_sftp_address'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Address'),
      '#description' => t('The SFTP server to send the file to. Ex: ftps.helgesonent.com'),
      '#default_value' => variable_get('helgeson_sftp_address'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_destination_repository_files'] = array(
      '#type' => 'textfield',
      '#title' => t('File destination repository'),
      '#description' => t('Folder in Helgeson server where the encrypted files will be transferred to. Ex.: /users/ftpPfizerBT/Chapstick_AYLI'),
      '#default_value' => variable_get('helgeson_destination_repository_files'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_sftp_userid'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP User name'),
      '#default_value' => variable_get('helgeson_sftp_userid'),
      '#maxlength' => 50,
      '#size' => 20,
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_sftp_password'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Password'),
      '#default_value' => variable_get('helgeson_sftp_password'),
      '#maxlength' => 50,
      '#size' => 20,
      '#required' => TRUE,
    );

    $form['file_transmission_settings']['helgeson_pgp_key_name'] = array(
      '#type' => 'textfield',
      '#title' => t('PGP Helgeson Public Key Name'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#default_value' => variable_get('helgeson_pgp_key_name'),
      '#description' => t('The location of the Helgeson Key (used to encrypt transferred files) from Drupal Private file system path'),
      '#required' => TRUE,
    );

    return system_settings_form($form);
  }
}

/**
 * Run Helgeson cronjob manually
 * @see helgeson_generate_and_send_file()
 */
function helgeson_run_cronjob_manually_form() {
  module_load_include('inc', 'helgeson', 'inc/helgeson.common');
  _helgeson_warn_on_missing_required_php_extensions();

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run Job Now'),
  );

  module_load_include('inc', 'helgeson', 'inc/helgeson.cron');
  $form['#submit'][] = 'helgeson_generate_and_send_file';

  return $form;
}

/**
 * Return an array containing the options (hours) available for scheduling
 * the cron job execution time
 *
 * @return array
 */
function _helgeson_get_batch_run_time_options() {
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