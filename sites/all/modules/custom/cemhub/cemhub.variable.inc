<?php
// $Id$

/**
 * @file
 * Implemets translatable variables info
 */

/**
 * Implements variable_info
 */
function cemhub_variable_info($options) {
  $variable['cemhub_default_template_title'] = array(
    'title' => t('Sign Up Title'),
    'description' => t('The title of the Sign Up form page'),
    'localize' => TRUE
  );
  
  $variable['cemhub_default_template_description'] = array(
    'title' => t('Sign Up Description'),
    'description' => t('The description of the Sign Up form page (shown below the title)'),
    'localize' => TRUE
  );

  return $variable;
}