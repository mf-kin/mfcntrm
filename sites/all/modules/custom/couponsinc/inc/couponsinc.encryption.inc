<?php

/**
 * @file
 * Files functionality for encryption and link generation
 */

/**
 * Get all params and generate the CPT parameter for encryption
 *
 * @param string $pin_code  - JDoe@aol.com <identified string>
 * @param string $offer_code - 4 to 5 digits
 * @param string $short_key - 10 digits
 * @param string $long_key - 60 digits
 *
 * @return string $cpt
 */
function couponsinc_generate_cpt($pin_code, $offer_code, $short_key, $long_key) {
  $cpt = '';

  $all_parameters_are_filled = $pin_code && $offer_code && $short_key && $long_key;
  if ($all_parameters_are_filled){
    $decode_x = ' abcdefghijklmnopqrstuvwxyz0123456789!$%()*+,-.@;<=>?[]^_{|}~';
    $encode_modulo = array();
    $offer_code_array = array();
    $ocode = $offer_code;

    if (strlen($offer_code) > 4) {
      $ocode = $offer_code % 10000;
    }

    // setup of matrix 3 - starting from seconf half of offercode
    $offer_code_array[0] = substr($ocode, 2, 2);
    $offer_code_array[1] = substr($ocode, 0, 2);

    couponsinc_watchdog('offer_code_array', array($offer_code_array));

    // setup of matrix for translation of matrix 1
    for ($i=0; $i < 61; $i++) {
      $encode_modulo[substr($decode_x, $i, 1)] = $i;
    }
    couponsinc_watchdog('encode_module_after_translation', array($encode_modulo));
    $pin_code = strtolower($pin_code) . $offer_code;

    if (strlen($pin_code) < 20) {
      $pin_code = $pin_code . ' couponsincproduction';
      $pin_code = substr($pin_code, 0, 20);
    }
    couponsinc_watchdog('pin_code', array($pin_code));
    $q  = 0;
    $j = strlen($pin_code);
    $k = strlen($short_key);
    $s1 = '';
    $s2 = '';
    $s3 = '';

    // from here one it just perform the sum of those three arrays item per item.
    for ($i=0; $i<$j; $i++) {
      $s1 = $encode_modulo[substr($pin_code, $i, 1)];
      $s2 = 2 * $encode_modulo[substr($short_key, $i % $k, 1)];
      $s3 = $offer_code_array[$i % 2];
      $q = ($q + $s1 + $s2 + $s3) % 61;
      $cpt .= substr($long_key, $q, 1);
    }
    couponsinc_watchdog('cpt', array($cpt));
  }

  return $cpt;
}