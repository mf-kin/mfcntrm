<?php

/**
 *  Function that returns the offers views difinition. 
 */
function couponsinc_get_views_definition() {
  $view = new view;
  $view->name = 'offers';
  $view->description = 'List of all available offers';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Offers';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Coupons and Special Offers';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = '<h1>Savings and Special Offers</h1>
  <p>Enjoy special savings by downloading this coupon.</p>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Offer Image */
  $handler->display->display_options['fields']['field_offer_image']['id'] = 'field_offer_image';
  $handler->display->display_options['fields']['field_offer_image']['table'] = 'field_data_field_offer_image';
  $handler->display->display_options['fields']['field_offer_image']['field'] = 'field_offer_image';
  $handler->display->display_options['fields']['field_offer_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_offer_image']['alter']['path'] = 'special-offers/[nid]';
  $handler->display->display_options['fields']['field_offer_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_offer_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_offer_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_offer_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_offer_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_offer_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_offer_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_offer_image']['field_api_classes'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'couponsinc_coupons_type' => 'couponsinc_coupons_type',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'coupons-and-special-offers';

  return $view;
}