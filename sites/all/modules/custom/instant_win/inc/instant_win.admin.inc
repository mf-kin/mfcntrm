<?php
/**
 * @file
 * Instant Win admin settings
 */

/**
 * Form builder. Instant Win general settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function instant_win_admin_general_settings_form() {
  $form = array();

  $form['register_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register Configurations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['register_config']['instant_win_register_limit_per_day'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit of registrations per day'),
    '#default_value' => variable_get('instant_win_register_limit_per_day', ''),
    '#description' => t('The number of register allowed by one user per day.'),
    '#required' => FALSE,
  );

  $form['register_config']['instant_win_register_minimum_available_codes'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum available codes.'),
    '#default_value' => variable_get('instant_win_register_minimum_available_codes', ''),
    '#description' => t("Minimum number of available codes before it's considered critically low."),
    '#required' => FALSE,
  );

  $form['register_config']['instant_win_register_limit_user_get_a_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit of alternate codes request per user.'),
    '#default_value' => variable_get('instant_win_register_limit_user_get_a_code', ''),
    '#description' => t('The number of alternate codes that a user may request during the promotion.'),
    '#required' => FALSE,
  );

  $form['register_config']['instant_win_get_a_code_email_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay of Get a Code email'),
    '#default_value' => variable_get('instant_win_get_a_code_email_delay', 0),
    '#description' => t('Delay in hours of get a code email.'),
    '#required' => FALSE,
  );

  $form['register_config']['instant_win_number_of_users_per_process'] = array(
    '#type' => 'textfield',
    '#title' => t('The number of users to be processed and notified.'),
    '#default_value' => variable_get('instant_win_number_of_users_per_process', '100'),
    '#description' => t('The number of users to be processed and notified once.'),
    '#required' => FALSE,
  );
  
  $form['register_config']['instant_win_number_of_codes_per_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Max codes per user'),
    '#default_value' => variable_get('instant_win_number_of_codes_per_user', '10'),
    '#description' => t('Max allowed number of codes each user can have during the whole contest.'),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * Form builder. Instant Win validation settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function instant_win_admin_validation_settings_form() {
  $form = array();

  $form['validation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Validation rules'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['validation']['instant_win_first_name_validation_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name validation pattern'),
    '#default_value' => variable_get('instant_win_first_name_validation_pattern', ''),
    '#description' => t("Regex used to validate <strong>First Name</strong> field. If left empty, this validation won't be performed. Example: <em>/^[a-z]+$/i</em>"),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation']['instant_win_last_name_validation_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name validation pattern'),
    '#default_value' => variable_get('instant_win_last_name_validation_pattern', ''),
    '#description' => t("Regex used to validate <strong>Last Name</strong> field. If left empty, this validation won't be performed. Example: <em>/^[a-z]+$/i</em>"),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation']['instant_win_zipcode_validation_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode validation pattern'),
    '#default_value' => variable_get('instant_win_zipcode_validation_pattern', ''),
    '#description' => t("Regex used to validate <strong>Zip/Postal Code</strong> field. If left empty, this validation won't be performed. Example: <em>/^[0-9]{5}$/</em>"),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation']['instant_win_phone_validation_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number validation pattern'),
    '#default_value' => variable_get('instant_win_phone_validation_pattern', ''),
    '#description' => t("Regex used to validate <strong>Telephone</strong> field. If left empty, this validation won't be performed. Example: <em>/^[0-9]{9,10}$/</em>"),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation']['instant_win_dob_min_age_to_register'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum age to register'),
    '#default_value' => variable_get('instant_win_dob_min_age_to_register', '18'),
    '#description' => t("The minimum age required for the user to be allowed to register."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Validation messages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['validation_messages']['instant_win_register_form_first_name_format_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name format'),
    '#default_value' => variable_get('instant_win_register_form_first_name_format_error_message', ''),
    '#description' => t("Message displayed when a invalid <strong>First Name</strong> format is provided by the user (only triggered if <strong>First Name validation pattern</strong> field is filled."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_last_name_format_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name format'),
    '#default_value' => variable_get('instant_win_register_form_last_name_format_error_message', ''),
    '#description' => t("Message displayed when a invalid <strong>Last Name</strong> format is provided by the user (only triggered if <strong>Last Name validation pattern</strong> field is filled."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_zipcode_format_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode format'),
    '#default_value' => variable_get('instant_win_register_form_zipcode_format_error_message', ''),
    '#description' => t("Message displayed when a invalid <strong>Zip/Postal Code</strong> format is provided by the user (only triggered if <strong>Zipcode validation pattern</strong> field is filled."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_phone_format_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number format'),
    '#default_value' => variable_get('instant_win_register_form_phone_format_error_message', ''),
    '#description' => t("Message displayed when a invalid <strong>Telephone</strong> format is provided by the user (only triggered if <strong>Phone Number validation pattern</strong> field is filled."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_dob_min_age_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum user age'),
    '#default_value' => variable_get('instant_win_register_form_dob_min_age_error_message', ''),
    '#description' => t('Message displayed when user is not older than the minimum allowed age.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_code_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Game Code'),
    '#default_value' => variable_get('instant_win_register_form_code_error_message',
            'Register Code is not valid or has already been used by other user.'),
    '#description' => t('Message displayed when a user is trying to register using a already used code.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_already_registered_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Already Registered Message'),
    '#default_value' => variable_get('instant_win_register_form_already_registered_message', ''),
    '#description' => t('Message displayed when user is already registered and try to register again.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_not_already_registered_message'] = array(
    '#type' => 'textfield',
    '#title' => t('NOT Already Registered Message'),
    '#default_value' => variable_get('instant_win_register_form_not_already_registered_message', ''),
    '#description' => t('Message displayed when user is not registered, and try to use "Already Registered Form".'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_register_form_exceed_limit_per_day_message'] = array(
    '#type' => 'textfield',
    '#title' => t('User exceeded the registrations limit per day'),
    '#default_value' => variable_get('instant_win_register_form_exceed_limit_per_day_message', ''),
    '#description' => t('Message displayed when user exceeded the registrations limit per day.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_no_available_alternate_code'] = array(
    '#type' => 'textfield',
    '#title' => t('No available alternate code on database'),
    '#default_value' => variable_get('instant_win_no_available_alternate_code', ''),
    '#description' => t('Message displayed when there is no available alternate code on the database.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_requested_alternate_exceed_limit_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Requested alternate codes exceeded limit'),
    '#default_value' => variable_get('instant_win_requested_alternate_exceed_limit_message', ''),
    '#description' => t('Message displayed when user exceeded the number of alternate codes.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $form['validation_messages']['instant_win_already_register_same_day_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Already used code'),
    '#default_value' => variable_get('instant_win_already_register_same_day_error_message',
            'The game code you have entered was previously used today. Please enter a different game code.'),
    '#description' => t('Message displayed when user tries to use the same code more than once in a day.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );
  
  $form['validation_messages']['instant_win_max_allowed_number_of_codes_per_user_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Max allowed number of codes per user reached'),
    '#default_value' => variable_get('instant_win_max_allowed_number_of_codes_per_user_error_message',
            'Sorry, you have exceeded the maximum number of game codes.'),
    '#description' => t('Message displayed when user tries to use more codes than allowed during the whole contest.'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );
  
  $form['validation_messages']['instant_win_server_request_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Server request error'),
    '#default_value' => variable_get('instant_win_server_request_error_message',
            'Sorry, we were unable to solve your request. Please, try again later.'),
    '#description' => t('Message displayed when server were unable to solve a request (e.g., database lock timeout).'),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  return system_settings_form($form);
}

/**
 * Form builder. Instant Win contents settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function instant_win_admin_contents_settings_form() {
  $form = array();

  $form['page-contents'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Contents'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $default_value = array(
    'value'  => '',
    'format' => ''
  );

  $form['page-contents']['instant_win_new_registration_form_display_success_as_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Main Form Success message as overlay'),
    '#default_value' => variable_get('instant_win_new_registration_form_display_success_as_overlay', FALSE),
    '#description' => t('Wheter the content of "Registration Main Form Success Page content" will be displayed as an overlay or be redirected to a new page.'),
  );

  $field_values = variable_get('instant_win_new_registration_form_success_page_content', $default_value);
  $form['page-contents']['instant_win_new_registration_form_success_page_content'] = array(
    '#type' => 'text_format',
    '#title' => t('Registration Main Form Success Page content'),
    '#default_value' => $field_values['value'],
    '#description' => t("The content of the page displayed after a successful registration."),
    '#required' => FALSE,
    '#format' => $field_values['format'],
  );

  $field_values = variable_get('instant_win_new_registration_code_success_page_content', $default_value);
  $form['page-contents']['instant_win_new_registration_code_success_page_content'] = array(
    '#type' => 'text_format',
    '#title' => t('New Game Code Success Page content'),
    '#default_value' => $field_values['value'],
    '#description' => t("The content of the page displayed after a successful new game code request."),
    '#required' => FALSE,
    '#format' => $field_values['format'],
  );

  $field_values = variable_get('instant_win_get_alternate_code_success_page_content', $default_value);
  $form['page-contents']['instant_win_get_alternate_code_success_page_content'] = array(
    '#type' => 'text_format',
    '#title' => t('Get Alternate Code Success Page content'),
    '#default_value' => $field_values['value'],
    '#description' => t("The content of the page displayed after a successful get alternate code request."),
    '#required' => FALSE,
    '#format' => $field_values['format'],
  );

  return system_settings_form($form);
}

/**
 * Form builder. Instant Win notification settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function instant_win_admin_notification_settings_form() {
  $default_textarea_value = array(
    'value'  => '',
    'format' => ''
  );

  $form = array();

  $form['notification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['notification']['instant_win_notification_try_again_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Try again email subject'),
    '#default_value' => variable_get('instant_win_notification_try_again_email_subject', ''),
    '#description' => t("The subject of the email sent to the user when he or she hasn't won the competition."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $field_try_again_email_body = variable_get('instant_win_notification_try_again_email_body', $default_textarea_value);
  $form['notification']['instant_win_notification_try_again_email_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Try Again email body'),
    '#default_value' => $field_try_again_email_body['value'],
    '#description' => t("The body of the email sent to the user when he or she hasn't won the competition."),
    '#required' => FALSE,
    '#format' => $field_try_again_email_body['format'],
  );


  $form['notification']['instant_win_notification_get_a_code_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Get a Code email subject'),
    '#default_value' => variable_get('instant_win_notification_get_a_code_email_subject', ''),
    '#description' => t("The subject of the email sent to the user when he or she requests a new game code."),
    '#required' => FALSE,
    '#maxlength' => 120,
  );

  $field_get_a_code_email_body = variable_get('instant_win_notification_get_a_code_email_body', $default_textarea_value);
  $form['notification']['instant_win_notification_get_a_code_email_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Get a Code email body'),
    '#default_value' => $field_get_a_code_email_body['value'],
    '#description' => t("The body of the email sent to the user when he or she requests a new game code. Available placeholders: <strong>#code#</strong>"),
    '#required' => FALSE,
    '#format' => $field_get_a_code_email_body['format'],
  );

  return system_settings_form($form);
}


function instant_win_prize_timestamp_form() {
  drupal_add_tabledrag('timestamp-table', 'order', 'sibling', 'timestamp-weight');

  $header = array(
    'delete' => t('Delete'),
    'prize' => t('Prize'),
    'timestamp' => t('Timestamp'),
    'timestamp_text' => t('Timestamp Text Format'),
  );

  $form['timestamp_table'] = array(
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => _instant_win_get_form_table_rows(),
    '#theme' => 'table',
    '#attributes' => array('id' => 'patterns-table'),
    '#empty'  => t('No timestamp available.'),
  );

  $form['#submit'][] = 'instant_win_prize_timestamp_form_submit';

  return system_settings_form($form);
}



function _instant_win_get_form_table_rows() {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');

  $timestamps = instant_win_get_all_prize_timestamps();

  $rows = array();
  foreach ($timestamps as $timestamp) {
    $rows[] = _instant_win_get_table_gateway($timestamp);
  }

  return $rows;
}


function _instant_win_get_table_gateway($timestamp) {
  $row = _instant_win_get_filled_row($timestamp);
  $row = _instant_win_get_rendered_row_fields($row);

  $row_definition = array(
    'data' => $row,
    'class' => array('draggable'),
  );

  return $row_definition;
}

function _instant_win_get_rendered_row_fields($row) {
  foreach($row as $field => $row_data) {
    drupal_render($row_data);
    $row[$field] = $row_data['#children'];
  }

  return $row;
}


function _instant_win_get_filled_row($timestamp = NULL) {
  $row = array(
    'delete' => array(
      '#name' => 'delete' . $timestamp->tid,
      '#type' => 'checkbox',
      '#checked' => FALSE,
    ),
    'prize' => array(
      '#markup' => $timestamp->title,
    ),
    'timestamp' => array(
      '#markup' => $timestamp->timestamp,
    ),
    'timestamp_text' => array(
      '#markup' => date('Y-m-d G:i:s', $timestamp->timestamp),
    ),
  );

  return $row;
}

function instant_win_prize_timestamp_form_submit($form, &$form_state) {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');

  $timestamps = instant_win_get_all_prize_timestamps();

  $success = TRUE;

  foreach ($timestamps as $timestamp) {
    if (isset($form_state['input']['delete' . $timestamp->tid])) {
      $result = instant_win_delete_prize_timestamp($timestamp->tid);

      if (!$result) {
        $success = FALSE;
      }
    }
  }

  if (!$success) {
    drupal_set_message('An error occured. Maybe, some timestamps can\'t be deleted', 'error');
  }

}

/**
 * Generate Registered Users per day report form.
 */
