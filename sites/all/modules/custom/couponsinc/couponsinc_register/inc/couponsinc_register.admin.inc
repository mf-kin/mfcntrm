<?php

/**
 * Callback function for CouponsInc Register admin hook_menu item.
 *
 * @param type $form
 * @param type $form_state
 * @return type
 */
function couponsinc_register_admin_config_form($form, &$form_state) {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('CouponsInc Register configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['couponsinc_register_webform_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('CouponsInc Register webform nid'),
    '#description' => t('Enter your CouponsInc webform nid here'),
    '#default_value' => variable_get('couponsinc_register_webform_nid', ''),
    '#required' => TRUE,
  );


  $form['register_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('CouponsInc Register form page'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['register_page']['couponsinc_register_register_form_path'] = array(
    '#type'           => 'textfield',
    '#title'          => t('CouponsInc register form page URL'),
    '#description' => t('The path to assign to the hook_menu responsible for generating the register form (<strong>!coupon_wildcard</strong> hook_menu argument is required for the module to work properly)', array('coupon_wildcard' => COUPONSINC_COUPON_ID_WILDCARD)),
    '#default_value'  => variable_get('couponsinc_register_register_form_path', ''),
    '#element_validate' => array(
      'couponsinc_register_validate_form_path',
    ),
    '#required' => TRUE,
  );


  $form['general']['couponsinc_register_coupon_behavior'] = array(
    '#type' => 'radios',
    '#title' => t('Coupons behavior'),
    '#description' => t('Configure if the user will be automatically redirected to the coupons page or if '.
            'he will recieve and e-mail with the coupons link'),
    '#options' => array('email' => t('E-Mail'), 'redirect' => t('Redirect')),
    '#default_value' => variable_get('couponsinc_register_coupon_behavior', 'redirect'),
  );

  $form['register_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('CouponsInc Register E-Mail'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['register_email']['couponsinc_register_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail subject'),
    '#default_value' => variable_get('couponsinc_register_email_subject', ''),
  );

  $email_body_field = variable_get('couponsinc_register_email_body', array('value' => '', 'format' => 'full_html'));

  $form['register_email']['couponsinc_register_email_body'] = array(
    '#type' => 'text_format',
    '#format' => $email_body_field['format'],
    '#title' => t('E-Mail body'),
    '#description' => t('Available tokens: [coupon-url]'),
    '#default_value' => $email_body_field['value'],
  );

  return system_settings_form($form);
}

/**
 * Validation for admin settings form.
 *
 * @param type $element
 * @param type $form_state
 * @param type $form
 */
function couponsinc_register_validate_form_path($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    $search_pattern = '/' . COUPONSINC_COUPON_ID_WILDCARD . '\b/';
    $is_coupon_placeholder_included = preg_match($search_pattern, $element['#value']);
    if (!$is_coupon_placeholder_included) {
      form_error($element, t('!coupon_wildcard hook_menu placeholder is required on CouponsInc register form page URL field.', array('coupon_wildcard' => COUPONSINC_COUPON_ID_WILDCARD)));
    }

    $starts_with_slash = preg_match('#^/#', $element['#value']);
    $ends_with_slash = preg_match('#/$#', $element['#value']);
    if ($starts_with_slash || $ends_with_slash) {
      form_error($element, "Form page URL can't start or end with '/'");
    }
  }
}