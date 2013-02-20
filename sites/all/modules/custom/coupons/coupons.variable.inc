<?php

function coupons_variable_info($options) {
  $variable['coupons_email_updates_url'] = array(
    'title' => t('Coupon sign up form page url'),
    'localize' => TRUE
  );
  
  $variable['coupons_email_updates_title'] = array(
    'title' => t('Coupon sign up form page title'),
    'localize' => TRUE
  );
  
  $variable['coupons_header_email_updates_page'] = array(
    'title' => t('Coupon sign up form header content'),
    'localize' => TRUE
  );
  
  $variable['coupons_bottom_email_updates_page'] = array(
    'title' => t('Coupon sign up form bottom content'),
    'localize' => TRUE
  );
  
  $variable['coupon_sign_up_form_id'] = array(
    'title' => t('Coupon sign up form ID'),
    'localize' => TRUE
  );
  
  $variable['coupons_email_updates_title'] = array(
    'title' => t('Coupon sign up form page title'),
    'localize' => TRUE
  );

  return $variable;
}