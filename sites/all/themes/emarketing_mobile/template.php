<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the Boron theme.
 */

function emarketing_mobile_preprocess_views_view(&$vars) {
  _emarketing_mobile_perform_view_preprocess(__FUNCTION__, $vars);  
}

/**
 * Gateway function responsible for calling appropriate preprocess function (if it exists),
 * according to the view/block being displayed.
 * 
 * @param string $preprocess_prefix
 * @param array $vars
 */
function _emarketing_mobile_perform_view_preprocess($preprocess_prefix, &$vars) {
  $view = $vars['view'];
  $function = implode('__', array($preprocess_prefix, $view->name, $view->current_display));
  
  if (function_exists($function)) {
    $function($vars);
  }
}
 
function emarketing_mobile_preprocess_page(&$vars) {
  emarketing_mobile_css_alter($vars);
  emarketing_mobile_js_alter($vars);
  _emarketing_mobile_generate_suggestions('page', $vars);
}

function _emarketing_mobile_generate_suggestions($preprocess_type, &$vars) {
  if (module_exists('path')) {
    $alias = drupal_get_path_alias(str_replace('/edit', '', $_GET['q']));
    $alias_exp = explode('/', $alias);
    $suggestions = array();
    $template_filename = $preprocess_type;
    
    $mobile_type = 'desktop';
    if (module_exists('mobile_tools')) {
      $device_type = mobile_tools_get_device();
      $mobile_type = $device_type['type'];
    }

    if (drupal_is_front_page()) {
      $suggestions[] = $template_filename . '-front';
    }

    foreach ($alias_exp as $path_part) {
      $template_filename = $template_filename . '__' . $path_part;
      $suggestions[] = str_replace('-', '_', $template_filename);
    }
    
    // Template Find a Store
    $gmapslivesearch_page = variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH, '');
    if ($mobile_type == 'mobile') {
      $gmapslivesearch_page = variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH_MOBILE,  variable_get(GMAPSLIVESEARCH_FIELD_BUY_NOW_PAGE_PATH, ''));
    }

    if ($alias == $gmapslivesearch_page ) {
      $template_filename =  'page__gmapslivesearchfindstore'; 
      $suggestions[] = $template_filename;
    }
    
    // Template Offers
    $offers_page = theme_get_setting('mobile_offers_path');
    if ($alias == $offers_page || $alias == 'offers') {
      $template_filename =  'page__offersmobile'; 
      $suggestions[] = $template_filename;
    }
    // Template Faqs
    $display_id = 'faq_page_mobile';
    $view = views_get_view('faqs');
    if (!empty($view)) {
      $view->set_display($display_id);
      $faq_page = $view->get_url();
    }
    
    if($alias == $faq_page) {
      $template_filename =  'page__faqsmobile'; 
      $suggestions[] = $template_filename;
    }
    
    $vars['theme_hook_suggestions'] = array_merge($vars['theme_hook_suggestions'], $suggestions);
    $vars['theme_hook_suggestions'] = array_unique($vars['theme_hook_suggestions']);
  }
}

function emarketing_mobile_css_alter(&$vars) {
  if (!user_is_logged_in()) {
    unset($vars[drupal_get_path('module', 'webform') . '/css/webform.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.menus.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.menus.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.menus.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.base.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.messages.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.theme.css']);
    unset($vars[drupal_get_path('module', 'field_group') . '/field_group.css']);
    unset($vars[drupal_get_path('module', 'views') . '/css/views.css']);
    unset($vars[drupal_get_path('module', 'ctools') . '/css/ctools.css']);
    unset($vars[drupal_get_path('module', 'ckeditor') . '/ckeditor.css']);
    unset($vars[drupal_get_path('module', 'field') . '/theme/field.css']);
    unset($vars[drupal_get_path('module', 'node') . '/node.css']);
    unset($vars[drupal_get_path('module', 'search') . '/search.css']);
    unset($vars[drupal_get_path('module', 'user') . '/user.css']);
    unset($vars[drupal_get_path('module', 'system') . '/system.menus.css']);
  }
}

