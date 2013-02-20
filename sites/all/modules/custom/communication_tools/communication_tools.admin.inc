<?php
// $Id$

/**
 *
 * @file
 * Administration module settings form definition
 */

function communication_tools_admin_form($form_state, $node) {

  $form['sent_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send to a Friend'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['share_tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('Share Tools'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['print_tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('Print Tools'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['print_tools'][COMMUN_TOOLS_PREFIX . 'show_print_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "Print" button'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'show_print_icon', ''),
    '#description' => t('Display "Print button".'),
    '#required' => FALSE
  );


  $site_logo_print_path = variable_get(COMMUN_TOOLS_PREFIX . 'site_logo_print');

  $form['print_tools'][COMMUN_TOOLS_PREFIX . 'site_logo_print'] = array(
    '#type'             => 'file',
    '#title'            => t('Site Logo For Print'),
    '#element_validate' => array('_communication_tools_element_validate'),
    '#file_name'        => 'site_logo_print',
  );

  if ($site_logo_print_path != '') {
    $form['print_tools']['site_logo_print_preview'] = array(
      '#type'   => 'item',
      '#markup' => t('Uploaded Image') . ': ' . theme('image',
                                                      array(
                                                         'path'  => $site_logo_print_path,
                                                         'alt'   => 'Site logo for printing',
                                                         'title' => 'Site logo for printing',
                                                      )
                                                ),
    );

    $form['print_tools']['site_logo_print_default_value'] = array(
      '#type'         => 'hidden',
      '#value'        =>  $site_logo_print_path,
    );

  }


  //This is necessary for browsers handle files.
  $form['#attributes'] = array('enctype' => "multipart/form-data");


  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'show_email_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "Send E-mail" button'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'show_email_icon', ''),
    '#description' => t('Display "E-mail to a Friend button".'),
    '#required' => FALSE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'email_block_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Send E-mail text link'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'email_block_link', ''),
    '#description' => t('The text of the link that will be displayed on the Block.'),
    '#required' => FALSE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'form_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Form Instructions'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'form_header', ''),
    '#description' => t('Instructions that will be shown in "Send to a friend" overlay '),
    '#required' => TRUE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'captcha_description_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Captcha text'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'captcha_description_text', ''),
    '#description' => t('Instructions that will be shown on "Captcha" challenge'),
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'send_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Send button text'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'send_button_text', ''),
    '#description' => t('Send button text on Send to a friend form'),
    '#required' => TRUE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'cancel_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Cancel button text'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'cancel_button_text', ''),
    '#description' => t('Cancel button text on Send to a friend form'),
    '#required' => TRUE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'privacy_policy_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Privacy policy path'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'privacy_policy_path', ''),
    '#description' => t('Specify the path of the Privacy Policy page'),
    '#required' => FALSE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'subject'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail Subject'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'subject', ''),
    '#description' => t('Subject of the e-mail that is sent to the friend'),
    '#required' => TRUE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail Body'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'email_body', ''),
    '#description' => t('Write the e-mail body that the users will receive. The "@@" '.
                        'variables will be replaced by the values of the fields filled in the form. <br/>
                        Where: @username@ = Friend name, @referer@ = Referer name, @link@ = Target page link, @footer@ = Site Url'),
    '#required' => TRUE
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'preloader_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Preloader Image Path'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'preloader_image'),
    '#description' => t('Specify the path for the Preloader Image from the Drupal File Public Path. Ex.: images/tools/tools_preloader.gif'),
  );
  
  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'success_page_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Success page path'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'success_page_path', 'send-to-a-friend/success-page'),
    '#description' => t('Specify the path of the success page. Module provides send-to-a-friend/success-page as default (you should set it explicitly).'),
    '#required' => TRUE
  );
  
  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'success_page_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Success page Body'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'success_page_body', 'Thank You! Your email has been sent.'),
    '#description' => t(''),
    '#required' => FALSE,
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'name_validation_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Name validation pattern'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'name_validation_pattern', ''),
    '#description' => t("Regex used to validate field names. If left empty, this validation won't be performed. Example: <em>/^[a-z]+(\s[a-z]+)?$/i</em>"),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $name_fields_description = t("Display this message when user enter characters that don't match the pattern defined on 'Name validation pattern' field");
  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'name_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Name field error message'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'name_error_message', ''),
    '#description' => $name_fields_description,
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'friend_name_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Friend Name field error message'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'friend_name_error_message', ''),
    '#description' => $name_fields_description,
    '#required' => FALSE,
    '#maxlength' => 120,
  );
  
  $form['sent_email'][COMMUN_TOOLS_PREFIX . 'show_content_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Content title on form.'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'show_content_title', ''),
    '#description' => t('Show the shared content title on the form to user.'),
    '#required' => FALSE
  );

  $form['share_tools'][COMMUN_TOOLS_PREFIX . 'show_facebook_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Facebook Fan Page button'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'show_facebook_icon', ''),
    '#description' => t('Whether or not to display the Facebook Fan Page button.'),
    '#required' => FALSE
  );

  $form['share_tools'][COMMUN_TOOLS_PREFIX . 'facebook_block_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook text link'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'facebook_block_link', ''),
    '#description' => t('The text of the link that will be displayed on the Block.'),
    '#required' => FALSE
  );

  $form['share_tools'][COMMUN_TOOLS_PREFIX . 'facebook_fan_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook fan url'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'facebook_fan_url', ''),
    '#description' => t('Especify the Facebook URL'),
    '#required' => FALSE
  );

  $form['share_tools'][COMMUN_TOOLS_PREFIX . 'show_addthis_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add AddThis button'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'show_addthis_icon', ''),
    '#description' => t('Display AddThis button.'.
                        ' The icon image and features are defined in the AddThis module settings.'),
    '#required' => FALSE
  );

  $form['#submit'][] = 'communication_tools_admin_config_form';

  return system_settings_form($form);
}

