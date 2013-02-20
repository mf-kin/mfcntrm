<?php
/**
* @file
* Coupon admin file
*/

/**
 * Admin screen for coupons module. 
 */
function coupons_admin_config() {
  $form['coupon_sign_up_form_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coupon sign up form page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  
  $form['coupon_sign_up_form_page']['coupons_email_updates_url'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Coupon sign up form page url'),
    '#default_value'  => variable_get('coupons_email_updates_url', ''),
  );
  
  $form['coupon_sign_up_form_page']['coupons_email_updates_title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Coupon sign up form page title'),
    '#default_value'  => variable_get('coupons_email_updates_title', ''),
  );

  global $language;
  $form['coupon_sign_up_form_page']['coupons_email_updates_metatags'] = array(
    '#type'           => 'item',
    '#title'          => 'Page metatags',
    '#markup'         => 'Configure the sign up form page metatags ' .
                         urldecode(l('here', 'admin/config/search/metatags_quick/path?path=' . variable_get('coupons_email_updates_url', 'email-updates') . '/%&lang=' . $language->language)),
  );
  
  $form['coupon_sign_up_form_page']['coupons_header_email_updates_page'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Coupon sign up form header content'),
    '#default_value'  => variable_get('coupons_header_email_updates_page', ''),
  );
  
  $form['coupon_sign_up_form_page']['coupons_bottom_email_updates_page'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Coupon sign up form bottom content'),
    '#default_value'  => variable_get('coupons_bottom_email_updates_page', ''),
  );
  
  $form['email_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Close to expiration email configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['email_config']['coupon_email_to_send'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination e-mail address'),
    '#description' => t('Enter the destination list e-mail address separated by ";".'),
    '#default_value' => variable_get('coupon_email_to_send', ''),
  );
  
  $form['email_config']['coupon_sender_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender e-mail address'),
    '#description' => t('Enter the sender e-mail address.'),
    '#default_value' => variable_get('coupon_sender_email', ''),
  );
  
  $form['email_config']['coupon_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('Email subject.'),
    '#default_value' => variable_get('coupon_email_subject', ''),
  );
  
  $form['email_config']['coupon_email_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Content'),
    '#description' => t('Enter the body of the email.<br /> Use @COUPONS@ as placeholder to put coupons data on the text. Example: See the coupons below: @COUPONS@'),
    '#default_value' => variable_get('coupon_email_content', ''),
  );
  
  $form['email_config']['coupon_days_before_expire'] = array(
    '#type' => 'textfield',
    '#title' => t('Days before expire'),
    '#description' => t('Set the amount of days to receive an email before coupons expire.'),
    '#default_value' => variable_get('coupon_days_before_expire', ''),
  );
  
  $form['coupon_print'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coupon Printing'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['coupon_print']['coupon_minutes_before_code_expire'] = array(
    '#type' => 'textfield',
    '#title' => t('Minutes before coupon code expire'),
    '#description' => t('Set the amount of minutes for which a coupon code is valid.'),
    '#default_value' => variable_get('coupon_minutes_before_code_expire', ''),
  );
  
  $form['coupon_sign_up_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coupon sign up form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $coupon_sign_form_id = variable_get('coupon_sign_up_form_id');
  
  if (!empty($coupon_sign_form_id)) {
    $form['coupon_sign_up_form']['coupon_remove_sign_up_form'] = array(
      '#type' => 'checkbox',
      '#title' => 'Remove coupon sign up form',
      '#suffix' => 'View the current form at this ' . l('link', 'node/' . $coupon_sign_form_id),
      '#description' => 'Mark to remove the current coupon sign up form and generate a new one',  
    );
  }
  else {
    $form['coupon_sign_up_form']['#description'] = t('You can generate the Coupon Sign Up form based on an existent webform. In case None is selected, an empty webform will be generated.');

    $form['coupon_sign_up_form']['generate_sign_up_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Generate Coupon Sign Up form'),
    );

    $available_webforms = coupons_admin_get_available_webforms();
    
    $form['coupon_sign_up_form']['base_sign_up_form'] = array(
      '#type' => 'select',
      '#title' => 'Webform',
      '#options' => $available_webforms,
      '#description' => t('Select the base webform to generate Coupon Sign Up form'), 
    );
  }
  
  // Font Size for printing
  module_load_include('inc', 'coupons', 'coupons.image');
  $form['image_generator'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image generator font-size'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['image_generator']['coupon_image_font_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Font size',
    '#description' => t('Font-size of timestamp and date string on coupon\'s image.'), 
    '#default_value' => variable_get('coupon_image_font_size', COUPONS_DEFAULT_IMAGE_FONT_SIZE),
  );

  $form['coupon_orientation_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coupon Orientation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['coupon_orientation_form']['coupon_orientation'] = array(
    '#type' => 'radios',
    '#title' => t('Orientation'),
    '#options' => array(
      'portrait' => 'Portrait',
      'landscape' => 'Landscape'
    ),
    '#default_value' => variable_get('coupon_orientation', 'portrait'),
  );
  
  $form['#submit'][] = 'coupons_admin_config_submit';
  
  return system_settings_form($form);
}

