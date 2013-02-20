<?php
// $Id$

/**
 *
 * @file
 * Communication tools common functions
 */

/**
 * Strips the Send To a Friend form path from the current URL, returning only the path being shared
 * 
 * @return string
 */
function communication_tools_get_shared_path() {
  $args = arg();  
  $path = implode('/', $args);
  $pattern = '^' . COMMUNICATION_TOOLS_SEND_TO_A_FRIEND_FORM_PATH . '/?';
  $path = preg_replace("#{$pattern}#", '', $path);  
  
  return $path;
}