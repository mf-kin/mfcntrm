<?php
// $Id$

/**
 *
 * @file
 * Send to a friend form definition
 */
function communication_tools_send_form($form, &$form_state) {
  $shared_path = communcation_tools_get_shared_path_details();
  
  if(variable_get(COMMUN_TOOLS_PREFIX . 'show_content_title', '')) {
    $form[COMMUN_TOOLS_PREFIX . 'content_title'] = array(
      '#markup' => $shared_path['title'],
    );
  }

  $privacy_link = l(t('Privacy Policy'),
    variable_get(COMMUN_TOOLS_PREFIX . 'privacy_policy_path'),
    array(
      'attributes' => array(
        'target' => '_blank',
        'title' => t('Privacy Policy')
      )
    )
  );
  $form_header_content = variable_get(COMMUN_TOOLS_PREFIX . 'form_header');
  $form['name'] = array(
    '#type' => 'fieldset',
    '#description' => strtr($form_header_content, array('!privacy_policy_link' => $privacy_link)),
  );

  $form[COMMUN_TOOLS_PREFIX . 'name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Name'),
    '#maxlength' => 30,
  );

  $form[COMMUN_TOOLS_PREFIX . 'email_yours'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Email'),
    '#required' => TRUE,
    '#element_validate' => array('communication_tools_form_email_validate')
  );

  $form[COMMUN_TOOLS_PREFIX . 'friends_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Friend's Name"),
    '#maxlength' => 30,
  );

  $form[COMMUN_TOOLS_PREFIX . 'email_friend'] = array(
    '#type' => 'textfield',
    '#title' => t("Friend's Email"),
    '#required' => TRUE,
    '#element_validate' => array('communication_tools_form_email_validate')
  );

  $form[COMMUN_TOOLS_PREFIX . 'submit_button'] = array(
    '#type' => 'submit',
    '#value' => variable_get(COMMUN_TOOLS_PREFIX . 'send_button_text'),
    '#attributes' => array(
      'title' => array(
        variable_get(COMMUN_TOOLS_PREFIX . 'send_button_text'),
        )
      ),
  );

  $is_https = _communication_tools_is_current_page_https();
  $form[COMMUN_TOOLS_PREFIX . 'cancel_button'] = array(
    '#markup' => '<iframe frameborder="0" src="' . url('send-to-a-friend/cancel-button', array('https' => $is_https, 'absolute' => TRUE)) . '" ></iframe>',
  );

  $form[COMMUN_TOOLS_PREFIX . 'article_title'] = array(
    '#type' => 'value',
    '#disabled' => TRUE,
    '#default_value' => $shared_path['title'],
    '#required' => TRUE
  );

  $form[COMMUN_TOOLS_PREFIX . 'article_path'] = array(
    '#type' => 'value',
    '#disabled' => TRUE,
    '#default_value' => $shared_path['url'],
    '#required' => TRUE
  );

  $form['#theme'] = COMMUN_TOOLS_PREFIX . 'form';

  //In case this become accessible for logged users this might need some fix.
  if(user_is_logged_in()) {
    $form['#token'] = FALSE;
  }

  if (function_exists('securepages_is_secure') && !securepages_is_secure()) {
    $form['#action'] = url($_GET['q'], array('https' => TRUE));
  }

  return $form;
}

function communcation_tools_get_shared_path_details() {
  module_load_include('inc', 'communication_tools', 'communication_tools.common');
  $path = communication_tools_get_shared_path();
  $parsed_url = drupal_parse_url($path);
  
  $url = _communication_tools_get_url($parsed_url);
  $title = communication_tools_get_page_title_from_path($parsed_url['path']);

  $shared_path = array(
    'url' => $url,
    'title' => $title,  
  );
  
  return $shared_path;
}


function _communication_tools_get_url($parsed_url) {  
  $options = array(
    'absolute' => TRUE,
    'query' => (!empty($parsed_url['query']) ? $parsed_url['query'] : NULL),        
  );

  $url = url($parsed_url['path'], $options);
  return $url;
}


function _communication_tools_is_current_page_https() {
  return (strpos($_SERVER['HTTP_REFERER'], 'https://') !== FALSE);
}

function communication_tools_build_email($form_state) {
  global $base_url;
  $title = $form_state['values'][COMMUN_TOOLS_PREFIX . 'article_title'];
  $path = $form_state['values'][COMMUN_TOOLS_PREFIX . 'article_path'];
  $friend_name = strip_tags($form_state['values'][COMMUN_TOOLS_PREFIX . 'friends_name']);
  $referer_name = strip_tags($form_state['values'][COMMUN_TOOLS_PREFIX . 'name']);

  $text = communication_tools_build_anchor_title($title);
  $link = l($text, url($path, array('https' => FALSE, 'absolute' => TRUE)));

  $body = variable_get(COMMUN_TOOLS_PREFIX . 'email_body');
  $body = str_replace('@username@', $friend_name, $body);
  $body = str_replace('@referer@', $referer_name, $body);
  $body = str_replace('@link@', $link, $body);
  $body = str_replace('@footer@', url($base_url, array('https' => FALSE, 'absolute' => TRUE)), $body);

  return $body;
}

function communication_tools_send_form_validate(&$elements, &$form_state, $form_id = NULL) {
  //exit(print_r($elements['communication_tools_name']));
  $name = trim($form_state['values']['communication_tools_name']);
  $friend_name = trim($form_state['values']['communication_tools_friends_name']);

  if (!empty($name) && !_check_valid_name($name)) {
    form_error($elements['communication_tools_name'], t(variable_get(COMMUN_TOOLS_PREFIX . 'name_error_message', '')));
  }

  if (!empty($friend_name) && !_check_valid_name($friend_name)) {
    form_error($elements['communication_tools_friends_name'], t(variable_get(COMMUN_TOOLS_PREFIX . 'friend_name_error_message', '')));
  }
}

