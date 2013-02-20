<?php

function communication_tools_variable_info($options) {
  $variable[COMMUN_TOOLS_PREFIX . 'form_header'] = array(
    'title' => t('Communications Tools Form Instructions'),
    'description' => t('Instructions that will be shown in "Send to a friend" overlay'),
    'localize' => TRUE
  );
  
  $variable[COMMUN_TOOLS_PREFIX . 'email_block_link'] = array(
    'title' => t('Send E-mail text link'),
    'description' => t('The text of the link that will be displayed on the Block.'),
    'localize' => TRUE
  );
  
  $variable[COMMUN_TOOLS_PREFIX . 'captcha_description_text'] = array(
    'title' => t('Communications Tools Captcha text'),
    'description' => t('Instructions that will be shown on "Captcha" challenge'),
    'localize' => TRUE
  );
  
  $variable[COMMUN_TOOLS_PREFIX . 'send_button_text'] = array(
    'title' => t('Communications Tools Send button text'),
    'description' => t('Send button text on Send to a friend form'),
    'localize' => TRUE
  );
  
  $variable[COMMUN_TOOLS_PREFIX . 'cancel_button_text'] = array(
    'title' => t('Communications Tools Cancel button text'),
    'description' => t('Cancel button text on Send to a friend form'),
    'localize' => TRUE
  );

  $variable[COMMUN_TOOLS_PREFIX . 'email_body'] = array(
    'title' => t('Communications Tools E-mail Body'),
    'description' => t('Write the e-mail body that the users will receive. The "@@" '.
                        'variables will be replaced by the values of the fields filled in the form. <br/>
                        Where: @username@ = Friend name, @referer@ = Referer name, @link@ = Target page link, @footer@ = Site Url'),
    'localize' => TRUE
  );

  $variable[COMMUN_TOOLS_PREFIX . 'success_page_path'] = array(
    'title' => t('Communications Tools Success page path'),
    'description' => t('Specify the path of the success page. Module provides send-to-a-friend/success-page as default (you should set it explicitly).'),
    'localize' => TRUE
  );

  $variable[COMMUN_TOOLS_PREFIX . 'success_page_body'] = array(
    'title' => t('Communications Tools Success page Body'),
    'description' => t(''),
    'localize' => TRUE
  );

  $name_fields_description = t("Display this message when user enter characters that don't match the pattern defined on 'Name validation pattern' field");
  $variable[COMMUN_TOOLS_PREFIX . 'name_error_message'] = array(
    'title' => t('Communications Tools Name field error message'),
    'description' => $name_fields_description,
    'localize' => TRUE
  );

  $variable[COMMUN_TOOLS_PREFIX . 'friend_name_error_message'] = array(
    'title' => t('Communications Tools Friend Name field error message'),
    'description' => $name_fields_description,
    'localize' => TRUE
  );

  $variable[COMMUN_TOOLS_PREFIX . 'facebook_block_link'] = array(
    'title' => t('Communications Tools Facebook text link'),
    'description' => t('The text of the link that will be displayed on the Block.'),
    'localize' => TRUE
  );

  return $variable;
}