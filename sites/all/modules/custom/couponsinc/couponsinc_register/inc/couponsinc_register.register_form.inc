<?php

/**
 * Callback function for CouponsInc Register page hook_menu item
 */
function couponsinc_register_register_form($coupon) {
  $form_node = node_load(variable_get('couponsinc_register_webform_nid'));

  $is_webform = (is_object($form_node) && $form_node->type == 'webform');
  if ($is_webform) {
    drupal_set_title($form_node->title);

    module_load_include('inc', 'couponsinc_register', 'inc/couponsinc_register.common');
    $register_form_id = couponsinc_register_get_register_webform_form_id();

    return drupal_get_form($register_form_id, $form_node, array(), FALSE, FALSE, $coupon);
  }
  else {
    watchdog('error', 'CouponsInc Register webform not found.');
    drupal_not_found();
  }
}