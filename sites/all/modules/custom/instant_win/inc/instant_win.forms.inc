<?php

/**
 *
 * Success page for already registered instant win form.
 *
 * @param type $form
 * @param type $form_state
 * @return string
 */
function instant_win_already_registered_success_page($form, &$form_state) {
  $form = array(
    '#id' => 'already-registered-form',
  );

  $value = variable_get('instant_win_new_registration_code_success_page_content', array('value' => ''));

  $form['success_page'] = array(
    '#markup' => $value['value'],
  );

  $form['close'] = array(
    '#id' => 'already-registered-success-page-close-button',
    '#type' => 'submit',
    '#value' => t('Close'),
    '#ajax' => array(
      'callback' => 'instant_win_already_registered_form_close_ajax',
      'effect' => 'fade',
      'event' => 'click',
      'wrapper' => 'already-registered-form',
    ),
    '#attributes' => array(
      'class' => array('close-modal-button'),
      'title' => t('Close'),
    ),
  );

  return $form;
}

/**
 * Form builder. Instant Win Alredy Registered form.
 *
 * @ingroup forms
 */
function instant_win_already_registered_form($form, &$form_state) {
  $request_failed_message = variable_get('instant_win_server_request_error_message', '');
  drupal_add_js(array('instant_win' => array('requestFailedMessage' => $request_failed_message)), 'setting');
  
  $form = array(
    '#id' => 'already-registered-form',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'instant_win') . '/scripts/form_general.js',
      ),
    ),
  );

  $form['title'] = array(
    '#prefix' => '<h1>',
    '#suffix' => '</h1>',
    '#markup' => t('Already Registered?'),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#required' => TRUE,
    '#element_validate' => array(
      '_instant_win_field_email_already_registered_validation',
      '_instant_win_validate_register_limit_per_day',
    ),
    '#maxlength' => 60,
  );

  $form['registration_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Game Code'),
    '#required' => TRUE,
    '#element_validate' => array('_instant_win_field_code_already_registered_validation'),
    '#maxlength' => 60,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => array(
      'callback' => 'instant_win_already_registerd_form_ajax',
      'effect' => 'fade',
      'event' => 'click',
      'wrapper' => 'already-registered-form',
    ),
    '#submit' => array(
      'instant_win_already_registered_form_submit',
    ),
    '#attributes' => array(
      'title' => t('Submit'),
      'class' => array(
        'instant-win-already-registered-submit'
      ),
    ),
  );

  return $form;
}


/**
 *
 * This function adds javascript code to create recaptcha. Use only when the form is loaded using ajax.
 *
 * @param type $form
 */
function _instant_win_add_captcha_refresh(&$form) {
  if (isset($form['captcha']) && $form['captcha']['#captcha_type'] == 'recaptcha/reCAPTCHA') {
    $recaptcha_theme = variable_get('recaptcha_theme', 'red');
    $form['reload_recaptcha'] = array(
      '#markup' => "<script>setTimeout(\" \
          Recaptcha.create(RecaptchaState.site, \
          jQuery('#". $form['#id'] ." div.captcha > div').attr('id'), \
            { \
              theme: '$recaptcha_theme', \
              callback: Recaptcha.focus_response_field \
            } \
          ); \
        \", 100);</script>",
    );
  }
}

function instant_win_already_registerd_form_ajax($form, $form_state) {
  if ($form_state['storage']['success']) {
    $result = drupal_get_form('instant_win_already_registered_success_page');
  }
  else {
    _instant_win_add_captcha_refresh($form);
    $result = $form;
  }
  return $result;
}

function instant_win_already_registered_form_submit($form, &$form_state) {
  $form_state['storage']['success'] = TRUE;

  $transaction = $form_state['transaction'];

  try {
    $now_timestamp = time();
    $email = drupal_strtolower($form_state['values']['email']);

    $user = instant_win_get_user_by_email($email);

    if($user !== FALSE) {
      db_insert('instant_win_contest_entries')
        ->fields(array(
          'email' => $email,
          'contest_code' => $form_state['values']['registration_code'],
          'registered_at' => $now_timestamp,
          'processed_at' => 0,
          'iwuid' => $user->iwuid,
        ))
        ->execute();
    } else {
      throw new Exception('Try insert NULL user');
    }

    unset($form_state['transaction']);
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('instant_win', $e);
  }
}

function instant_win_already_registered_form_close_ajax($form, $form_state) {
  $form = drupal_get_form('instant_win_already_registered_form');
  _instant_win_add_captcha_refresh($form);
  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 * @return string
 */
function instant_win_get_code_form($form, &$form_state) {
  $request_failed_message = variable_get('instant_win_server_request_error_message', '');
  drupal_add_js(array('instant_win' => array('requestFailedMessage' => $request_failed_message)), 'setting');

  $form = array(
    '#id' => 'get-a-code-form',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'instant_win') . '/scripts/form_general.js',
      ),
    ),
  );

  $form['title'] = array(
    '#prefix' => '<h1>',
    '#suffix' => '</h1>',
    '#markup' => t('Need a game code?'),
  );

  $form['description'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('Enter your e-mail address below and a game code will be sent to you.'),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#required' => TRUE,
    '#maxlength' => 60,
    '#element_validate' => array(
      'instant_win_validate_email',
      'instant_win_get_a_alternate_code',
      '_instant_win_is_total_codes_taken_field_email_validation',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => array(
      'callback' => 'instant_win_get_code_form_ajax',
      'effect' => 'fade',
      'event' => 'click',
      'wrapper' => 'get-a-code-form',
    ),
    '#submit' => array('instant_win_get_code_form_submit'),
    '#attributes' => array(
      'title' => t('Submit'),
      'class' => array(
        'instant-win-get-a-code-submit'
      ),
    ),
  );

  return $form;
}

