<?php
/**
 * @file
 * Mobile Offers Admin form
 */

/**
 * Implementation of the admin form
 */
function mobile_offers_admin_form($form, &$form_state) {

  global $base_url;

  if(isset($_GET['change_ct'])) {
    delete_mobile_offers_related_variables();
  }
  
  $form['coupon_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coupon information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['coupon_information'][MOBILE_OFFERS_PREFIX . 'register_page_full_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Site Coupon Form URL'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'register_page_full_site', 'http://'),
    '#required' => TRUE
  );

  $form['coupon_information'][MOBILE_OFFERS_PREFIX . 'use_generic_coupon_url'] = array(
    '#type' => 'checkbox',
    '#title' => variable_get(MOBILE_OFFERS_PREFIX . 'register_page_full_site', 'http://') . t(' is a generic URL?'),
    '#description' => t('Select if you will use the above URL as a unique Coupon URL') . '<br />' .
      '<strong>' . t('If selected, there is no need to fill the Products Coupons ID\'s below') . '</strong>',
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'use_generic_coupon_url', ''),
    '#required' => FALSE
  );

  $product_content_type_selected = variable_get(MOBILE_OFFERS_PREFIX . 'product_content_type');

  if(isset($product_content_type_selected) && !empty($product_content_type_selected)) {
    $content_type = array($product_content_type_selected => ucwords($product_content_type_selected));
    $url_ct_change = url($base_url . '/admin/config/mobile-offers', array('query' => array('change_ct' => $product_content_type_selected)));

    $form['coupon_information'][MOBILE_OFFERS_PREFIX . 'product_content_type_fixed'] = array(
      '#type' => 'select',
      '#title' => t('Product Content Type'),
      '#options' => $content_type,
      '#default_value' => $product_content_type_selected,
      '#disabled' => TRUE,
      '#field_suffix' => l(t('Change'), $url_ct_change, array('attributes' => array('class' => 'change-content-type'))),
      '#description' => t('Be careful! If you change the Content Type you will loose the data stored in the fields below.'),
      '#required' => FALSE,
    );

    if (module_exists('nodequeue')) {
      $product_queue_qid = array_keys(nodequeue_get_qids($product_content_type_selected));
      $product_queue = nodequeue_load_queues($product_queue_qid);

      foreach ($product_queue as $queue) {
        $product_queue_name[$queue->qid] = $queue->title;
      }

      $form['coupon_information'][MOBILE_OFFERS_PREFIX . 'product_queue'] = array(
        '#type' => 'select',
        '#title' => t('Product Queue'),
        '#options' => $product_queue_name,
        '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'product_queue'),
        '#required' => FALSE,
      );
    }

    $product_nodes = get_nodes_from_node_type($product_content_type_selected);

    foreach ($product_nodes as $nid => $product) {
      $form['coupon_information'][MOBILE_OFFERS_PREFIX . $product_content_type_selected . '_' . $nid] = array(
        '#type' => 'textfield',
        '#title' => $product->title . t(' Form ID'),
        '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . $product_content_type_selected . '_' . $nid),
        '#required' => FALSE
      );
    }
  }
  else {
    $available_ct_list = node_type_get_types();
    $content_type = array('' => 'Select a Content Type');

    foreach($available_ct_list as $ct_key => $ct_obj) {
      $content_type[$ct_key] = $ct_obj->name;
    }

    $form['coupon_information'][MOBILE_OFFERS_PREFIX . 'product_content_type'] = array(
      '#type' => 'select',
      '#title' => t('Product Content Type'),
      '#ajax' => array(
        'callback' => 'ajax_load_products_callback',
        'wrapper' => 'content-type-list',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#options' => $content_type,
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'product_content_type', ''),
      '#required' => TRUE,
      '#suffix' => '<div id="content-type-list"></div>',
    );
  }

  $form['email_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['email_information'][MOBILE_OFFERS_PREFIX . 'email_subject'] = array(
      '#type' => 'textarea',
      '#title' => t('Email Subject'),
      '#description' => t('This is going to be the email subject text that the user is going to recieve.') ,
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'email_subject', variable_get('site_name', '')),
      '#required' => TRUE
  );
  
  $form['email_information'][MOBILE_OFFERS_PREFIX . 'email_copy'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Copy'),
    '#description' => t('This is going to be the email text that the user is going to recieve.') . '<br />' .
      t('User @form_url@ to replace the value for the correct Form URL') . '<br />' .
      '<strong>' . t('HTML tags are allowed.') . '</strong>',
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'email_copy', ''),
    '#required' => TRUE
  );

  $form['email_information'][MOBILE_OFFERS_PREFIX . 'default_mail_sender'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Mail sender'),
    '#description' => t('The user is going to see this email as the sender. (i.e: From: ' . variable_get("default_mail_sender", "noreply@pfizer.com") . ')'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'default_mail_sender', 'noreply@pfizer.com'),
    '#required' => TRUE
  );

  $form['email_information'][MOBILE_OFFERS_PREFIX . 'default_mail_sender_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Mail sender Name'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'default_mail_sender_name', 'Noreply'),
    '#required' => TRUE
  );

  $form['email_information'][MOBILE_OFFERS_PREFIX . 'reply_to_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply to this email'),
    '#description' => t('When the user clicks on Reply, this is going to be the replied email'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'reply_to_email', ''),
    '#required' => FALSE
  );

  $form['form_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['form_information'][MOBILE_OFFERS_PREFIX . 'product_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Product field title'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'product_field_title', ''),
    '#required' => FALSE
  );

  $form['form_information'][MOBILE_OFFERS_PREFIX . 'product_default_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Product field default value'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'product_default_value', 'Select Product'),
    '#required' => FALSE
  );

  $form['form_information'][MOBILE_OFFERS_PREFIX . 'form_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Email field title'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'form_field_title', ''),
    '#required' => FALSE
  );

  $form['form_information'][MOBILE_OFFERS_PREFIX . 'email_field_default_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Email field default value'),
    '#description' => theme('image', array('path' => drupal_get_path('module', 'mobile_offers') . '/images/form_mobile_offers.gif')),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'email_field_default_value', ''),
    '#required' => FALSE
  );

  $form['form_information'][MOBILE_OFFERS_PREFIX . 'submit_field_default_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit field default value'),
    '#description' => theme('image', array('path' => drupal_get_path('module', 'mobile_offers') . '/images/form_mobile_offers_submit.gif')),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'submit_field_default_value', 'Submit'),
    '#required' => TRUE
  );

  $form['page_content_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page content'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['page_content_information'][MOBILE_OFFERS_PREFIX . 'page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'page_title', 'Offers'),
    '#required' => TRUE
  );

  $form['page_content_information'][MOBILE_OFFERS_PREFIX . 'form_prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Form Text Prefix'),
    '#description' => theme('image', array('path' => drupal_get_path('module', 'mobile_offers') . '/images/page_content_prefix.gif')) . '<br />' .
      t('The text to be shown before the form.') . '<br />' .
      '<strong>' . t('HTML tags are allowed.') . '</strong>',
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'form_prefix', ''),
    '#required' => FALSE
  );

  $form['page_content_information'][MOBILE_OFFERS_PREFIX . 'form_suffix'] = array(
    '#type' => 'textarea',
    '#title' => t('Form Text Suffix'),
    '#description' => theme('image', array('path' => drupal_get_path('module', 'mobile_offers') . '/images/page_content_suffix.gif')) . '<br />' .
      t('The text to be shown after the form.') . '<br />' .
      '<strong>' . t('HTML tags are allowed.') . '</strong>',
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'form_suffix', ''),
    '#required' => FALSE
  );

  $form['thank_you_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thank You Page / Alert Message'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['thank_you_page'][MOBILE_OFFERS_PREFIX . 'create_thank_you_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Thank You page?'),
    '#description' => t('If checked, the module will create a new page to show after the form submission. If not, there will be only an alert with the copy.'),
    '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'create_thank_you_page', ''),
    '#required' => FALSE
  );

  if(variable_get(MOBILE_OFFERS_PREFIX . 'create_thank_you_page', '') == 1) {
    $form['thank_you_page'][MOBILE_OFFERS_PREFIX . 'thank_you_page_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Thank You page Title'),
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_page_title', 'Thank You'),
      '#required' => TRUE
    );

    $form['thank_you_page'][MOBILE_OFFERS_PREFIX . 'thank_you_page_content'] = array(
      '#type' => 'textarea',
      '#title' => t('Thank You page Content'),
      '#description' => '<strong>' . t('HTML tags are allowed.') . '</strong>',
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_page_content', ''),
      '#required' => TRUE
    );
    
  }
  else {
    $form['thank_you_page'][MOBILE_OFFERS_PREFIX . 'thank_you_alert_content'] = array(
      '#type' => 'textarea',
      '#title' => t('Thank You Alert Content'),
      '#description' => '<strong>' . t('HTML tags are NOT allowed.') . '</strong>',
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_alert_content', ''),
      '#required' => TRUE
    );
    $form['thank_you_page'][MOBILE_OFFERS_PREFIX . 'thank_you_alert_button'] = array(
      '#type' => 'textfield',
      '#title' => t('Thank You Alert Button'),
      '#description' => '<strong>' . t('HTML tags are NOT allowed.') . '</strong>',
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'thank_you_alert_button', 'Continue'),
      '#required' => FALSE
    );
  }
  
  $form['page_error_content'] = array(
      '#type' => 'fieldset',
      '#title' => t('Error menssages'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  
  $form['page_error_content'][MOBILE_OFFERS_PREFIX . 'error_empty_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Message when email is empty'),
      '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'error_empty_email', 'Email field is required.'),
      '#required' => TRUE
  );
  
  $form['page_error_content'][MOBILE_OFFERS_PREFIX . 'error_invalid_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Message when email is invalid'),
        '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'error_invalid_email', 'Please enter a valid email address.'),
        '#required' => TRUE
  );
  
  $form['page_error_content'][MOBILE_OFFERS_PREFIX . 'error_empty_product'] = array(
          '#type' => 'textfield',
          '#title' => t('Message when product is empty'),
          '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . 'error_empty_product', 'You must select a product.'),
          '#required' => TRUE
  );
  
  $form['#submit'][] = 'save_product_ids';
  
  return system_settings_form($form);
}