function instant_win_total_registered_users_report_form() {
  return instant_win_report_form('registered_users');
}

/**
 * Generate Total Winners per day report form.
 */
function instant_win_total_winners_report_form() {
  return instant_win_report_form('winners');
}

/**
 * Gateway function to generate a report form with a structure as (formatted string)day => (integer)total.
 *
 * @param string $report_form
 *  Form identifier
 *
 * @return array
 *  Renderable form array for the specified $report_form.
 */
function instant_win_report_form($report_form) {
  $form = array();

  $header = array(
    'day' => t('Day'),
    'total' => t('Total per day'),
  );

  $form['winners_report_table'] = array(
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => _instant_win_report_form_table_rows($report_form),
    '#theme' => 'table',
    '#attributes' => array('id' => 'users-report-table'),
    '#empty'  => t('No winners available.'),
  );

  return $form;
}

/**
 * Gateway function to generate a report form rows for table structured form.
 *
 * @param string $report_form
 *  Form identifier
 *
 * @return array
 *  Renderable rows array for the specified $report_form.
 */
function _instant_win_report_form_table_rows($report_form) {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');

  if ($report_form == 'winners') {
    $users_per_day = instant_win_get_all_winners_per_day();
  }
  else if ($report_form == 'registered_users') {
    $users_per_day = instant_win_get_all_registered_users_per_day();
  }

  $total_users = 0;
  $rows = array();
  foreach ($users_per_day as $users) {
    $rows[] = _instant_win_get_report_table_row_definition($users);
    $total_users += $users->total;
  }

  $rows[] = _instant_win_get_table_total_row($total_users);

  return $rows;
}

