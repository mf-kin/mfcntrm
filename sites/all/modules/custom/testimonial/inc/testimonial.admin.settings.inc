<?php
/**
 * @file
 * Testimonial admin settings
 */

/**
 * Form builder. Testimonial settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function testimonial_admin_settings($form_state, $node) {
  $form = array();

  $form['email_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email configurations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email_config']['testimonial_GSSE_email'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('GSSE e-mail address'),
    '#description' => t('Enter the GSSE distribution list e-mail address. For multiple e-mails use comma to separate (Ex: email1@test.com, email2@test.com)'),
    '#default_value' => variable_get('testimonial_GSSE_email', ''),
  );

  $form['email_config']['testimonial_sender_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender e-mail address'),
    '#description' => t('Enter the sender e-mail address.'),
    '#default_value' => variable_get('testimonial_sender_email', ''),
  );

  $form['email_config']['testimonial_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('Email subject.'),
    '#default_value' => variable_get('testimonial_email_subject', ''),
  );

  $form['email_config']['testimonial_email_content'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Content'),
    '#description' => t('Enter body of the email.'),
    '#default_value' => variable_get('testimonial_email_content', ''),
  );
  
  $form['testimonial_body_limit_words'] = array(
    '#type' => 'textfield',
    '#title' => t('Validate limit of words'),
    '#description' => t('Enter the limit of words'),
    '#default_value' => variable_get('testimonial_body_limit_words', 0),
    '#maxlength' => 5,
    '#size' => 5,
  );

  $form['testimonial_step1_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Step 1 - Page Title'),
    '#default_value' => variable_get('testimonial_step1_title', ''),
  );

  $form['testimonial_step1_body'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Step 1 - Page Body'),
    '#default_value' => variable_get('testimonial_step1_body', ''),
  );

  $form['testimonial_step1_disclaimer'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Step 1 - Story disclaimer'),
    '#description' => t('Enter the text to be shown below the Story input field.'),
    '#default_value' => variable_get('testimonial_step1_disclaimer', ''),
  );

  $form['testimonial_release_agreement_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Step 1 - Release Agreement Title'),
    '#description' => t('Enter the title of the Release Agreement to be shown below the form.'),
    '#default_value' => variable_get('testimonial_release_agreement_title', ''),
  );

  $form['testimonial_release_agreement_body'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Step 1 - Release Agreement Body'),
    '#description' => t('Enter the text of the Release Agreement to be shown below the form.'),
    '#default_value' => variable_get('testimonial_release_agreement_body', ''),
  );

  $form['testmonial_required_message_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Required message'),
    '#description' => t('Text to indicate that the fields are required.'),
    '#default_value' => variable_get('testmonial_required_message_text', 'Fields are required'),    
  );

  $form['testimonial_step2_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Step 2 - Page Title'),
    '#default_value' => variable_get('testimonial_step2_title', ''),
  );

  $form['testimonial_step2_body'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Step 2 - Page Body'),
    '#default_value' => variable_get('testimonial_step2_body', ''),
  );

  $form['testimonial_thankyou'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thank You'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['testimonial_thankyou']['testimonial_thankyou_content'] = array(
    '#type' => 'text_format',
    '#format' => 'full_html',
    '#title' => t('Thank You page content'),
    '#default_value' => variable_get('testimonial_thankyou_content', ''),  
  );

  $form['testimonial_thankyou']['testimonial_thankyou_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Thank You page url'),
    '#description' => t('Enter the internal path to the Thank You page.'),
    '#default_value' => variable_get('testimonial_thankyou_url', 'share-your-story/thank-you'),
  );

  $form['testimonial_associated_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Associated content'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['testimonial_associated_content']['info'] = array(
    '#markup' => '<p>In order to set the content which should be associated to the testimonials, click ' . 
      l('here',
        'admin/structure/types/manage/testimonial/fields/field_testimonial_prod_nid/field-settings',
        array('attributes' => 
          array('target' => '_blank',
                'class' => 'overlay-exclude',
          ),
        )
      ) . '.</p>',
  );

  $form['testimonial_generated_pdf_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title generated PDF'),
    '#description' => t('Title showed inside of the generated pdf.'),
    '#default_value' => variable_get('testimonial_generated_pdf_title', ''),  
  );

  $form['form_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form validation configurations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['form_config']['testimonial_email_conf_error_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail confirmation field error message'),
    '#description' => t('The e-mail confirmation field error message'),
    '#default_value' => variable_get('testimonial_email_conf_error_msg', ''),
  );
  
  $form['overlay_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['overlay_configuration']['testimonial_form_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use overlay'),
    '#description' => t('Check if testimonial pages will appear on overlay'),
    '#default_value' => variable_get('testimonial_form_overlay', ''),  
  );
  
  $form['overlay_configuration']['testimonial_form_preloader_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Preloader Image Path'),
    '#description' => t('Specify the path for the Preloader Image from the Drupal File Public Path. Ex.: images/preloader.gif'),
    '#default_value' => variable_get('testimonial_form_preloader_image', ''),
  );
  
  return system_settings_form($form);

}

function testimonial_admin_settings_validate($form, &$form_state) {
  $form_state['values']['testimonial_email_content'] = $form['email_config']['testimonial_email_content']['value']['#value'];
  $form_state['values']['testimonial_step1_body'] = $form['testimonial_step1_body']['value']['#value'];
  $form_state['values']['testimonial_step2_body'] = $form['testimonial_step2_body']['value']['#value'];
  $form_state['values']['testimonial_release_agreement_body'] = $form['testimonial_release_agreement_body']['value']['#value'];
  $form_state['values']['testimonial_step1_disclaimer'] = $form['testimonial_step1_disclaimer']['value']['#value'];
  $form_state['values']['testimonial_thankyou_content'] = $form['testimonial_thankyou']['testimonial_thankyou_content']['value']['#value'];
}

/**
 * Return all content types created on Drupal CMS to associate which kind of content
 * a testimonial can be submitted.
 */
function testimonial_get_available_content_types() {
  $types = array();
  foreach (node_type_get_types() as $machine_name => $type) {
    $types[$machine_name] = $type->name;
  }
  return $types;
}
