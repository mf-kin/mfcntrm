<?php
/**
 * @file
 * Testimonial submission functions
 */
 
/**
 * Gateway handler to Testimonial current state edition page or confirmation page.
 */
function testimonial_submission_gateway($form, &$form_state) {
  
  //If storage is not set the form is being accessed for the first time so, a default value
  //is set.
  if (!isset($form_state['storage'])) {
    $form_state['storage']['step'] = 1;
  }
  
  switch ($form_state['storage']['step']) {
    case 1:
      $form = testimonial_submission_form($form, $form_state);
      break;
    case 2:
      $form = testimonial_submission_confirmation_form($form, $form_state);
      break;
  }
  return $form;
}

/**
 * Get content for Testimonials Thank you page.
 */
function testimonial_thank_you_page() {
  $text = variable_get('testimonial_thankyou_content', '');
  $text = preg_replace('/sites\/default\/files/i', variable_get('file_public_path', 'sites/default/files'), $text);
  $text = preg_replace('/sites\/all\/themes\/([^\/]*)/i', path_to_theme(), $text);
  return $text;
}

/**
 * Form builder. Creates the Testimonial form.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function testimonial_submission_form($form, &$form_state) {

  $form['#prefix'] = '<h1>' . variable_get('testimonial_step1_title', 'Testimonials - Step 1') .
      '</h1>' . variable_get('testimonial_step1_body', '');
  
  $required_message = variable_get('testmonial_required_message_text', 'Fields are required');
  
  if (TESTIMONIAL_USE_VERLAY) {
    $form['error_messages'] = array(
      '#markup' => theme('status_messages'),
    );
  }
  
  $form['required_fields'] = array(
    '#markup' => t('<span class="form-required" title="' . $required_message . '">*</span>' . $required_message),
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => isset($form_state['values']['name']) ? $form_state['values']['name'] :
      (isset($form_state['storage']['values']['name']) ? $form_state['storage']['values']['name'] : NULL),  
  );
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#element_validate' => array('testimonial_form_email_validate'),
    '#default_value' => isset($form_state['values']['email']) ? $form_state['values']['email'] :
        (isset($form_state['storage']['values']['email']) ? $form_state['storage']['values']['email'] : NULL),  
  );
  
  $product_related = testimonial_get_all_associated_content();
  $product_using_get = NULL;
  if (!empty($_GET['product']) && is_numeric($_GET['product']) 
          && array_key_exists($_GET['product'], $product_related)) {
    $product_using_get = $_GET['product'];
  }
  $default_value = isset($form_state['values']['product']) ? $form_state['values']['product'] :
    (isset($form_state['storage']['values']['product']) ? $form_state['storage']['values']['product'] : NULL);
  if ($default_value == NULL) {
    $default_value = $product_using_get;
  }
    
  $form['product'] = array(
    '#type' => 'select',
    '#title' => t('Products'),
    '#empty_option' => t('Select A Product'),
    '#options' => $product_related,
    '#required' => TRUE,
    '#default_value' => $default_value,  
  );
  
  $form['email_confirm'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirm Email'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#element_validate' => array('testimonial_form_email_confirm_validate'),
    '#default_value' => isset($form_state['values']['email_confirm']) ? $form_state['values']['email_confirm'] :
      (isset($form_state['storage']['values']['email_confirm']) ?
          $form_state['storage']['values']['email_confirm'] : NULL), 
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Story Title'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => isset($form_state['values']['title']) ? $form_state['values']['title'] :
      (isset($form_state['storage']['values']['title']) ? $form_state['storage']['values']['title'] : NULL),  
  );
  
  $form['body'] = array(
    '#type' => 'textarea',
    '#resizable' => FALSE,
    '#title' => t('Story'),
    '#required' => TRUE,
    '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] :
      (isset($form_state['storage']['values']['body']) ? $form_state['storage']['values']['body'] : NULL),  
    '#element_validate' => array('testimonial_form_body_validate'),
  );
  
  $form['disclaimer'] = array(
    '#markup' => variable_get('testimonial_step1_disclaimer', ''),
  );
  
  $form['release_agreement_title'] = array(
    '#markup' => variable_get('testimonial_release_agreement_title', ''),
  );
  
  $form['release_agreement_body'] = array(
    '#markup' => variable_get('testimonial_release_agreement_body', ''),
  );
  $form['agreement'] = array(
    '#type' => 'radios',
    '#options' => array(
      'agree' => t('I agree'),
      'disagree' => t('I disagree'),
    ),
    '#element_validate' => array('testimonial_form_agreement_validate'),
    '#default_value' => isset($form_state['values']['agreement']) ? $form_state['values']['agreement'] :
      (isset($form_state['storage']['values']['agreement']) ? $form_state['storage']['values']['agreement'] : NULL),  
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Story'),
    '#attributes' => array('title' => 'Submit'),
  );
  
  $form['#theme'] = 'testimonial_submission_form';
  
  return $form;
}

/**
 * Validate limit words Body
 */
function testimonial_form_body_validate($element, $form_state) {
  $arrValue = explode(' ', $element['#value']);
  if (count($arrValue) > intval(variable_get('testimonial_body_limit_words', 0))) {
    form_error($element, t('Exceeded the limit of ' . variable_get('testimonial_body_limit_words', 0) . ' words'));
  }
}

/**
 * Validate e-mail address.
 */