function mobile_offers_products_aliases_admin_form($form, &$form_state) {
  $form['products_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Products Aliases'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $product_content_type_selected = variable_get(MOBILE_OFFERS_PREFIX . 'product_content_type');
  
  if(isset($product_content_type_selected) && !empty($product_content_type_selected)) {
    $product_nodes = get_nodes_from_node_type($product_content_type_selected);
    
    foreach ($product_nodes as $nid => $product) {
      $form['products_information'][MOBILE_OFFERS_PREFIX . $product_content_type_selected . '_' . $nid . '_alias'] = array(
        '#type' => 'textfield',
        '#title' => $product->title . t(' alias'),
        '#default_value' => variable_get(MOBILE_OFFERS_PREFIX . $product_content_type_selected . '_' . $nid . '_alias'),
        '#description' => t('Give ') . $product->title . t(' an alias to be shown on the Coupons page.'),
        '#required' => FALSE
      );
    }
  }
  else {
    drupal_set_message(t('To add an alias to a specific Product, you need to go to General Configuration and select the Products Content Type first'), 'warning');
    
    $form['products_information']['products_aliases_warning'] = array(
      '#markup' => '<p>' . t('You must select the Product content type first') . '</p>',
    );
  }
  
  return system_settings_form($form);
}

function save_product_ids($form, &$form_state) {
  $product_nodes = get_nodes_from_node_type($form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type']);
  
  foreach ($product_nodes as $nid => $product) {
    if(isset($_POST[MOBILE_OFFERS_PREFIX . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '_' . $nid])) {
      variable_set(MOBILE_OFFERS_PREFIX . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '_' . $nid, $_POST[MOBILE_OFFERS_PREFIX . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '_' . $nid]);
    }
  } 
}