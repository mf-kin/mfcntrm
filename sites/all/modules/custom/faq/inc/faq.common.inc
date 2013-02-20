<?php

/**
 * @file
 * Manages FAQ (questions/answers) content, providing a search enabled listing
 */

function faq_add_js_settings() {
  $view = views_get_view('faqs');
  $mobile_device['type'] = 'desktop';
  
  if (module_exists('mobile_tools')) {
    $mobile_device = mobile_tools_get_device();
  }
 
  
  if (isset($view->display['faq_page_mobile']->display_options['path']) &&  $mobile_device['type'] == 'mobile') { 
    $view_path = $view->display['faq_page_mobile']->display_options['path'];
    $faq_settings = array(
      'faq_path' => $view_path,
      'faq_view_all_text' => variable_get('faq_view_all_text_mobile', 'View All'),
      'faq_search_faqs_text' => variable_get('faq_search_faqs_text_mobile', 'Search FAQs'),
      'faq_search_results_text' => variable_get('faq_search_results_text_mobile', 'Search Results For %filter_text%'),
	  	'faq_search_results_number' => variable_get('faq_search_results_number_mobile', ''),
    );drupal_add_js(array('faq' => $faq_settings), 'setting');

  }
  else  if (isset($view->display['faqs_page']->display_options['path'])) {
    $view_path = $view->display['faqs_page']->display_options['path'];

    $faq_settings = array(
      'faq_path' => $view_path,
      'faq_view_all_text' => variable_get('faq_view_all_text', 'View All'),
      'faq_search_faqs_text' => variable_get('faq_search_faqs_text', 'Search FAQs'),
      'faq_search_results_text' => variable_get('faq_search_results_text', 'Search Results For %filter_text%'),
      'faq_search_results_number' => variable_get('faq_search_results_number', ''),
    );
    drupal_add_js(array('faq' => $faq_settings), 'setting');


  }
}