/**
 * Validator for file field.
 */
function _communication_tools_element_validate($element, &$form_state) {
  $form_key = implode('_', $element['#parents']);

  if (empty($_FILES['files']['name'][$form_key]) || form_get_error($element)) {
    return;
  }

  //Available extensions
  $extensions = array('jpg', 'png', 'jpeg');

  $dot = strrpos($_FILES['files']['name'][$form_key], '.');
  $extension = drupal_strtolower(drupal_substr($_FILES['files']['name'][$form_key], $dot+1));
  $file_error = FALSE;
  if (!in_array($extension, $extensions)) {
    form_error($element,
               t("Files with the '%ext' extension are not allowed, please upload a file with a %exts extension.",
                 array('%ext' => $extension, '%exts' => implode(',', $extensions))));
    $file_error = TRUE;
  }

  // Save the file to a temporary location.
  if (!$file_error) {
    $upload_dir = file_build_uri('images/communication_tools/logo/');
    if (file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY)) {
      // Note that the $extensions list was already validated above but needs to
      // be passed into file_save_upload() for it to pass internal validation
      // and not use the default extension checklist.

      $file = file_save_upload($form_key, array('file_validate_extensions' => array(implode(' ', $extensions))),
              $upload_dir, FILE_EXISTS_REPLACE);
      if ($file->filename != $element['#file_name'] .'.'. $extension) {
        $file = file_move($file, $upload_dir .'/'. $element['#file_name'] .'.'. $extension, FILE_EXISTS_REPLACE);
      }

      if ($file) {
        // Set the hidden field value.
        $parents = $element['#array_parents'];
        array_pop($parents);
        $parents[] = $form_key . '_fid';
        form_set_value(array('#parents' => $parents), $file->fid, $form_state);
      }
      else {
        drupal_set_message(
          t("The uploaded file %filename couldn't to be saved. The destination directory may not be writable.",
            array('%filename' => $file->filename)
          ),
          'error'
        );
      }
    }
    else {
      drupal_set_message(
        t("The uploaded file %filename couldn't to be saved. The destination directory does not exist."),
        'error'
      );
    }
  }

}

/**
 * Save files to the database.
 */
function communication_tools_admin_config_form($form, &$form_state) {
    $image_path = NULL;
    if (isset($form_state['values']['print_tools'][COMMUN_TOOLS_PREFIX . 'site_logo_print_fid'])) {
      $file = file_load($form_state['values']['print_tools'][COMMUN_TOOLS_PREFIX . 'site_logo_print_fid']);

      $file->filename = substr($file->uri, strrpos($file->uri, '/') + 1);
      $file->status = FILE_STATUS_PERMANENT;
      // Save any new files permanently.
      file_save($file);
      $image_path = $file->uri;
    }
    else if (isset($form_state['values']['site_logo_print_default_value'])){
      $image_path = $form_state['values']['site_logo_print_default_value'];
    }
    $form_state['values'][COMMUN_TOOLS_PREFIX . 'site_logo_print'] = $image_path;

}

