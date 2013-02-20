<?php
/**
 * @file
 * Implements actions to be fired on new system events
 */

/**
 * Return 403 action.
 */
function specialtriggers_return_403_action($object, $context) {
  // Print a 403 page.
  $return = '';
  drupal_add_http_header('Status', '403 Forbidden');
  watchdog('access denied', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);

  // Keep old path for reference, and to allow forms to redirect to it.
  if (!isset($_GET['destination'])) {
    $_GET['destination'] = $_GET['q'];
  }

  $path = drupal_get_normal_path(variable_get('site_403', ''));
  if ($path && $path != $_GET['q']) {
    // Custom 403 handler. Set the active item in case there are tabs to
    // display or other dependencies on the path.
    menu_set_active_item($path);
    $return = menu_execute_active_handler($path, FALSE);
  }

  if (empty($return)) {
    // Standard 403 handler.
    drupal_set_title(t('Access denied'));
    $return = t('You are not authorized to access this page.');
  }

  print drupal_render_page($return);
  drupal_exit();
}

/**
 * Return 404 action.
 */
function specialtriggers_return_404_action($object, $context) {
  // Print a 404 page.
  $return = '';
  drupal_add_http_header('Status', '404 Not Found');

  watchdog('page not found', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);

  // Check for and return a fast 404 page if configured.
  drupal_fast_404();

  // Keep old path for reference, and to allow forms to redirect to it.
  if (!isset($_GET['destination'])) {
    $_GET['destination'] = $_GET['q'];
  }

  $path = drupal_get_normal_path(variable_get('site_404', ''));
  if ($path && $path != $_GET['q']) {
    // Custom 404 handler. Set the active item in case there are tabs to
    // display, or other dependencies on the path.
    menu_set_active_item($path);
    $return = menu_execute_active_handler($path, FALSE);
  }

  if (empty($return)) {
    // Standard 404 handler.
    drupal_set_title(t('Page not found'));
    $return = t('The requested page "@path" could not be found.', array('@path' => request_uri()));
  }

  drupal_set_page_content($return);
  $page = element_info('page');
  print drupal_render_page($page);
  drupal_exit();
}
