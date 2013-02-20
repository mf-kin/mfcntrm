<?php

/**
 * Returns the hook_menu path for CouponsInc Register form.
 * @return type
 */
function couponsinc_register_get_register_form_path() {
  return variable_get('couponsinc_register_register_form_path', 'coupons-inc-register/' . COUPONSINC_COUPON_ID_WILDCARD);
}

/**
 * Returns CouponsInc Register webform form id.
 *
 * @return string
 */
function couponsinc_register_get_register_webform_form_id() {
  return 'webform_client_form_' . variable_get('couponsinc_register_webform_nid');
}

/**
 * Returns the CouponsInc register form path for a specific coupon.
 *
 * @param int $coupon_nid
 * @return string
 */
function couponsinc_register_get_register_form_path_by_coupon_nid($coupon_nid) {
  $register_form_path = couponsinc_register_get_register_form_path();
  $register_form_path = str_replace(COUPONSINC_COUPON_ID_WILDCARD, $coupon_nid, $register_form_path);
  $register_form_path = base_path() . $register_form_path;

  return $register_form_path;
}