/**
 * Return a list of available webforms registered in the system.
 */
function coupons_admin_get_available_webforms() {
  $rows = db_select('node', 'nd')
                    ->fields('nd', array('title','nid'))
                    ->condition('type', 'webform')
                    ->execute()
                    ->fetchAll();

  $available_webforms = array('' => 'None');
  foreach ($rows as $row) {
    $available_webforms[$row->nid] = $row->title;
  }
  
  return $available_webforms;
}

/**
 * Aditional submit function to flush all caches and rebuild the menu item.
 */
function coupons_admin_config_submit($form, &$form_state) {
  if (!empty($form_state['values']['generate_sign_up_form'])) {
    $node = new stdClass();
    $node->type = 'webform';
    $node->title = 'Coupon Sign Up';
    $node->language = LANGUAGE_NONE;
    $node->status = 1;
    node_object_prepare($node); // Set some default values.
    $node = node_submit($node); // Prepare node for a submit
    node_save($node);
  
    if (!empty($form_state['values']['base_sign_up_form'])) {
      $webform_node = node_load($form_state['values']['base_sign_up_form']);
      $last_weight = 0;
      foreach ($webform_node->webform['components'] as $component) {
        $component['nid'] = $node->nid;
        webform_component_insert($component);
        $last_weight = $component['weight'];
      }
      
      foreach (coupons_get_aditional_fiels() as $component) {
        $component['nid'] = $node->nid;
        $component['weight'] = ++$last_weight;
        webform_component_insert($component);
      }
    }        
    
    variable_set('coupon_sign_up_form_id', $node->nid);
    drupal_set_message('Coupon webform generated to view it access ' . l($node->title, 'node/' . $node->nid), 'status');
  }
  
  if (!empty($form_state['values']['coupon_remove_sign_up_form'])) {
    node_delete(variable_get('coupon_sign_up_form_id'));
    variable_set('coupon_sign_up_form_id', '');
  } 
  
  drupal_flush_all_caches();
}

/**
 * Load the coupons that are close to be expired based on the amount of days set in "admin/config/coupons"
 */
function coupons_get_items_close_to_expiration_date() {
  $result = db_select('field_data_field_coupon_end_date', 'f')
            ->fields('f')
            ->where('(TO_DAYS(date(f.field_coupon_end_date_value)) - TO_DAYS(NOW())) <= :days_before_expire', array(':days_before_expire' => variable_get('coupon_days_before_expire', 1)))
            ->execute()
            ->fetchAll();
  
  $coupons = array();
  foreach ($result as $items) {
    $coupons[] = node_load($items->entity_id);
  };
  
  return $coupons;
}

/**
 * Build the body content for the email to be sent for admin.
 * @param array $coupons Coupons next to expiration
 * @return type 
 */
