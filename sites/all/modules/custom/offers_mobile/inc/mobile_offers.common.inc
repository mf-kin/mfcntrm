<?php
function ajax_load_products_callback($form, $form_state) {   
  $product_nodes = get_nodes_from_node_type($form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type']);
  
  $form['content_types_values']['content_type_contents_prefix'] = array(
    '#markup' => '<div id="content-type-list">',
  );
  
  foreach ($product_nodes as $nid => $product) {
    $form['content_types_values'][MOBILE_OFFERS_PREFIX . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '_' . $nid] = array(
      '#type' => 'textfield',
      '#title' => $product->title . t(' Form ID'),
      '#attributes' => array(
        'id' => array('edit-mobile-offers-' . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '-' .$nid),
        'name' => MOBILE_OFFERS_PREFIX . $form_state['values'][MOBILE_OFFERS_PREFIX . 'product_content_type'] . '_' . $nid,
      ),
      '#required' => FALSE
    );
  }
  
  $form['content_types_values']['content_type_contents_suffix'] = array(
    '#markup' => '</div>',
  );
  
  return $form['content_types_values'];
}

function delete_mobile_offers_related_variables() {
  // Delete all variables related to this Content Type
  $node_type = $_GET['change_ct'];
  $product_nodes = get_nodes_from_node_type($node_type);
  
  foreach ($product_nodes as $nid => $product) {
    $variable_delete = variable_get(MOBILE_OFFERS_PREFIX . $node_type . '_' . $nid);
    if(isset($variable_delete)) {
      variable_del(MOBILE_OFFERS_PREFIX . $node_type . '_' . $nid);
    }
    $variable_delete_alias = variable_get(MOBILE_OFFERS_PREFIX . $node_type . '_' . $nid . '_alias');
    if(isset($variable_delete_alias)) {
      variable_del(MOBILE_OFFERS_PREFIX . $node_type . '_' . $nid . '_alias');
    }
  }

  variable_del(MOBILE_OFFERS_PREFIX . 'product_content_type');
  variable_del(MOBILE_OFFERS_PREFIX . 'product_content_type_fixed');
}

function get_nodes_from_node_type($node_type) {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $node_type)
	->propertyCondition('status', 1)
    ->execute();        
  $product_nodes = node_load_multiple(array_keys($entities['node']));
  
  return $product_nodes;
}
