<?php

function gmapslivesearch_variable_info($options) {
  $variable['gmapslivesearch_buy_online_address_search_label'] = array(
    'title' => t('TextBox address label'),
    'description' => t('Label for field address search on buy online'),
    'localize' => TRUE
  );
  
  $variable['gmapslivesearch_buy_online_store_search_label'] = array(
    'title' => t('Select stores label'),
    'description' => t('Label for field store on buy online'),
    'localize' => TRUE
  );
  
  $variable['gmapslivesearch_great_distance_message'] = array(
    'title' => t('Great distance message'),
    'description' => t('Message that appears when the stores on Find a Store are too far away'),
    'localize' => TRUE
  );
  
  $variable['gmapslivesearch_field_buy_now_page_title'] = array(
    'title' => t('Gmaps live search page title'),
    'description' => t('Title for Gmaps live search page'),
    'localize' => TRUE
  );
  
  $variable['gmapslivesearch_field_buy_now_page_path'] = array(
    'title' => t('Path to Gmaps live search page'),
    'description' => t('URL for Gmaps live search page'),
    'localize' => TRUE
  );


  return $variable;
}