function communication_tools_send_form_submit($form, &$form_state) {
  _communication_tools_send_email($form_state);
  _communication_tools_log_form_submission($form_state);

  $redirect_attributes = array(
    'absolute' => TRUE,
    'https'    => _communication_tools_is_current_page_https(),
  );

  $form_state['redirect'] = url(variable_get(COMMUN_TOOLS_PREFIX . 'success_page_path', 'send-to-a-friend/success-page'), $redirect_attributes);
}

function _communication_tools_log_form_submission($form_state) {
  $entry_to_insert = _communication_tools_get_log_entry_for_inserting($form_state);
  _communication_tools_insert_log_entry($entry_to_insert);
}

function _communication_tools_get_log_entry_for_inserting($form_state) {
  $from = $form_state['values'][COMMUN_TOOLS_PREFIX . 'email_yours'];
  $to = $form_state['values'][COMMUN_TOOLS_PREFIX . 'email_friend'];

  $entry = array(
    'ip' => _communication_tools_get_sender_ip(),
    'from_mail' => $from,
    'to_mail' => $to,
    'submitted' => mktime(0, 0, 0, date("m")  , date("d"), date("Y")),
  );

  return $entry;
}

function _communication_tools_get_sender_ip() {
  return $_SERVER['REMOTE_ADDR'];
}

function _communication_tools_insert_log_entry(array $entry) {
  $log_id = db_insert('communication_tools_logs')
            ->fields($entry)
            ->execute();

  return $log_id;
}

function _communication_tools_send_email($form_state) {
  $subject = variable_get('site_name');
  $body = communication_tools_build_email($form_state);
  $from = $form_state['values'][COMMUN_TOOLS_PREFIX . 'email_yours'];
  $to = $form_state['values'][COMMUN_TOOLS_PREFIX . 'email_friend'];
  $reply_to = $from;
  //The "pdweb@pfizer.com" is the general callback, just in case the website has no e-mail set on Drupal System > Site Information
  $system_mail = variable_get('site_mail','pdbweb@pfizer.com');

  $from_name = trim(mime_header_encode($form_state['values'][COMMUN_TOOLS_PREFIX . 'name'])
          . ' <' . $from . '>');
  //TODO put the fancy title here and partially include the template
  $my_module = COMMUN_TOOLS_PREFIX ;
  $my_mail_token = 'mailer';

  $message = array(
    'id' => $my_module . '_' . $my_mail_token,
    'to' => $to,
    'subject' => $subject,
    'body' => drupal_wrap_mail($body),
    'headers' => array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/html; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer'                  => 'Drupal',
      'From'                      => $from,
      'Reply-To'                  => $reply_to,
      'Return-Path'               => $system_mail,
    ),
  );
  
  $system = drupal_mail_system($my_module, $my_mail_token);
  $system->mail($message);
  

  $redirect_attributes = array(
    'absolute' => TRUE,
    'https'    => _communication_tools_is_current_page_https(),
  );

  $form_state['redirect'] = url(variable_get(COMMUN_TOOLS_PREFIX . 'success_page_path', 'send-to-a-friend/success-page'), $redirect_attributes);
}

function communication_tools_build_anchor_title($title) {
  $formated_anchor_text = "" ;
  if (strlen($title)) {
    $formated_anchor_text = variable_get('site_name') . variable_get(COMMUN_TOOLS_PREFIX . 'separator') . $title;
  }
  else {
    $formated_anchor_text = variable_get('site_name');
  }
  return $formated_anchor_text;
}

function communication_tools_get_page_title_from_path($path) {
  $title = '';

  $menu_item = menu_get_item($path);
  if (empty($menu_item['title'])) {
    $query = "SELECT link_title FROM {menu_links} WHERE link_path = :link_path AND menu_name = 'navigation'";
    $args = array(':link_path' => $path);
    $result = db_query($query, $args);
    
    $title = $result->fetchField(0);
  }
  else {
    $title = $menu_item['title'];
  }
  
  return $title;
}

function communication_tools_cancel_button() {
  $cancel_label = t('Cancel');
  $output = <<<HTML
    <a id="send-to-a-friend-cancel" title="{$cancel_label}" onclick="window.parent.parent.jQuery('a.ui-dialog-titlebar-close').click()">
      {$cancel_label}
    </a>
HTML;

  return $output;
}

function communication_tools_success_page() {
  $success_message = variable_get(COMMUN_TOOLS_PREFIX . 'success_page_body', 'Thank You! Your email has been sent.');
  $output = <<<HTML
    {$success_message}
HTML;

  return $output;
}

function communication_tools_delivery_raw_page($page_callback_result) {
  $page = array(
    '#theme_wrappers' => array('html'),
    'content' => array(
      '#markup' => $page_callback_result,
    ),
  );

  print drupal_render($page);
}

/**
 * White List to only allow characters A-Z and only allow 1 space
 * @param string $name
 * @return boolean
 */
function _check_valid_name($name) {
  $name_validation_pattern = variable_get(COMMUN_TOOLS_PREFIX . 'name_validation_pattern', '');

  $is_valid = TRUE;
  if (!empty($name_validation_pattern)) {
    $is_valid = preg_match($name_validation_pattern, $name);
  }

  return $is_valid;
}