function instant_win_get_code_form_validate($form, &$form_state) {
  $form_state['no_cache'] = TRUE;
}

function _instant_win_is_total_codes_taken_field_email_validation($element, &$form_state, $form) {
  module_load_include('inc', 'instant_win', 'inc/instant_win.validators');

  if (!empty($element['#value']) && instant_win_is_total_codes_taken_by_an_user_exeeded($element, $form_state)) {
    form_error($element, variable_get('instant_win_requested_alternate_exceed_limit_message', ''));
  }
}

function instant_win_get_a_alternate_code($element, &$form_state) {
  try {
    module_load_include('inc', 'instant_win', 'inc/instant_win.common');
    instant_win_create_transaction_on_form_state($form_state);
    $contest_code = instant_win_get_available_alternate_code();

    if ($contest_code) {
      $form_state['contest_code'] = $contest_code;
    }
    else {
      form_error($element, variable_get('instant_win_no_available_alternate_code', ''));
    }
  }
 catch (Exception $e) {
   $form_state['transaction']->rollback();
   form_error($element, variable_get('instant_win_server_request_error_message', ''));
   watchdog_exception('instant_win', $e);
 }
}

function instant_win_validate_email($element, &$form_state) {
  module_load_include('inc', 'webform', 'components/email');
  _webform_validate_email($element, $form_state);
}

function instant_win_get_code_success_page($form, &$form_state) {
  $form = array(
    '#id' => 'get-a-code-form',
  );

  $value = variable_get('instant_win_get_alternate_code_success_page_content', array('value' => ''));

  $form['success_page'] = array(
    '#markup' => check_markup($value['value'], $value['format']),
  );

  $form['close'] = array(
    '#type' => 'submit',
    '#id'   => 'get-a-code-success-page-close-button',
    '#value' => t('Close'),
    '#ajax' => array(
      'callback' => 'instant_win_get_code_form_close_ajax',
      'effect' => 'fade',
      'event' => 'click',
      'wrapper' => 'get-a-code-form',
    ),
    '#attributes' => array(
      'class' => array('close-modal-button'),
      'title' => t('Close'),
    ),
  );

  return $form;
}

function instant_win_get_code_form_ajax($form, $form_state) {
  if ($form_state['storage']['success']) {
    $result = drupal_get_form('instant_win_get_code_success_page');
  }
  else {
    _instant_win_add_captcha_refresh($form);
    $result = $form;
  }
  return $result;
}

function instant_win_get_code_form_close_ajax($form, $form_state) {
  $form = drupal_get_form('instant_win_get_code_form');
  _instant_win_add_captcha_refresh($form);
  return $form;
}

function instant_win_get_code_form_submit($form, &$form_state) {
  $form_state['storage']['success'] = TRUE;

  $transaction = $form_state['transaction'];

  try {
    module_load_include('inc', 'instant_win', 'inc/instant_win.common');
    $contest_code = $form_state['contest_code'];

    if ($contest_code) {
      $request_time = time();
      $email = $form_state['values']['email'];

      db_insert('instant_win_alternate_codes')
        ->fields(
          array(
            'email' => $email,
            'contest_code' => $contest_code,
            'requested_at' => $request_time,
            'email_sent_at' => 0,
          ))
        ->execute();

      db_update('instant_win_contest_codes')
        ->fields(
          array(
            'sent_at' => $request_time,
          ))
        ->condition('code', $contest_code, '=')
        ->execute();
    }
    else {
      throw new Exception('Unable to get the alternate code from form_state');
    }

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('instant_win', $e);
  }

}


function instant_win_already_registered_form_validate($form, &$form_state) {
  module_load_include('inc', 'instant_win', 'inc/instant_win.validators');
  $form_state['no_cache'] = TRUE;
  $email = $form_state['values']['email'];

  $code = new stdClass();
  $code->contest_code = $form_state['values']['registration_code'];

  $query = _instant_win_get_query_for_all_unique_code_entries_from_an_user($email);
  $codes_entries = $query->execute()->fetchAll();

  $code_per_user_limit = variable_get('instant_win_number_of_codes_per_user', 10);

  $has_reached_code_limit = (count($codes_entries) >= $code_per_user_limit);
  $is_code_assigned_to_user = in_array($code, $codes_entries);

  if ($has_reached_code_limit && !$is_code_assigned_to_user) {
    form_set_error('', variable_get('instant_win_max_allowed_number_of_codes_per_user_error_message', ''));
  }
}