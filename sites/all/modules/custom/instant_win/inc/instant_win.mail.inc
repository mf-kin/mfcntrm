<?php
/**
 * @file
 * Instant Win email functions
 */
 
/**
 * Return mail definition based on $key. Act as a gateway of each mail definition of the module.
 *
 * @param string $key
 *  Indentification of the wanted email.
 *
 * @param array $params
 *  Parameters for the function which implements the wanted email.
 *
 * @return array
 *  Array with the final email body and subject in the form of:
 *  array(
 *    'subject' => string,
 *    'body'    => array('line1', 'line2', ..., 'lineN'),
 *  );
 */
function _instant_win_get_mail_definition($key, $params) {
  switch ($key) {
    case 'instant_win_get_a_code_mail':
      $email_definition = instant_win_get_a_code_mail($params['code']);
      break;
    case 'instant_win_prize_mail':
      $email_definition = instant_win_get_prize_mail($params['node']);
      break;
    case 'instant_win_try_again_mail':
    default:
      $email_definition = instant_win_get_try_again_mail($params);
      break;
  }
  
  return $email_definition;
}

/**
 * Return the email definition of the try again email that is sent to the user when he doesn't win
 * the competition.
 * 
 * @param array $params
 *  Parameters for the function.
 * 
 * @return array
 *  Array with the final email body and subject in the form of:
 *  array(
 *    'subject' => string,
 *    'body'    => array('line1', 'line2', ..., 'lineN'),
 *  );
 */
function instant_win_get_try_again_mail() {  
  $body = variable_get('instant_win_notification_try_again_email_body', '');  
  $message = check_markup($body['value'], $body['format']);
  
  return array(
    'subject' => variable_get('instant_win_notification_try_again_email_subject', ''),
    'body' => $message,
  );  
}

/**
 * Return the email definition of the prize email when user wins
 * 
 * @param int|stdClass $prize_node
 * 
 * @return array
 *  Array with the final email body and subject in the form of:
 *  array(
 *    'subject' => string,
 *    'body'    => array('line1', 'line2', ..., 'lineN'),
 *  );
 */
function instant_win_get_prize_mail($prize_node) {
  if (is_numeric($prize_node)) {
    $prize_node = node_load((int)$prize_node);
  }
  
  $prize_email_definition = NULL;
  
  $is_prize_node = is_object($prize_node) && isset($prize_node->type) && ($prize_node->type == 'prize');
  if ($is_prize_node) {
    if (!empty($prize_node->field_prize_email_subject[$prize_node->language])) {
      $prize_email_definition['subject'] = $prize_node->field_prize_email_subject[$prize_node->language][0]['value'];
    }
    
    if (!empty($prize_node->field_prize_email_body[$prize_node->language])) {
      $prize_email_definition['body'] = $prize_node->field_prize_email_body[$prize_node->language][0]['value'];
    }
  }
  
  return $prize_email_definition;
}

/**
 * Return the email definition of the requested alternate code.
 * 
 * @param array $code
 *  Contest Code.
 * 
 * @return array
 *  Array with the final email body and subject in the form of:
 *  array(
 *    'subject' => string,
 *    'body'    => array('line1', 'line2', ..., 'lineN'),
 *  );
 */
function instant_win_get_a_code_mail($code) {
  $default_value = array(
    'value'  => '', 
    'format' => ''
  );

  $body = variable_get('instant_win_notification_get_a_code_email_body', $default_value);

  $body['value'] = check_markup($body['value'], $body['format']);

  $email_definition = array(
    'subject' => variable_get('instant_win_notification_get_a_code_email_subject', ''),
    'body'    => $body['value'],
  );

  $email_definition['body'] = str_replace('#code#', $code, $email_definition['body']);
  
  return $email_definition;
}



