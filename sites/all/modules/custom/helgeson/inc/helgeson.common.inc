<?php
// $Id$

/**
 * @file
 * Common functionality
 */

/**
 * Checks if required PHP extensions are loaded. If not, print warnings to the screen
 */
function _helgeson_warn_on_missing_required_php_extensions() {
  // Clear messages
  drupal_get_messages('warning');

  if (!extension_loaded(HELGESON_EXTENSION_SSH_NAME)) {
    drupal_set_message(t(HELGESON_MESSAGE_ERROR_EXTENSION_SSH), 'warning');
  }

  if (!extension_loaded(HELGESON_EXTENSION_GNUPG_NAME)) {
    drupal_set_message(t(HELGESON_MESSAGE_ERROR_EXTENSION_GNUPG), 'warning');
  }
}

/**
 * Returns whether or not the Helgeson integration is enabled
 *
 * @return bool
 */
function helgeson_is_integration_enabled() {
  return (bool)variable_get('helgeson_integration_enabled');
}

/**
 * Set last successful file transfer timestamp
 * 
 * @param string $timestamp
 */
function helgeson_set_last_successful_file_transfer_timestamp($timestamp) {
  variable_set('helgeson_last_successful_file_transfer_timestamp', $timestamp);
}

/**
 * Get last successful file transfer timestamp
 */
function helgeson_get_last_successful_file_transfer_timestamp() {
  return variable_get('helgeson_last_successful_file_transfer_timestamp', 0);
}

/**
 * Returns if the helgeson exportation should be performed or not
 * 
 * @return bool $should_run_exportation
 */
function helgeson_should_run_exportation() {
  $should_run_exportation = FALSE;
  
  if (helgeson_is_integration_enabled()) {
    $yesterday         = date('Ymd', strtotime("yesterday"));
    $last_cron_run_day = variable_get('helgeson_last_cron_run', $yesterday);
    $current_day       = date('Ymd');

    $is_in_scheduled_hour_range = (variable_get('helgeson_run_cron_hour', '2') == date('H', REQUEST_TIME));
    $hasnt_ran_today            = ($last_cron_run_day < $current_day);

    $should_run_exportation = ($is_in_scheduled_hour_range && $hasnt_ran_today);
  }
  else {
    watchdog('helgeson', 'The file was not generated because the variable "Integration enabled" is unset.');
  }
  
  return $should_run_exportation;
}