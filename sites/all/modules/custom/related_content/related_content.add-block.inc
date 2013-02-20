<?php
/**
 * @file
 * Related content module block
 */

function related_content_add_block_form($form, &$form_state) {
  $form = related_content_get_fields($form_state);
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

function related_content_add_block_form_submit($form, &$form_state) {

  db_insert('related_content_blocks')->fields(array(
    'id' => $form_state['values']['id'],
    'max_items' => $form_state['values']['max_items'],
    'vid' => $form_state['values']['vid'],
  ))->execute();
  
  $content_type_query = db_insert('related_content_types')->fields(array('id', 'type', 'link_text'));
  foreach ($form_state['values']['content_type'] as $content_type) {
    if ($content_type) {
      if(isset($form_state['values']['link_text_'. $content_type]) 
              && $form_state['values']['link_text_'. $content_type] != '') {
        $text_link = $form_state['values']['link_text_'. $content_type];
      } else {
        $text_link = NULL;
      }
      $content_type_query->values(array($form_state['values']['id'], $content_type, $text_link));
      $content_type_query->execute();
    }
  }
  
  $vocabularies_query = db_insert('related_content_exclude_terms')->fields(array('id', 'tid'));
  foreach ($form_state['values']['exclude_terms'] as $tid) {
    if ($tid) {
      $vocabularies_query->values(array($form_state['values']['id'], $tid));
      $vocabularies_query->execute();
    }
  }
  
  $fields_query = db_insert('related_content_fields')->fields(array('id', 'field'));
  foreach ($form_state['values']['fields'] as $field) {
    if ($field) {
      $fields_query->values(array($form_state['values']['id'], $field));
      $fields_query->execute();
    }
  }
  
  drupal_set_message(t('Block created!'));
}