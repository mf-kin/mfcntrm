<?php

/**
 * @file
 * Admin settings
 */

/**
 * Form builder. General settings form.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function couponsinc_general_settings_form() {
  module_load_include('inc', 'couponsinc', 'inc/couponsinc.utils');
  
  $form = array();
  
  $form['couponsinc_general_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#tree'        => TRUE,
  );
  
  $form['couponsinc_general_settings']['debug'] = array(
    '#type'          => 'checkbox',
    '#prefix'        => t('<strong>Is debug enabled?</strong>'),
    '#title'         => t('check this field if you want to watchdog this module'),
    '#default_value' => couponsinc_is_debug_enabled(),
  );
  
  $form['couponsinc_general_settings']['base_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Address to Generate Coupon'),
    '#description'   => t('A client can print a coupon using Coupons, Inc. technology by making a call to this link with parameters.'),
    '#default_value' => couponsinc_get_url(),
  );
  
  return system_settings_form($form);
}