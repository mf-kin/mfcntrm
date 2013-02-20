<?php

function addthis_variable_info($options) {
  $variable['addthis_image'] = array(
    'title' => t('Addthis Button image url'),
    'description' => t('URL for small image in the button. Example: http://s9.addthis.com/button1-share.gif'),
    'localize' => TRUE
  );
  
  $variable['addthis_image_secure'] = array(
    'title' => t('Addthis Button image url for secure https connection'),
    'description' => t('This is only required if your site uses secure https connections. Example: https://secure.addthis.com/button1-share.gif'),
    'localize' => TRUE
  );

  $variable['addthis_image_width'] = array(
    'title' => t('Addthis Button image width'),
    'description' => t('Width for button image. Example: 125'),
    'localize' => TRUE
  );

  $variable['addthis_image_height'] = array(
    'title' => t('Addthis Button image height'),
    'description' => t('Height for button image. Example: 16'),
    'localize' => TRUE
  );
  
  $variable['addthis_image_attributes'] = array(
    'title' => t('Addthis Button image HTML attributes'),
    'description' => t('Extra HTML attributes for img tag. Example: alt=""'),
    'localize' => TRUE
  );
  
  return $variable;
}