function emarketing_mobile_js_alter(&$vars) {
  drupal_add_js(array('themePath' => drupal_get_path('theme', 'emarketing_mobile')), 'setting');
  
  // Adding the iScroll js only on the set pages on theme config
  $nodes_to_load_iscroll = explode("\n", theme_get_setting('pages_to_load_iscroll'));

  foreach($nodes_to_load_iscroll as $key => $node) {
    if($_GET['q'] == $node) {
      drupal_add_js(drupal_get_path('theme', 'emarketing_mobile') . '/scripts/jquery.carousel.js');
      drupal_add_js(drupal_get_path('theme', 'emarketing_mobile') . '/scripts/jquery.touchwipe.1.1.1.js');
      drupal_add_js(array('loadIScroll' => 'true'), 'setting');
    }
  }
  
  if(module_exists('mobile_tools')) {
    drupal_add_js(array('mobileDevice' => mobile_tools_get_device()), 'setting');
  }
  else {
    drupal_add_js(array('mobileDevice' => array('type' => 'desktop', 'group' => '')), 'setting');
  }

  $carousel_interval = theme_get_setting('carousel_interval');

  if(!empty($carousel_interval)) {
    drupal_add_js(array('carousel_interval' => $carousel_interval), 'setting');
  }

}

// Changes the default meta content-type tag to the shorter HTML5 version
function emarketing_mobile_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
    
  $head_elements['ios_web_app'] = array (
	'#type' => 'html_tag',
	'#tag' => 'meta',
	'#attributes' => array (
	  'name' => 'apple-mobile-web-app-capable',
	  'content' => 'yes',
     ),
  );

  $head_elements['viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width; initial-scale=1; maximum-scale=1, user-scalable=no',
    ),
  );
  $head_elements['apple-touch-icon'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'apple-touch-icon',
        'href' => url(  variable_get('file_public_path') .'/'. theme_get_setting('mobile_bookmark_icon'), array('absolute' => 'true')),
    ),
  );
  $head_elements['apple-touch-icon-precomposed'] = array(
        '#type' => 'html_tag',
        '#tag' => 'link',
        '#attributes' => array(
          'rel' => 'apple-touch-icon-precomposed',
          'href' => url(  variable_get('file_public_path') .'/'. theme_get_setting('mobile_bookmark_icon'), array('absolute' => 'true')),
    ),
  );

  $head_elements['telephone_number_detection'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'format-detection',
      'content' => 'telephone=no',
    ),
  );
}

/**
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 */
function emarketing_mobile_preprocess_html(&$vars) {
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = 'version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  }
  else {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.2//EN" "http://www.openmobilealliance.org/tech/DTD/xhtml-mobile12.dtd">' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }
  
  // Adding class to body on home page mobile
  $mobile_home_page_node = theme_get_setting('node_mobile_home_page');
  
  if($_GET['q'] == $mobile_home_page_node) {
    foreach($vars['classes_array'] as $key => $class) {
      if($class == 'not-front') {
        unset($vars['classes_array'][$key]);
      }
    }
    $vars['classes_array'][] = 'front-mobile';
  }
  
}

function emarketing_mobile_menu_link__menu_main_menu_mobile(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  // If the link have any class, get it and add it to the li
  if( isset( $element['#original_link']['options']['attributes']['class'] )  ){
    $original_link_class = ' ' . implode($element['#original_link']['options']['attributes']['class'], ' ');
    $element['#attributes']['class'][] = $original_link_class;
  }
  
  // If the menu item has sublinks remove original link.
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
    $element['#localized_options']['attributes']['class'][] = 'no-link';
  }
  
  $element['#localized_options']['html'] = TRUE;

  $span_before = '';
  if ($element['#title'] == 'Home') {
    $span_before  = '<span class="general-sprite home"></span>';
  }
  $output = l(
    $span_before . $element['#title'] . '<span class="general-sprite"></span>', 
    $element['#href'],
    $element['#localized_options']
  );
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

function emarketing_mobile_process_html(&$vars) {
  $vars['page'] = str_replace('!desktop_url', _emarketing_mobile_replace_desktop_url(), $vars['page']);
}

function _emarketing_mobile_replace_desktop_url() {
  $options = array();
  $url = variable_get('mobile_tools_desktop_url') . '/';

  $options = array( 'query' => array(
      'device' => 'desktop'
  ));
  
  if (drupal_get_http_header('status')==NULL) {
    if (request_path()!='') { //not front page
      $url .= drupal_get_path_alias();
    }
  }
  
  return url($url, $options);
}
