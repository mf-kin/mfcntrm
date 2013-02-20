<?php

/**
 *
 * Implements hook_blok_view() for already registered block
 * 
 * @return type 
 */
function instant_win_block_view_already_registered() {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');
  drupal_add_library('system', 'ui.dialog');
  return array(
    'content' => array(
      'already_registered_link' => array(
        '#markup' => _instant_win_get_modal_link(t('Already Registered?'), url('already-registered'), 'already-registered'),
      ),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'instant_win') . '/scripts/modal_link.js',
        ),
      ),
    ),
  );
}

/**
 *
 * Implements hook_blok_view() for get a code block
 * 
 * @return type 
 */
function instant_win_block_view_get_a_code() {
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');
  drupal_add_library('system', 'ui.dialog');
  return array(
    'content' => array(
      'already_registered_link' => array(
        '#markup' => _instant_win_get_modal_link(t('NEED A GAME CODE?'), url('get-a-code'), 'get-a-code'),
      ),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'instant_win') . '/scripts/modal_link.js',
        ),
      ),
    ),
  );
}

function _instant_win_get_modal_link($link_title, $link_url, $div_id) {
  $output = <<<HTML
  <div id="{$div_id}" class="modal-link">
    <a href="{$link_url}" title="{$link_title}">{$link_title}</a>
  </div>
HTML;

  return $output;
}
