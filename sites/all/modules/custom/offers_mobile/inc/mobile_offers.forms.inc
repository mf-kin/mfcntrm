<?php
/**
 * @file
 * Mobile Offers form
 */

/**
 * Implementation of a form to be displayed in the Offers page
 */
function mobile_offers_form($form, &$form_state) {
  // Printing page title and form prefix
  $form['#prefix'] = variable_get(MOBILE_OFFERS_PREFIX . 'form_prefix', '');

  // Products field
  $product_node_type = variable_get(MOBILE_OFFERS_PREFIX . 'product_content_type');
  $product_nodes = get_nodes_from_node_type($product_node_type);
  $product_list = Array('' => variable_get(MOBILE_OFFERS_PREFIX . 'product_default_value'));

  $product_queue_qid = variable_get(MOBILE_OFFERS_PREFIX . 'product_queue', '');
  foreach ($product_nodes as $nid => $product) {
    $product_alias = variable_get(MOBILE_OFFERS_PREFIX . $product_node_type . '_' . $nid . '_alias');
    if (module_exists('nodequeue') && isset($product_queue_qid) && (!empty($product_queue_qid))){
      $position = nodequeue_get_subqueue_position($product_queue_qid,$nid);
      $product_list[$position] = !empty($product_alias) ? $product_alias : $product->title;
      ksort($product_list);
    } else {
      $product_list[$nid] = !empty($product_alias) ? $product_alias : $product->title;
    }
  }

  $form['product_value'] = array(
    '#type' => 'select',
    '#title' => variable_get(MOBILE_OFFERS_PREFIX . 'product_field_title', ''),
    '#options' => $product_list,
    '#required' => FALSE,
  );

  // Email field
  $form['email_value'] = array(
    '#type' => 'textfield',
    '#title' => variable_get(MOBILE_OFFERS_PREFIX . 'form_field_title', ''),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'email_field_default_value', ''),
    '#required' => FALSE
  );

  // Submit button
  $form['email_submit'] = array(
    '#type' => 'submit',
    '#value' => variable_get(MOBILE_OFFERS_PREFIX . 'submit_field_default_value', 'Submit'),
    '#required' => TRUE
  );
  
  // Printing form suffix
  $form['#suffix'] = variable_get(MOBILE_OFFERS_PREFIX . 'form_suffix', '');

  if(isset($form_state['storage']) && isset($form_state['storage']['alert_msg'])) {
    // Printing alert message if successful
    $form['alert_msg'] = array(
      '#markup' =>
        '<div class="thank-you-alert">' . variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_alert_content', '') .
          '<a class="thank-you-alert-button">' . variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_alert_button', '') . '</a>
        </div>',
    );
  }

  if(variable_get(MOBILE_OFFERS_PREFIX . 'email_field_default_value', '') != '') {
    drupal_add_js(array('mobileOffersEmailValue' => variable_get(MOBILE_OFFERS_PREFIX . 'email_field_default_value', '')), 'setting');
    drupal_add_js(drupal_get_path('module', 'mobile_offers') . '/scripts/mobile_offers.js');
  }
 
  return $form;
}

/**
 * Implementation of form validation 
 */
function mobile_offers_form_validate($form, &$form_state) {
  $email_invalid = variable_get(MOBILE_OFFERS_PREFIX . 'error_invalid_email', 'Please enter a valid email address.');
  $email_empty = variable_get(MOBILE_OFFERS_PREFIX . 'error_empty_email', 'Email field is required.');
  $product_empty = variable_get(MOBILE_OFFERS_PREFIX . 'error_empty_product', 'You must select a product.');
  
  if($form_state['values']['product_value'] == ''){
    form_set_error('product_value', t($product_empty));
  }
  
  if(trim($form_state['values']['email_value']) == '') {
    form_set_error('email_value', t($email_empty));
  }

  if(!preg_match('/^[^\W][a-zA-Z0-9_]+(\.[a-zA-Z0-9_]+)*\@[a-zA-Z0-9_]+(\.[a-zA-Z0-9_]+)*\.[a-zA-Z]{2,4}$/', $form_state['values']['email_value'])) {
    form_set_error('email_value', t($email_invalid));
  }
}

/**
 * Implementation of form submit
 */
function mobile_offers_form_submit($form, &$form_state) {

  $user_email = $form_state['values']['email_value'];

  if(mobile_offers_send_custom_mail($user_email, $form_state)) {
    $email_fields = array(
      'id' => NULL,
      'email_value' => $user_email,
      'created' => time()
    );

    $email_log = db_insert('mobile_offers_email')
                 ->fields($email_fields)
                 ->execute();

    if(variable_get(MOBILE_OFFERS_PREFIX . 'create_thank_you_page', '') == 1) {
      $redirect_attributes = array(
        'absolute' => TRUE,
        'https' => (strpos($_SERVER['HTTP_REFERER'], 'https://') !== FALSE),
      );

      $form_state['redirect'] = url('offers-thank-you', $redirect_attributes);
    }
    else {
      $form_state['rebuild'] = TRUE;
      $form_state['storage']['alert_msg'] = TRUE;
    }
  }
}

/**
 * Implementation of a function to send an email to the user
 */
function mobile_offers_send_custom_mail($user_email, $form_state) {
  $subject     =  variable_get(MOBILE_OFFERS_PREFIX . 'email_subject', variable_get('site_name', ''));
  $body        = variable_get(MOBILE_OFFERS_PREFIX . 'email_copy', '');
  if(variable_get(MOBILE_OFFERS_PREFIX . 'use_generic_coupon_url', '') == 0) {
    $products_content_type = variable_get(MOBILE_OFFERS_PREFIX . 'product_content_type');
    $body = str_replace('@form_url@', variable_get(MOBILE_OFFERS_PREFIX . 'register_page_full_site') . '/' . variable_get(MOBILE_OFFERS_PREFIX . $products_content_type . '_' . $form_state['values']['product_value'], ''), $body);
  }
  else {
    $body = str_replace('@form_url@', variable_get(MOBILE_OFFERS_PREFIX . 'register_page_full_site'), $body);
  }
  $from        = variable_get(MOBILE_OFFERS_PREFIX . 'default_mail_sender', 'noreply@pfizer.com');
  $to          = $user_email;
  $system_mail = variable_get('site_mail','noreply@pfizer.com');
  $from_name   = trim(mime_header_encode(variable_get(MOBILE_OFFERS_PREFIX . 'default_mail_sender_name', 'Noreply')) . ' <' . $from . '>');
  $reply_to    = variable_get(MOBILE_OFFERS_PREFIX . 'reply_to_email', '');

  if(empty($reply_to)) {
    $reply_to = $from;
  }

  $module_name   = 'mobile_offers';
  $my_mail_token = 'mailer';

  $message = array(
    'id' => $module_name . '_' . $my_mail_token,
    'to' => $to,
    'subject' => $subject,
    'body' => drupal_wrap_mail($body),
    'headers' => array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/html; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer'                  => 'Drupal',
      'From'                      => $from_name,
      'Reply-To'                  => $reply_to,
      'Return-Path'               => $system_mail,
    ),
  );

  $system = drupal_mail_system($module_name, $my_mail_token);
  $system->mail($message);

  return TRUE;
}