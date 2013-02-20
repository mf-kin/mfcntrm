<?php

/**
 *
 * @file
 * Administration config form for the FAQs module
 *
 */

 /**
 * The administrative module configuration form.
 */
function faq_admin_form() {
  $form = array();

  $form['faq_answer_expansion_mode'] = array(
    '#type' => 'select',
    '#title' => t('Answer expansion mode'),
    '#default_value' => variable_get('faq_answer_expansion_mode', ''),
    '#description' => t('Should the answers be collapsible/expandable?'),
    '#options' => array(
      '0' => 'No Collapsible',
      '1' => 'All Collapsible/Expandable',
      '2' => 'Only One Expandable', 
    ),
  );
  
  $form['faq_view_all_text'] = array(
    '#type' => 'textfield',
    '#title' => t('View all filter text'),
    '#description' => t('The text to be shown on view all filter'),
    '#default_value' => variable_get('faq_view_all_text', 'View All'),
  );


  $form['faq_search_faqs_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Search FAQs filter text'),
    '#description' => t('The text to be shown on input field filter'),
    '#default_value' => variable_get('faq_search_faqs_text', 'Search FAQs'),
  );

  $form['faq_search_results_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Search Results For" text'),
    '#description' => t('The text to be shown on "Search Results For" section.
                        The %filter_text% will be replaced to the filter text topic or input content'
                      ),
    '#default_value' => variable_get('faq_search_results_text', 'Search Results For %filter_text%'),
  );

  $form['faq_search_results_number'] = array(
    '#type' => 'textfield',
    '#title' => t('"Search Results Number" text'),
    '#description' => t('The text to be shown on "Search Results Number" section.
                         The %results_number% will be replaced by the number of results.
                         If left <strong>empty</strong>, no text will be displayed at all.'
                      ),
    '#default_value' => variable_get('faq_search_results_number', ''),
  );

  return system_settings_form($form);
}

/**
* The administrative module configuration form for mobile.
*/
function faq_admin_form_mobile() {
  $form = array();

  $form['faq_answer_expansion_mode_mobile'] = array(
    '#type' => 'select',
    '#title' => t('Answer expansion mode'),
    '#default_value' => variable_get('faq_answer_expansion_mode_mobile', 1),
    '#description' => t('Should the answers be collapsible/expandable?'),
    '#options' => array(
      '0' => 'No',
      '1' => 'Yes',
  ),
  );

  $form['faq_view_all_text_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('View all filter text'),
    '#description' => t('The text to be shown on view all filter'),
    '#default_value' => variable_get('faq_view_all_text_mobile', 'View All'),
  );


  $form['faq_search_faqs_text_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Search FAQs filter text'),
    '#description' => t('The text to be shown on input field filter'),
    '#default_value' => variable_get('faq_search_faqs_text_mobile', 'Search FAQs'),
  );

  $form['faq_search_results_text_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('"Search Results For" text'),
    '#description' => t('The text to be shown on "Search Results For" section. 
                        The %filter_text% will be replaced to the filter text topic or input content'
  ),
    '#default_value' => variable_get('faq_search_results_text_mobile', 'Search Results For %filter_text%'),
  );

  $form['faq_search_results_number_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('"Search Results Number" text'),
    '#description' => t('The text to be shown on "Search Results Number" section. 
                         The %results_number% will be replaced by the number of results. 
                         If left <strong>empty</strong>, no text will be displayed at all.'
  ),
    '#default_value' => variable_get('faq_search_results_number_mobile', ''),
  );
  
  $form['faq_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#default_value' => variable_get('faq_page_title', 'FAQs'),
  );

  return system_settings_form($form);
}