/**
 * Return rendered row definition for table themed form.
 *
 * @param stdClass $users
 *  Users definition for a specific day. It must have 'user_registered_at' with a timestamp
 *  and 'total' keys with an integer.
 *
 * @return array
 *  Rendered row definition.
 */
function _instant_win_get_report_table_row_definition($users) {
  $row = _instant_win_get_report_filled_row($users);

  $row_definition = array(
    'data' => $row,
  );

  return $row_definition;
}

/**
 * Fill row with renderable array.
 *
 * @param stdClass $users
 *  Users definition for a specific day. It must have 'user_registered_at' with a timestamp
 *  and 'total' keys with an integer.
 *
 * @return array
 *  Filled renderable array row.
 */
function _instant_win_get_report_filled_row($users) {
  $row = array(
    'day' => date('Y-m-d', $users->user_registered_at),
    'total' => $users->total,
  );

  return $row;
}

/**
 * Return total
 *
 * @param integer $total_users
 *  Total users.
 *
 * @return array
 *  Last total table row.
 */
function  _instant_win_get_table_total_row($total_users) {
  return array(
    'data' => array(
      'day' => '<strong>TOTAL</strong>',
      'total' => $total_users,
    ),
  );
}


function instant_win_available_codes_status_ws() {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');

  $available_codes = instant_win_get_count_available_codes();
  $minimum_available_codes = variable_get('instant_win_register_minimum_available_codes', 0);

  $status = INSTANT_WIN_COUNT_AVAILABLE_CODES_OK;
  if ($available_codes < $minimum_available_codes) {
    $status = INSTANT_WIN_COUNT_AVAILABLE_CODES_NOK;
  }

  $available_codes_status = array(
    'total'  => $available_codes,
    'status' => $status,
  );

  drupal_add_http_header('Content-type', 'application/json');
  print drupal_json_encode($available_codes_status);
}