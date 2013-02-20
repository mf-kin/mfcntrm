<?php 

/**
 * Implementation of hook_html_head_alter.
 * It's needed so we are able to change the page header. For example, we can:
 * - add IE7 compatibility metatag
 * - add mobile friendly tags
 * - etc
 */
function pfizer_html_head_alter(&$head_elements) {
  //Add IE 7 Compatible tag
  $head_elements['internet_explorer_compatible'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array (
      'http-equiv' => 'X-UA-Compatible',
      'content' => 'IE=7'
    ),
    '#weight' => '-1000'
  );
}

/**
 *  Return the proper sidebar class, based on which side bars are available for the page
 */
function pfizer_get_sidebars_class($vars){

  $has_left_side_bar  = !empty($vars['page']['left_sidebar']);
  $has_right_side_bar = !empty($vars['page']['right_sidebar']);

  if ($has_left_side_bar && $has_right_side_bar) {
    $sidebars_class = 'both_sidebars';
  } elseif ($has_left_side_bar) {
    $sidebars_class = 'left_sidebar';
  } elseif ($has_right_side_bar) {
    $sidebars_class = 'right_sidebar';
  } else {
    $sidebars_class = 'no_sidebars';
  }
  
  $item = menu_get_item();
  $custom_class = check_plain(drupal_get_path_alias(preg_replace('/[^\w]/', '-', drupal_strtolower($item['title']))));
      
  $sidebars_class .= ' ' . $custom_class;  
  return $sidebars_class;
}

function pfizer_preprocess_page(&$vars) {
  $vars['page']['content']['sidebar_class'] = pfizer_get_sidebars_class($vars); 
  pfizer_css_alter($vars);
  drupal_add_js(array('themePath' => path_to_theme()), 'setting');
}

function pfizer_css_alter(&$vars) {

 if(!user_is_logged_in()){
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

function pfizer_breadcrumb(&$vars) {
  $breadcrumb = $vars['breadcrumb'];
  if (!empty($breadcrumb)) {
    $crumbs = '<div class="breadcrumb"><ul>';
    $array_size = count($breadcrumb);

    $i = 0;
    while ( $i < $array_size) {
      $crumbs .= '<li class="breadcrumb-' . $i;
      if ($i == 0) {
        $crumbs .= ' first';
      }
      $crumbs .=  '">' . $breadcrumb[$i] . '</li>';
      $i++;
    }
    $title = menu_get_active_title();
    $title = $title ? $title : drupal_get_title();
    $crumbs .= '<li class="last active">'. $title .'</li></ul></div>';
    return $crumbs;
  }
}
 