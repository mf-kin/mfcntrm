<?php
/**
* @file
* Coupon image generation functions and iframe generation
*/

define('COUPONS_DEFAULT_IMAGE_FONT_SIZE', 20);
define('COUPONS_IMAGE_FONT_ANGLE', 0);

/*
 * Generates the image of the coupon with code and expiration date
 */
function coupons_generate_image($node, $coupon_code = NULL) { 
  module_load_include('inc', 'coupons', 'coupons.code');
  global $user;
  
  $coupon_code = coupons_decode_coupon_code($coupon_code);
  
  $is_admin = coupons_is_user_admin($user);  
  if (!$is_admin) {    
    coupons_redirect_to_coupon_form_on_invalid_coupon_code($coupon_code, $node);
  }
  
  // If output buffering is on: discard current content and disable further buffering
  if (ob_get_level()) {
    ob_end_clean();
  }
  
  $image = image_load($node->field_coupon_image[$node->language][0]['uri']);  
  image_gd_load($image);
    
  // get positions to print the code
  $code_x = $node->field_coupon_code_pos_x[$node->language][0]['value'];
  $code_y = $node->field_coupon_code_pos_y[$node->language][0]['value'];
  
  // get position to print the expiration date
  $date_x = $node->field_coupon_code_date_x[$node->language][0]['value'];
  $date_y = $node->field_coupon_code_date_y[$node->language][0]['value'];

  $expiration_days = $node->field_coupon_exp_time[$node->language][0]['value'];  
  $expiration_date = mktime(0, 0, 0, date('m'), date('d') + $expiration_days, date('Y'));
  
  $coupon_end_date = $node->field_coupon_end_date[$node->language][0]['value'];  
  $coupon_end_date = strtotime($coupon_end_date);
  
  if ($expiration_date > $coupon_end_date) {
    $expiration_date = $coupon_end_date;
  }
  
  $textcolor = imagecolorallocate($image->resource, 0, 0, 0);
  $font_path = drupal_get_path('module', 'coupons') . '/fonts/Adamina-Regular.ttf';

  $security_code_string = t('Security Code: ') . $coupon_code;
  imagettftext($image->resource, variable_get('coupon_image_font_size', COUPONS_DEFAULT_IMAGE_FONT_SIZE), COUPONS_IMAGE_FONT_ANGLE, $code_x, $code_y, $textcolor, $font_path, $security_code_string);
 
  $expiration_date_string = t('Exp. Date: ') . date('m/d/Y', $expiration_date);
  imagettftext($image->resource, variable_get('coupon_image_font_size', COUPONS_DEFAULT_IMAGE_FONT_SIZE), COUPONS_IMAGE_FONT_ANGLE, $date_x, $date_y, $textcolor, $font_path, $expiration_date_string);

  
  
  $rotate_factor = _coupons_get_rotate_factor(); 
  $rotated_image = imagerotate($image->resource, $rotate_factor, 0) ;
  
  drupal_add_http_header('Content-type', 'image/png');
  imagepng($rotated_image);
  imagedestroy($image->resource);
  
  exit();
}

function _coupons_get_rotate_factor() {
  $safari_version = _coupon_get_safari_version();
  $is_browser_safari = ($safari_version !== NULL);
  $orientation = variable_get('coupon_orientation', 'portrait');
  $rotate_factor = 0;
  if (($is_browser_safari && $safari_version < 5)  || $orientation == 'landscape') {
	$rotate_factor = -90;
  }
  
  return $rotate_factor;
}

function _coupon_get_safari_version() {
  $version = NULL;
  
  if (preg_match('#Version/([0-9]).*Safari/[0-9\.]+$#', $_SERVER['HTTP_USER_AGENT'], $matches)) {
    $version = $matches[1];
  }
  
  return $version;
}


function coupons_generate_coupon($node, $coupon_code = NULL) {
  module_load_include('inc', 'coupons', 'coupons.code');
  drupal_add_css(drupal_get_path('module', 'coupons') . '/styles/coupons.print.css', array('media' => 'print'));
  
  $coupon_code = coupons_decode_coupon_code($coupon_code);
  
  global $user;
  $is_admin = coupons_is_user_admin($user);  
  if (!$is_admin) {    
    coupons_redirect_to_coupon_form_on_invalid_coupon_code($coupon_code, $node);
  }
  
  $node->field_coupon_image['und'][0]['path'] = 'coupon-image/' . $node->nid . '/' . coupons_encode_coupon_code($coupon_code);
     
  $image_markup = theme('image', $node->field_coupon_image['und'][0]);
  
  $safari_version = _coupon_get_safari_version();
  $is_browser_safari = ($safari_version !== NULL);
  $orientation = variable_get('coupon_orientation', 'portrait');

  if (($is_browser_safari && $safari_version < 5) || $orientation == 'landscape') {
    $output = <<<HTML
    <div class="coupon-image-code landscape">      
      {$image_markup}
    </div>
HTML;
  }
  else {
   $output = <<<HTML
    <div class="coupon-image-code">      
      {$image_markup}
    </div>
HTML;
  }
  
  return $output;
}

function coupons_iframe_content($page_callback_result) {
  $page = array(
    '#theme_wrappers' => array('html'),
      'content' => array(
        '#markup' => $page_callback_result,
    ),
  );
  
  print drupal_render($page);
}