function coupons_build_email_body($coupons) {
  $rows = array();
  foreach ($coupons as $coupon) {
    $rows[] = array(
      $coupon->title, 
      date('m-d-Y', strtotime($coupon->field_coupon_start_date['und'][0]['value'])), 
      date('m-d-Y', strtotime($coupon->field_coupon_end_date['und'][0]['value'])),
      l('Edit', 'node/' . $coupon->nid . '/edit', array('absolute' => TRUE)),  
    );
  }
  
  $header = array('Coupon', 'Start date', 'Expiration date', 'Edit');
  $body = variable_get('coupon_email_content', '');
  $body = str_replace('@COUPONS@', theme('table', array('header' => $header, 'rows' => $rows)), $body);

  return $body;
}

/**
 * Creates the mail envelope and sent it.
 * @param array $coupons Coupons next to expiration
 */
function coupons_send_mail($coupons) {
  $subject = variable_get('coupon_email_subject');
  $name = variable_get('site_name');
  $body = coupons_build_email_body($coupons);
  $from = variable_get('coupon_sender_email', '');
  $to = variable_get('coupon_email_to_send', '');
  $reply_to = $from;

  $from_name = trim(mime_header_encode($name)
          . ' <' . $from . '>');
  
  $my_module = 'coupons' ;
  $my_mail_token = 'mailer';

  $message = array(
    'id' => $my_module . '_' . $my_mail_token,
    'to' => $to,
    'subject' => $subject,
    'body' => drupal_wrap_mail($body),
    'headers' => array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/html; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer'                  => 'Drupal',
      'From'                      => $from,
      'Reply-To'                  => $reply_to,
    ),
  );

  $system = drupal_mail_system($my_module, $my_mail_token);
  $system->mail($message);
}

/*
 * Returns aditional fields (Coupon Id and Timestamp) to create on
 * coupon webform
 */
function coupons_get_aditional_fiels() {
  $components = array(
    'field_coupon_id' => array(
      'nid'       => 0,
      'pid'       => 0,
      'weight'    => 0,
      'form_key'  => 'field_coupon_id',  
      'name'      => 'Coupon Id',
      'type'      => 'hidden',
      'extra'     => array(                    
          'disabled' => FALSE,
          'unique' => FALSE,          
          'conditional_operator' => '='
      ),
      'mandatory' => 0,
    ),
    'field_coupon_timestamp' => array(
      'nid'       => 0,
      'pid'       => 0,
      'weight'    => 0,
      'form_key'  => 'field_coupon_timestamp',
      'name'      => 'Timestamp',
      'type'      => 'hidden',
      'extra'     => array(                    
          'disabled' => FALSE,
          'unique' => FALSE,          
          'conditional_operator' => '='
      ),
      'mandatory' => 0,
    ),
    'field_coupon_value' => array(
      'nid'       => 0,
      'pid'       => 0,
      'weight'    => 0,
      'form_key'  => 'field_coupon_value',
      'name'      => 'Coupon Value',
      'type'      => 'hidden',
      'extra'     => array(                    
          'disabled' => FALSE,
          'unique' => FALSE,          
          'conditional_operator' => '='
      ),
      'mandatory' => 0,
    ),    
    'field_coupon_campaign_id' => array(
      'nid'       => 0,
      'pid'       => 0,
      'weight'    => 0,
      'form_key'  => 'field_coupon_campaign_id',
      'name'      => 'Coupon Campaign ID',
      'type'      => 'hidden',
      'extra'     => array(                    
          'disabled' => FALSE,
          'unique' => FALSE,          
          'conditional_operator' => '='
      ),
      'mandatory' => 0,
    ),
    'field_coupon_print' => array(
      'nid'       => 0,
      'pid'       => 0,
      'weight'    => 0,
      'form_key'  => 'field_coupon_print',
      'name'      => 'Did the coupon print?',
      'type'      => 'hidden',
      'extra'     => array(                    
          'disabled' => FALSE,
          'unique' => FALSE,          
          'conditional_operator' => '='
      ),
      'mandatory' => 0,
    ),    
  );
  return $components;
}