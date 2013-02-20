<?php
/**
 * @file
 * CouponsInc Register email functions
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
function couponsinc_register_get_mail_definition($key, $params) {
  switch ($key) {
    case COUPONSINC_REGISTER_EMAIL:
      $email_definition = couponsinc_register_get_register_mail($params);
      break;
    default:
      $email_definition = array();
  }

  return $email_definition;
}

/**
 * Return the email definition of coupon link email that is sent to the after registering
 * to get a coupon
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
function couponsinc_register_get_register_mail($params) {
  $message = array();

  $message['subject'] = variable_get('couponsinc_register_email_subject', '');

  $email_body = variable_get('couponsinc_register_email_body', '');
  $message['body'] = check_markup($email_body['value'], $email_body['format']);

  $coupon_url = couponsinc_generate_link($params['offer_code'], $params['check_code'], $params['short_key'], $params['long_key'], $params['pin_code']);
  $message['body'] = str_replace('[coupon-url]', $coupon_url, $message['body']);
  
  $message['body'] = explode('\n', $message['body']);

  return $message;
}