function testimonial_form_email_validate($element, $form_state) {
  if (valid_email_address($form_state['values'][$element['#name']]) == FALSE) {
    form_error($element, t('The email is not valid'));
  }
}

/**
 * Validate the confirmation e-mail address.
 */
function testimonial_form_email_confirm_validate($element, $form_state) {
  if ($form_state['values'][$element['#name']] != $form_state['values']['email']) {
    form_error($element, t(variable_get('testimonial_email_conf_error_msg', 'The email confirmation does not match the email address')));
  }
}

/**
 * Validate the agreement checkboxes.
 */
function testimonial_form_agreement_validate($element, $form_state) {
  if ('agree' != $form_state['values'][$element['#name']]) {
    form_error($element, t('You must agree with the Testimonial Release Agreement'));
  }
}

/**
 * Process testimonial form.
 */
function testimonial_submission_gateway_submit($form, &$form_state) {
  if ($form_state['storage']['step'] == 1) {
    $form_state['rebuild'] = TRUE;
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['step'] += 1;
  }
  else {
    $form_state['redirect'] = variable_get('testimonial_thankyou_url', '<front>');
    
    $node = new stdClass();
    $node->type = 'testimonial';
    $node->title = $form_state['storage']['values']['title'];
    $node->language = LANGUAGE_NONE;
    $node->status = 0;
    $node->body[$node->language][0]['value'] = $form_state['storage']['values']['body'];
    $node->field_testimonial_email[$node->language][0]['value'] = $form_state['storage']['values']['email'];
    $node->field_testimonial_firstname[$node->language][0]['value'] = $form_state['storage']['values']['name'];
    $node->field_testimonial_prod_nid[$node->language][0]['nid'] = $form_state['storage']['values']['product'];
    node_object_prepare($node); // Set some default values.
    $node = node_submit($node); // Prepare node for a submit
    node_save($node);
  }
}

/**
 * Generates the testimonial confirmation page.
 */
function testimonial_submission_confirmation_form($form, &$form_state) {
  
  $form = array();
  
  $form['#prefix'] = '<h1>' . variable_get('testimonial_step2_title', 'Testimonials - Step 2') .
          '</h1>' . variable_get('testimonial_step2_body', '');
  
  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('First Name:'),
    '#markup' => check_plain($form_state['storage']['values']['name']),
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'item',
    '#title' => t('Email:'),
    '#markup' => check_plain($form_state['storage']['values']['email']),
    '#required' => TRUE,
  );
  $form['story_title'] = array(
    '#type' => 'item',
    '#title' => t('Story Title:'),
    '#markup' => check_plain($form_state['storage']['values']['title']),
    '#required' => TRUE,
  );
  $form['product'] = array(
    '#type' => 'item',
    '#title' => t('Product:'),
    '#markup' => node_load($form_state['storage']['values']['product'])->title,
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#markup' => '<label class="testimonial-story-title">' . check_plain($form_state['storage']['values']['title']) .
      '</label><p class="testimonial-story-body">' . nl2br(check_plain($form_state['storage']['values']['body'])) . '</p>',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('title' => 'Submit'),
  );
  
  $form['#theme'] = 'testimonial_confirmation_form';
  
  $form['edit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit'),
    '#submit' => array('testimonial_submission_confirmation_edit_form'),
    '#attributes' => array('title' => 'Edit'),
  );
  
  $form['#attributes'] = array(
    'class' => array('testimonial-preview-page'),  
  );
  
  return $form;
}

/**
 * Set to rebuild the form as it's coming back for the first step. 
 * Also decreased the number of the current step.
 */
function testimonial_submission_confirmation_edit_form($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  //Go back to the first step of Testimonial form to edit the values submited.
  $form_state['storage']['step']--;
}

/**
 * Get options for related content on Testimonial page.
 */
function testimonial_get_all_associated_content() {
  $contents = array();
  
  //Get the instance for the node reference field.
  $instance = _field_invoke_get_instances('node', 'testimonial', array('field_name' => 'field_testimonial_prod_nid', 'deleted' => FALSE));
  
  if (isset($instance[0])) {
    //Get additional info for the field.
    $field = field_info_field('field_testimonial_prod_nid');
    $instance = $instance[0];
    // Abstract over the actual field columns, to allow different field types to
    // reuse those widgets.
    $value_key = key($field['columns']);

    $type = str_replace('options_', '', $instance['widget']['type']);
    $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;
    $required = TRUE;
    $has_value = FALSE;
    
    //Get the properties for the field.
    $properties = _options_properties($type, $multiple, $required, $has_value);
    // Prepare the list of options.
    $options = _options_get_options($field, $instance, $properties);
    //Remove the first option as we already have one.
    unset($options['_none']);
    $contents = $options;
  }
  
  return $contents;
}

function testimonial_delivery_page($page_callback_result) {

  if (TESTIMONIAL_USE_VERLAY) {
    $page = array(
      '#theme_wrappers' => array('html'),
      'content' => array(
        '#markup' => drupal_render(drupal_get_form($page_callback_result['#form_id'])),
      ),
    );
    print drupal_render($page);
  }
  else {
    drupal_deliver_html_page($page_callback_result);
  }
}

function testimonial_delivery_success_page($page_callback_result) {
 
  if (TESTIMONIAL_USE_VERLAY) {
    $page = array(
      '#theme_wrappers' => array('html'),
      'content' => array(
        '#markup' => $page_callback_result,
      ),
    );
    print drupal_render($page);
  }
  else {
    drupal_deliver_html_page($page_callback_result);
  }
}