function communication_tools_admin_logs_interface_form($form, &$form_state) {
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['filter'][COMMUN_TOOLS_PREFIX . 'filter_log_ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'filter_log_ip', ''),
    '#description' => t(''),
    '#required' => FALSE,
    '#maxlength' => 15,
  );
  
  $form['filter'][COMMUN_TOOLS_PREFIX . 'filter_log_from_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'filter_log_from_mail', ''),
    '#description' => t(''),
    '#required' => FALSE,
    '#maxlength' => 128,
  );
  
  $form['filter'][COMMUN_TOOLS_PREFIX . 'filter_log_to_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'filter_log_to_mail', ''),
    '#description' => t(''),
    '#required' => FALSE,
    '#maxlength' => 128,
  );
  
  $form['filter'][COMMUN_TOOLS_PREFIX . 'filter_log_submitted_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => variable_get(COMMUN_TOOLS_PREFIX . 'filter_log_submitted_date', ''),
    '#description' => t('Format: mm/dd/aaaa'),
    '#required' => FALSE,
    '#maxlength' => 10,
  );
  
  $form['filter'][COMMUN_TOOLS_PREFIX . 'submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  
  $table_markup = '';
  $filter_results = '';
  
  if (!empty($form_state['values']['log_data'])) {
    $filter_results = $form_state['values']['log_data'];
  } 

  if (!empty($form_state['values'])) {
    $table_markup = _communication_tools_admin_logs_interface_table_markup($filter_results);
  }
  
  $form[COMMUN_TOOLS_PREFIX . 'log_data'] = array(
    '#markup' => $table_markup,
  );
  
  return $form;
}

/**
 * Validates the Administrative form to retrieve the IP logs for the Send to a Friend functionality
 * @param $form The Drupal form structure
 * @param $form_state The state of the form after submitting
 */
function communication_tools_admin_logs_interface_form_validate($form, &$form_state) {
  $ip = $form_state['values']['communication_tools_filter_log_ip'];
  $from = $form_state['values']['communication_tools_filter_log_from_mail'];
  $to = $form_state['values']['communication_tools_filter_log_to_mail'];
  $submitted = $form_state['values']['communication_tools_filter_log_submitted_date'];
  
  //Check if at least one field has text
  if(empty($ip) && empty($from) && empty($to) && empty($submitted)) {
    form_error($form['filter'], 'Please enter value at least one field.');
  }
  
  //Check if the date has been entered correctly
    
  $valid = false; //$valid will become true if the date provided is valid i.e. has only numbers, slashes, hyphens
  $correct = false; //$correct will become true if the date is correct i.e. the day belongs to the month and the month is valid.
  
  $date = $form_state['values']['communication_tools_filter_log_submitted_date'];
  
  if(!empty($date)){
      $date_parts = explode('/',$form_state['values']['communication_tools_filter_log_submitted_date']);
      $valid = (strlen($date)==10) && strtotime($date);

      if($valid)
        $correct = checkdate($date_parts[0],$date_parts[1],$date_parts[2]);

      if(!$correct || !$valid){
          form_error($form['filter']['communication_tools_filter_log_submitted_date'], 'Please enter a correct date mm/dd/yyyy.');
      }
  }
  
}

function communication_tools_admin_logs_interface_form_submit($form, &$form_state) {
   $form_state['rebuild'] = TRUE;
   $form_state['values']['log_data'] = _communication_tools_admin_logs_interface_get_data($form_state['values']);
}

function _communication_tools_admin_logs_interface_get_data($data) {
  $ip = $data['communication_tools_filter_log_ip'];
  $from = $data['communication_tools_filter_log_from_mail'];
  $to = $data['communication_tools_filter_log_to_mail'];
  $submitted = $data['communication_tools_filter_log_submitted_date'];
  
  $where_optionals = array();
  $query_args = array();
  
  // filter by IP
  if (!empty($ip)) {
    $where_optionals[] = 'communication_tools_logs.ip LIKE :ip';
    $query_args[':ip'] = '%' . db_like($ip) . '%';
  }

  // filter by from mail
  if (!empty($from)) {
    $where_optionals[] = 'communication_tools_logs.from_mail LIKE :from_mail';
    $query_args[':from_mail'] = '%' . db_like($from) . '%';
  }
  
  // filter by to mail
  if (!empty($to)) {
    $where_optionals[] = 'communication_tools_logs.to_mail LIKE :to_mail';
    $query_args[':to_mail'] = '%' . db_like($to) . '%';
  }
  
  // filter by submitted date
  if (!empty($submitted)) {
    $submitted_timestamp = strtotime($submitted);
    $where_optionals[] = 'communication_tools_logs.submitted = :submitted';
    $query_args[':submitted'] = $submitted_timestamp;
  }
  
  $where_optionals = count($where_optionals) ? ' '. implode(' AND ', $where_optionals) : '';
  
  if (!empty($where_optionals)) {
    $where_optionals = 'WHERE '. $where_optionals;
  }
  
  $query = <<<QUERY
      SELECT * FROM {communication_tools_logs}
      {$where_optionals}
    ORDER BY communication_tools_logs.submitted DESC
QUERY;
  
  $result = db_query($query, $query_args)->fetchAll();
  
  return $result;
}

function _communication_tools_admin_logs_interface_table_markup($filter_results_markup) {
  
  $header = array(
    array('data' => t('IP')), 
    array('data' => t('FROM')),
    array('data' => t('TO')),
    array('data' => t('SUBMITTED DATE'))
  );
  
  $rows = _communication_tools_admin_logs_interface_table_markup_rows($filter_results_markup);

  $variables['header'] = $header; 
  $variables['rows'] = $rows;
  $variables['attributes'] = array();
  $variables['colgroups'] = array();
  $variables['caption'] = '';
  $variables['sticky'] = FALSE;
  $variables['empty'] = t('No results found.');
  $table_markup = theme_table($variables);

  return $table_markup;
}

function _communication_tools_admin_logs_interface_table_markup_rows($filter_results_markup) {
  $rows = array();
  
  if(!empty($filter_results_markup)) {
    foreach($filter_results_markup as $id => $data) {
      $rows[$id] = array(array('data' => $data->ip),
                        array('data' => $data->from_mail), 
                        array('data' => $data->to_mail),
                        array('data' => date('m/d/Y',$data->submitted))
                  );
    }
  }

  return $rows;
}