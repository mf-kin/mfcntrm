<?php
/**
* @file
* Coupon form definition
*/

/**
 * Sign up for email updates form page.
 */
function coupon_email_updates_form($coupon_node) {
  $sign_up_form = node_load(variable_get('coupon_sign_up_form_id'));

  if (empty($sign_up_form)) {
    watchdog('error', 'The Webform for Sign Up page is not set at CEMHUB module');
    drupal_not_found();
  }
  else {
    return drupal_get_form('webform_client_form_' . $sign_up_form->nid, $sign_up_form, array(), FALSE, FALSE, $coupon_node);
  }
}

/**
 * Redirect to thank you for user print the coupon.
 */
function coupons_submit_redirect($form, &$form_state) {
  module_load_include('inc', 'coupons', 'coupons.code');

  if (isset($form_state['values']['coupon_node'])) {
    $coupon_node = $form_state['values']['coupon_node'];
    if (isset($coupon_node) && !empty($coupon_node)) {
      $redirect_params = array(
        drupal_get_path_alias('node/' . $coupon_node->nid),
        array(
          'query' => array(
            'coupon_code' => coupons_encode_coupon_code($form_state['values']['submitted_tree']['field_coupon_timestamp']),
          ),
        ),
      );

      $form_state['redirect'] = $redirect_params;
    }
  }
}

/**
 * Fill form hidden fields with coupon node information.
 */
function _coupon_fill_hidden_form_fields($form, &$form_state) {
  $coupon_node = $form_state['values']['coupon_node'];
  
  if (!empty($coupon_node)) {
    $node_language = $coupon_node->language;
  
    // Populate Coupon Id field on form
    if (isset($coupon_node->field_coupon_id[$node_language][0]['value'])) {
      $form_state['values']['submitted']['field_coupon_id'] = $coupon_node->field_coupon_id[$node_language][0]['value'];
    }
  
    //Populate Coupon Value field
    if (isset($coupon_node->field_coupon_value[$node_language][0]['value'])) {
      $form_state['values']['submitted']['field_coupon_value'] = $coupon_node->field_coupon_value[$node_language][0]['value'];
    }
    
    //Populate Coupon Campaign ID
    $default   = isset($coupon_node->field_coupon_campaign_id[$node_language][0]['value'])?$coupon_node->field_coupon_campaign_id[$node_language][0]['value']:'';
    $overriden = isset($form_state['values']['field_coupon_campaign_id_overriden'])?$form_state['values']['field_coupon_campaign_id_overriden']:'';
    
    // If something has been passed to the form through the parameter, it will be on the $overriden variable
    // In that case, we need to use that value instead of the default one (which will be on the $default variable)
    if($overriden != '')
      $form_state['values']['submitted']['field_coupon_campaign_id'] = $overriden;
    else
      $form_state['values']['submitted']['field_coupon_campaign_id'] = $default;
  
    //Populate Coupon Print verification
    $form_state['values']['submitted']['field_coupon_print'] = PRINT_COUPON_DEFAULT_YES;
  }
}