<?php

module_load_include('inc', 'system', 'system.mail');

/**
 * @file
 * Instant Win implementations of MailSystemInterface.
 */

/**
 * The default Drupal mail backend using PHP's mail function.
 */
class InstantWinMailSystem extends DefaultMailSystem {
  /**
   * Concatenate and wrap the e-mail body for plain-text mails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $message) {
    // Join the body array into one string.
    $message['body'] = implode("\n\n", $message['body']);

    $message['body'] = $this->format_urls($message['body']);
    
    // Wrap the mail body for sending.
    $message['body'] = drupal_wrap_mail($message['body']);
    return $message;
  }
  
  /**
   * Replace placeholders for full host name, so emails are sent with absolute urls.
   */
  private function format_urls($message) {
    $host_name = substr(url('<front>', array('absolute' => TRUE)), 0, -1);
    
    $message = str_replace('#host_name#', $host_name, $message);
  
    return $message;
  }
}


