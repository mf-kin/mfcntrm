<?php
// $Id$

/**
 * @file
 * Forms to manage settings on the CEMHUB administration
 */

/**
 *
 * Form constructor for the CEM HUB registration.
 */
function cemhub_vendor_registration($form, &$form_state) {
  cemhub_check_required_environment_settings();
  
  $file_private_path = cemhub_get_private_file_system_path();
  if ($file_private_path){
    $form['vendor_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Vendor Detail'),
      '#collapsible' => TRUE,
    );

    $form['vendor_settings']['cemhub_vendor_id'] = array(
      '#type' => 'textfield',
      '#title' => t('CEM HUB Vendor ID'),
      '#maxlength' => 3,
      '#size' => 3,
      '#default_value' => variable_get('cemhub_vendor_id'),
      '#description' => t('An alpha code provided by Pfizer. Ex: ACC.'),
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_organization_code'] = array(
      '#type' => 'textfield',
      '#title' => t('UII Root'),
      '#maxlength' => 6,
      '#size' => 6,
      '#default_value' => variable_get('cemhub_organization_code'),
      '#description' => t('This number is used to build the 24-digit "UII".'),
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_repository_files'] = array(
      '#type' => 'textfield',
      '#title' => t('Temporary repository Files'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#description' => t('Folder where the .txt files will be generated in Drupal Private file system path. Ex.: cemhub/files'),
      '#default_value' => variable_get('cemhub_repository_files'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_file_sequence'] = array(
      '#type' => 'textfield',
      '#title' => t('File Sequence Number'),
      '#description' => t('Fix the file sequence number to avoid conflict between files sent for different sites in the same day.
        <br>If this field is empty, the file sequence number starting with “1” for the first file and increments by 1 for any additional files sent in the same day.'),
      '#field_prefix' => 'PFIZ_' . variable_get('cemhub_vendor_id', 'CIT') . '_CHNLIN_' . date('Ymd') . '_',
      '#default_value' => variable_get('cemhub_file_sequence'),
      '#field_suffix' => '.txt.pgp',
      '#maxlength' => 2,
      '#size' => 2,
      '#required' => FALSE,
    );
    
    $form['vendor_settings']['cemhub_batch_run_time'] = array(
      '#type' => 'select',
      '#title' => t('Run the Daily Batch Process as:'),
      '#options' => _cemhub_get_batch_run_time_options(),
      '#default_value' => variable_get('cemhub_batch_run_time', '04'),
      '#description' => t('Time is allowed to run the batch process to generate and send the file to CEM HUB during the Drupal Cron.<br>
        Notifying that the hour to send the file for vendors must occur 4AM EST.<br>
        You must configure the CRONTAB on the Web Server to call the cron.php file in the Drupal folder at the same time that is selected here.'),
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_sftp_address'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Address'),
      '#description' => t('Groton, CT USA: sft-am.pfizer.com <br> Sandwich, UK: sft-eu.pfizer.com <br> Tokyo, Japan: sft-ap.pfizer.com'),
      '#default_value' => variable_get('cemhub_sftp_address'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_destination_repository_files'] = array(
      '#type' => 'textfield',
      '#title' => t('File destination repository'),
      '#description' => t('Folder in CEM server where the encrypted files will be transferred. Ex.: home/pfizcit/Outbound'),
      '#default_value' => variable_get('cemhub_destination_repository_files'),
      '#maxlength' => 100,
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_sftp_userid'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP User name'),
      '#default_value' => variable_get('cemhub_sftp_userid'),
      '#maxlength' => 50,
      '#size' => 7,
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_sftp_pass_ppk'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Passphrase'),
      '#default_value' => variable_get('cemhub_sftp_pass_ppk'),
      '#maxlength' => 50,
      '#size' => 20,
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_sftp_public_key'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Public Key Path'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#default_value' => variable_get('cemhub_sftp_public_key'),
      '#description' => t('The location of the Public key from Drupal Private file system path. <br> Notice: The public key, MUST be in OpenSSH authorized_keys file format.<br>Ex. path: cemhub/pgp522.pub'),
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_sftp_private_key'] = array(
      '#type' => 'textfield',
      '#title' => t('SFTP Private Key Path'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#default_value' => variable_get('cemhub_sftp_private_key'),
      '#description' => t('The location of the PPK Key from Drupal Private file system path.<br> Notice: The public key, MUST be in OpenSSH format.<br> Ex. path: cemhub/CEM_channel.ppk'),
      '#required' => TRUE,
    );

    $form['vendor_settings']['cemhub_pgp_key_name'] = array(
      '#type' => 'textfield',
      '#title' => t('PGP CEM Public Key Name'),
      '#field_prefix' => $file_private_path ? $file_private_path . '/' : '',
      '#default_value' => variable_get('cemhub_pgp_key_name'),
      '#description' => t('The location of the CEM Key from Drupal Private file system path'),
      '#required' => TRUE,
    );

    return system_settings_form($form);
  }
}

/**
 * Form constructor for CEMHUB default template content settings
 *
 * @param array $form
 * @param array $form_state
 *
 * @return system_settings_form
 */
function cemhub_default_template_content_form($form, &$form_state) {
  cemhub_check_required_environment_settings();
  if (cemhub_get_private_file_system_path()) {
    $form['default_template_content'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default Template Content'),
    );

    $form['default_template_content']['cemhub_default_template_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => variable_get('cemhub_default_template_title'),
      '#description' => t('The title of the Sign Up form page'),
    );

    $form['default_template_content']['cemhub_default_template_description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => variable_get('cemhub_default_template_description'),
      '#description' => t('The description of the Sign Up form page (shown below the title)'),
    );

    $form['default_template_content']['cemhub_default_template_subtitle'] = array(
      '#type' => 'textfield',
      '#title' => t('Subtitle'),
      '#default_value' => variable_get('cemhub_default_template_subtitle'),
      '#description' => t('The subtitle of the Sign Up form page (shown below the description)'),
    );

    $form['default_template_content']['cemhub_default_template_optins_subtitle'] = array(
      '#type' => 'textfield',
      '#title' => t('Opt-ins Subtitle'),
      '#default_value' => variable_get('cemhub_default_template_optins_subtitle'),
      '#description' => t('The subtitle shown above newsletter opt-ins'),
    );

    $form['default_template_content']['cemhub_default_template_disclaimer'] = array(
      '#type' => 'textfield',
      '#title' => t('Disclaimer'),
      '#default_value' => variable_get('cemhub_default_template_disclaimer'),
      '#description' => t('The disclaimer text shown at the bottom of the page (next to the Submit button)'),
    );

    return system_settings_form($form);
  }
}

/**
 * Form constructor for the CEM HUB Integration Settings.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function cemhub_integration_settings_form($form, &$form_state){
  cemhub_check_required_environment_settings();
  
  if (cemhub_get_private_file_system_path()) {
    $form['integration_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Integration Settings'),
    );

    $form['integration_settings']['cemhub_integration_enabled'] = array(
      '#type' => 'checkbox',
      '#prefix' => t('<b>Integration with HUB</b>'),
      '#title' => t('Enable Integration with HUB to generate the file and submit it through the SFTP channel.'),
      '#default_value' => variable_get('cemhub_integration_enabled'),
      '#description' => t('Very careful with this configuration. Only enable it if you are sure that the integration with HUB and all the configuration fields above are correctly working.'),
    );

    $form['integration_settings']['cemhub_delete_flatfile'] = array(
      '#type' => 'checkbox',
      '#prefix' => t('<b>Generated flat file</b>'),
      '#title' => t('Delete the flat file after generating the encrypted one.'),
      '#default_value' => variable_get('cemhub_delete_flatfile'),
      '#description' => t('That configuration still depends if the Integration enabled variable is set and if unchecked, will persist the file at the ' . variable_get('cemhub_repository_files') . ' folder.'),
    );

    $form['integration_settings']['cemhub_delete_entries'] = array(
      '#type' => 'checkbox',
      '#prefix' => t('<b>Database entries</b>'),
      '#title' => t('Delete the entries at the database after generating the flat file.'),
      '#default_value' => variable_get('cemhub_delete_entries'),
    );
    
    $form['integration_settings']['cemhub_last_cron_run'] = array(
      '#type' => 'textfield',
      '#title' => t('Last integration by Cron'),
      '#description' => 'Timestamp of the last successful integration performed by Cron. Format: YYYMMDD.<br>Leave blank if you want to reset.',
      '#default_value' => variable_get('cemhub_last_cron_run'),
      '#maxlength' => 8,
      '#size' => 20,
    );

    return system_settings_form($form);
  }
}

/**
 * Form constructor for the webforms Settings.
 * This form define which webforms will be integrated with CEM HUB
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function cemhub_forms_settings_form($form, &$form_state){
  cemhub_check_required_environment_settings();

  if (cemhub_get_private_file_system_path()) {
    $available_webforms = cemhub_get_available_webforms();

    $webforms_to_update = array();
    foreach ($available_webforms as $webform) {
      $nid = $webform->nid;
      $webfom_title = 'Webform: ' . $webform->title . ' (nid ' . $nid . ')';
      $webforms_to_update[] = $nid;

      $integrated_webforms = cemhub_get_integrated_webforms_settings(FALSE, $nid);
      $webform_config = !empty($integrated_webforms) ? get_object_vars($integrated_webforms) : NULL;

      $form['form_nid' . $nid . '_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => $webfom_title,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form['form_nid' . $nid . '_fieldset']['campaign'] = array(
        '#type' => 'fieldset',
        '#title' => 'Campaign',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['form_nid' . $nid . '_fieldset']['layout'] = array(
        '#type' => 'fieldset',
        '#title' => 'Layout',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form['form_nid' . $nid . '_fieldset']['date-field'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field DOB',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form['form_nid' . $nid . '_fieldset']['integration'] = array(
        '#type' => 'fieldset',
        '#title' => 'Integration',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['form_nid' . $nid . '_fieldset']['campaign']['cemhub_form_nid' . $nid . '_campaign_source'] = array(
        '#type' => 'textfield',
        '#title' => t('Campaign Source ID'),
        '#size' => 25,
        '#maxlength' => 25,
        '#default_value' => isset($webform_config['campaign_source']) ? $webform_config['campaign_source'] : '',
        '#description' => t('The CEM Campaign ID that is related to this Webform'),
      );

      $form['form_nid' . $nid . '_fieldset']['campaign']['cemhub_form_nid' . $nid . '_survey_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Survey ID'),
        '#size' => 6,
        '#maxlength' => 6,
        '#default_value' => isset($webform_config['survey_id']) ? $webform_config['survey_id'] : '',
        '#description' => t('The CEM Survey ID that is related to this Webform'),
      );

      $form['form_nid' . $nid . '_fieldset']['layout']['cemhub_form_nid' . $nid . '_template_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Template Name'),
        '#default_value' => isset($webform_config['template_name']) ? $webform_config['template_name'] : '',
        '#description' => t('Set the name of form template. To use the default template provided by the module, leave it as <strong>'. CEMHUB_BASE_TEMPLATE . '</strong>'),
      );

      $form['form_nid' . $nid . '_fieldset']['date-field']['cemhub_form_nid' . $nid . '_group_date_fields'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<b>Group date fields</b>',
        '#title' => t('Whether or not to send the date fields as a single entry in the text file.'),
        '#default_value' => isset($webform_config['group_date_fields']) ? $webform_config['group_date_fields'] : 1,
        '#description' => t('If checked, use the same answer/question codes for both Year and Month fields.'),
      );

      $form['form_nid' . $nid . '_fieldset']['date-field']['cemhub_form_nid' . $nid . '_age_validation'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<b>Age validation</b>',
        '#title' => t('Whether or not to use 18 age validation in the field DOB.'),
        '#default_value' => isset($webform_config['age_validation']) ? $webform_config['age_validation'] : 1,
      );

      $form['form_nid' . $nid . '_fieldset']['integration']['cemhub_form_nid' . $nid . '_last_submission_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Last webform entry that was submitted'),
        '#default_value' => empty($webform_config['last_submission_id']) ? '-1' : $webform_config['last_submission_id'],
        '#size' => 10,
        '#description' => t('You can reset that value to -1 if you want to submit all entries again to the HUB, but this flag is the tracking about what was already sent.'),
      );

      $form['form_nid' . $nid . '_fieldset']['integration']['cemhub_form_nid' . $nid . '_limit_submissions'] = array(
        '#type' => 'textfield',
        '#title' => t('Limit of UII Submissions per day'),
        '#description' => t('Limit of submissions per day. The default value is 9999.<br>
          Care to change this field: If the records exceed the expected limit, all the records starting from the limit number will be rejected.'),
        '#default_value' => cemhub_get_limit_submissions_per_day($nid),
        '#maxlength' => 10,
        '#size' => 10,
      );

      $form['form_nid' . $nid . '_fieldset']['integration']['cemhub_form_nid' . $nid . '_active'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<b>Integration</b>',
        '#title' => t('Enable Integration with HUB'),
        '#default_value' => isset($webform_config['active']) ? $webform_config['active'] : 0,
        '#description' => t('All Submissions of this form will be sent to the CEM HUB'),
      );
    }

    $form['cemhub_update_nid_list'] = array(
      '#type' => 'hidden',
      '#value' => $webforms_to_update,
    );

    $form['actions']['submit'] = array(
      '#type'  => 'submit',
      '#value' => t('Save Settings'),
    );

    return $form;
  }
}

/**
 * Form Submit for the webforms Settings.
 *
 * @param array $form
 * @param array $form_state
 */
function cemhub_forms_settings_form_submit($form, &$form_state){
  form_state_values_clean($form_state);
  
  $webforms_to_update = $form_state['values']['cemhub_update_nid_list'];
  foreach ($webforms_to_update as $nid) {
    $integrate_data = cemhub_forms_settings_formats_data_to_database_integration($form_state, $nid);
    
    $webform_already_exists = cemhub_get_integrated_webforms_settings(FALSE, $nid);
    if ($webform_already_exists) {
      // Update existing
      cemhub_update_webform_config($integrate_data);
    }
    else {
      // Create new entry in table
      cemhub_insert_new_entry_of_webform_config($integrate_data);
    }
  }

  drupal_set_message(t('Fields have been successfully saved'));
}

/**
 * Run Job CEMHUB manually
 * @see cemhub_process_submissions()
 */
function cemhub_run_manually() {
  module_load_include('inc', 'cemhub', 'cemhub.submission');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run Job Now'),
    '#disabled' => cemhub_check_required_environment_settings() ? FALSE : TRUE,
  );
  $form['#submit'][] = 'cemhub_process_submissions';

  return $form;
}

/**
 *
 * Form for listing the webform fields for mapping the fields of CEMHUB
 */
function cemhub_mapping_fields() {
  cemhub_check_required_environment_settings();
  
  if (cemhub_get_private_file_system_path()) {
    $webforms_to_update = array();

    $enabled_webforms = cemhub_get_integrated_webforms_settings(TRUE);

    if (!empty($enabled_webforms)) {
      foreach ($enabled_webforms as $webform) {
        $webform = node_load($webform->nid);
        $webforms_to_update[] = $webform->nid;

        cemhub_sync_tables($webform->nid);

        $form['fields_table_nid' . $webform->nid] = array(
          '#type' => 'fieldset',
          '#description' => t('Define the Questions and the Answers of the fields that will be sent to CEMHUB.
            <p>PS.: If this form has a Field <b>Day of Birthday</b> with two selects <b>Month</b> and <b>Year</b> and the Answer expected by the CEM HUB is <b>"YYYYMM01"</b>, use these names for the fields:</p>
            <p><b>DOB Month:</b> ' . CEMHUB_FIELD_DOB_MONTH . '<br> <b>DOB Year:</b> ' . CEMHUB_FIELD_DOB_YEAR . '</p>'),
          '#title' => t('Webform: ' . $webform->title),
          '#collapsible' => TRUE,
          '#collapsed'   => TRUE,
        );

        $cemhub_fields = _cemhub_get_mapped_webform_fields_by_nid($webform->nid);

        $options = array();
        foreach ($cemhub_fields as $field) {
          $field_properties =  _cemhub_get_webform_field_properties($webform->nid, $field->cid);

          $options['cemhub_fields_cid' . $field->cid . 'nid_' . $webform->nid] = array(
            'user_field' => $field_properties['form_key'],
            'question_code' =>  array(
              'data' => array(
                '#type' => 'textfield',
                '#name'  => 'cid_' . $field->cid . '_nid' . $webform->nid . '_question',
                '#value' => $field->question_code,
                '#size' => 6,
                '#maxlength' => 6,
              ),
            ),
            'answer_code' =>  array(
              'data' => array(
                '#type' => ($field_properties['type'] == 'select') ? 'textarea' : 'textfield',
                '#name'  => 'cid_' . $field->cid . '_nid' . $webform->nid . '_answer',
                '#value' => $field->answer_code,
                '#description' => ($field_properties['type'] == 'select') ? t('The default value in case of multiple values is the first row. Ex.: default_value|answer') : '',
                '#size' => 6,
                '#maxlength' => 6,
              ),
            ),
            'status' => array(
              'data' => array(
                '#type' => 'checkbox',
                '#name'  => 'cid_' . $field->cid . '_nid' . $webform->nid . '_status',
                '#checked' => $field->active ? TRUE : FALSE,
              ),
            ),
          );
        }

        // table header
        $form['fields_table_nid' . $webform->nid]['cemhub_fields'] = array(
          '#theme' => 'table',
          '#header' => array(
            'user_field'    => t('Webform Field Key'),
            'question_code' => t('Question Code'),
            'answer_code'   => t('Answer Code'),
            'status'        => t('Status'),
          ),
          '#rows' => $options,
          '#empty' => t('No content available.'),
          '#weight' => 7,
        );
      }

      $form['cemhub_webforms_to_update'] = array(
        '#type' => 'hidden',
        '#value' => $webforms_to_update,
      );

      $form['actions']['submit'] = array(
        '#type'  => 'submit',
        '#value' => t('Save Table'),
      );

      return $form;

    }
    else {
      drupal_set_message(t('No webform has been integrated. Access the tab: "FORMS SETTINGS"'), 'warning');
    }
  }
}

/**
 * Form Submit for cemhub_mapping_fields
 * 
 * @see cemhub_mapping_fields
 *
 * @param array $form
 * @param array $form_state
 */
function cemhub_mapping_fields_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  $webforms_to_update = $form_state['values']['cemhub_webforms_to_update'];

  foreach ($webforms_to_update as $nid) {
    $cemhub_fields = _cemhub_get_mapped_webform_fields_by_nid($nid);

    if (!empty($cemhub_fields)) {
      foreach ($cemhub_fields as $field) {
        $question_code = $form_state['input']['cid_' . $field->cid . '_nid' . $nid . '_question'];
        $answer_code   = $form_state['input']['cid_' . $field->cid . '_nid' . $nid . '_answer'];
        $active        = isset($form_state['input']['cid_' . $field->cid . '_nid' . $nid . '_status']) ? 1 : 0;

        _cemhub_update_webform_fields_mapping(
          $field->cid,
          $nid,
          $question_code,
          $answer_code,
          $active
        );
      }
    }
  }

  drupal_set_message(t('Fields have been successfully saved'));
}
