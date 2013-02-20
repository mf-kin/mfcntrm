<?php

/**
 * Checks whether current page is an admin page.
 *
 * @return boolean
 */
function special_replace_is_admin_page() {
  return stripos($_GET['q'], 'admin/') === 0;
}

/**
 * Checks whether current page is a content editing page.
 *
 * @return boolean
 */
function special_replace_is_content_edit_page() {  
  return preg_match('#^node/[0-9]+/edit#', $_GET['q']);
}

/**
 * Perform a selection in the database to return the whole special_replace table.
 *
 * @return
 *  Object with resulting query.
 */
function special_replace_get_all_patterns() {
  return db_select('special_replace', 'sr')
          ->fields('sr')
          ->orderBy('sr.weight')
          ->execute();
}

/**
 * Apply all replace patterns in the body of the HTML.
 */
function _special_replace_body_content($children, $elements) {
  if (preg_match('/<body[^>]*>(.+)<\/body>/si', $children, $matches)) {
    $replaced_text = special_replace_apply_replacements($matches[1]); 
    
    if ($replaced_text) {
      $children = str_replace(
        $matches[1],
        $replaced_text,
        $children
      );
    }
  }

  return $children;
}

/**
 * Apply all active replacement patterns to $text.
 *
 * @param string
 *  Text to be replaced.
 *
 * @return
 *  Replaced string. NULL if no patterns were matched.
 */
function special_replace_apply_replacements($text) {
  $patterns = _special_replace_get_patterns_and_replacements();
  return preg_replace($patterns['patterns'], $patterns['replacements'], $text); 
}

/**
 * Return an array with all active patterns and replacements.
 *
 * @return array
 */
function _special_replace_get_patterns_and_replacements() {
  module_load_include('inc', 'special_replace', 'special_replace.common');
  $special_replaces = special_replace_get_all_patterns();
  
  $patterns = array();
  $replacements = array();
  foreach ($special_replaces as $special_replace) {
    if ($special_replace->active) {
      $patterns[] = $special_replace->pattern;
      $replacements[] = $special_replace->replacement;
    }
  }
  
  return array(
    'patterns' => $patterns,
    'replacements' => $replacements,
  );
}

/**
 * Apply all active pattern replacements to $text. Return unchanged $text if no matches are found.
 *
 * @param string $text
 */
function _special_replace_replace_string($text) {
  $replaced_text = special_replace_apply_replacements($text);
  if ($replaced_text) {
    $text = $replaced_text;
  }
  
  return $text;
}

/**
 * Replace all string inside an array recursively.
 *
 * @param array &$tree
 */
function _special_replace_recursive(&$tree) {
  foreach ($tree as $key => $item) {
    if (is_string($item)) {
      $tree[$key] = _special_replace_replace_string($item);
    }
    elseif (is_array($item)) {
      _special_replace_recursive($item);
      $tree[$key] = $item;
    }
  }
}