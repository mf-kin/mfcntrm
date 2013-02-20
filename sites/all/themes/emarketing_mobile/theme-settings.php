<?php

function emarketing_mobile_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['isroll_load_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('iScroll Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['isroll_load_pages']['pages_to_load_iscroll'] = array(
    '#type' => 'textarea',
    '#title' => t('Nodes to load iScroll'),
    '#default_value' => theme_get_setting('pages_to_load_iscroll'),  
    '#required' => FALSE
  );

  $form['mobile_home_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Home Page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['mobile_home_page']['node_mobile_home_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Home Page Mobile node'),
    '#default_value' => theme_get_setting('node_mobile_home_page'),  
    '#required' => FALSE
  );

  $form['mobile_home_page']['carousel_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Carousel Interval'),
    '#default_value' => theme_get_setting('carousel_interval'),  
    '#required' => FALSE
  );
  
  $form['mobile_bookmark'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bookmark Icon'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  
  $form['mobile_bookmark']['mobile_bookmark_icon'] = array(
      '#type' => 'textfield',
      '#title' => t('Icon to display on the bookmark'),
      '#default_value' => theme_get_setting('mobile_bookmark_icon'),  
      '#required' => FALSE
  );
  
  $form['mobile_offers'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mobile Offers'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  
  $form['mobile_offers']['mobile_offers_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path Offers'),
      '#default_value' => theme_get_setting('mobile_offers_path'),  
      '#required' => FALSE
  );
}