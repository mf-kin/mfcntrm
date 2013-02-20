<?php
/**
 * @file
 * Testimonial content admin workflows
 */

/**
 * Form builder. Creates the list of Testimonials pending review.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function testimonial_publish_form() {
  
  $form = array();
  
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => testimonial_publish_form_table_header(),
    '#options' => testimonial_publish_form_table_rows(),
    '#multiple' => TRUE,
    '#empty' => t('No pending testimonials'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#name' => 'publish',
    '#value' => t('Publish selected testimonials'),
  );
  
  $form['export'] = array(
    '#type' => 'submit',
    '#name' => 'export',
    '#value' => t('Export selected testimonials'),
  );
  
  $form['delete'] = array(
    '#type' => 'submit',
    '#name' => 'delete',
    '#value' => t('Delete selected testimonials'),    
  );
  
  $form['#submit'][] = 'testimonial_publish_form_submit';
  
  return $form;
}

/**
 * Get table header for Testimonial admin screen.
 */
function testimonial_publish_form_table_header() {
  return array(
    'id'    => t('Story ID'),
    'title' => t('Story Title'),
    'name'  => t('First Name'),
    'date'  => t('Date'),
  );
}

/**
 * List of all unpublished testimonials.
 * @return array Unpublished testimonials. 
 */
function testimonial_publish_form_table_rows() {
  $testimonials = array();
  
  $contents = array();
  $testimonials = testimonial_get_unpublished_testimonials();
  foreach ($testimonials as $testimonial) {
    $testimonials[$testimonial['id']] = array(
      'id' => $testimonial['id'],
      'title' => l(check_plain($testimonial['story_title']), 'node/' . $testimonial['id']),
      'name' => check_plain($testimonial['publisher_name']),
      'date' => $testimonial['created'],  
    );
  }
  return $testimonials;
}

/**
 * Handles the Testimonials' admin form submission
 * Publishes, exports or delete Testimonial nodes based on user's option
 */
function testimonial_publish_form_submit($form, &$form_state) {
  $selected_nodes = array_unique(array_values($form_state['values']['table']));
  if (FALSE !== ($zero = array_search(0, $selected_nodes))) {
    array_splice($selected_nodes, $zero, 1);
  }
  
  switch ($form_state['clicked_button']['#name']) {
    case 'publish':
      testimonial_publish_testimonials($selected_nodes);
      break;
    case 'delete':
      if (!empty($selected_nodes)) {
        $_SESSION['testimonial_data'] = $selected_nodes;
        $form_state['redirect'] = 'admin/config/testimonial/delete-confirm/';
      }
      else {
        drupal_set_message(t('You must select at least one content to delete!'), 'warning');
      }
      break;
    case 'export': 
      if (!empty($selected_nodes)) {
        testimonial_export_testimonials($selected_nodes);
      }
      else {
        drupal_set_message(t('You must select at least one content to export!'), 'warning');
      }
      break;
  }
}

/**
 * Publishes the selected testimonials.
 */
function testimonial_publish_testimonials($nids) {
  foreach ($nids as $nid) {
    $node = node_load($nid);
    $node->status = 1;
    node_save($node);
  }
  drupal_set_message(t('The selected testimonials were published successfully.'));
}

/**
 * Generates the testimonial's delete confirmation page.
 */
function testimonial_delete_confirmation_form() {
  
  if (!isset($_SESSION['testimonial_data'])) {
    testimonial_delete_confirmation_cancel();
  }
  
  $form = array();
  
  $form['#prefix'] = t('Are you sure you want to delete the following testimonials?');
  
  $testimonials = testimonial_get_unpublished_testimonials($_SESSION['testimonial_data']);
  
  $nodes_html = '';
  foreach ($testimonials as $item) {
    $nodes_html .= '<p><b>' . $item['id'] . ':</b> ' . $item['story_title'] . '</p>';
  }
  
  $form['nodes'] = array(
    '#markup' => $nodes_html,
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('testimonial_delete_confirmation_cancel'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete testimonials'),
  );
  
  $form['#submit'][] = 'testimonial_delete_confirmation_submit';
  
  return $form;
}

/*
 * Send the user back to the testimonial review form.
 */
function testimonial_delete_confirmation_cancel() {
  unset($_SESSION['testimonial_data']);
  drupal_goto('admin/config/testimonial/review');
}

/**
 * Deletes the selected testimonials nodes
 */
function testimonial_delete_confirmation_submit($form, &$form_state) {
  if (!isset($_SESSION['testimonial_data'])) {
    testimonial_delete_confirmation_cancel();
  }
  $form_state['redirect'] = 'admin/config/testimonial/review';
  node_delete_multiple($_SESSION['testimonial_data']);
  drupal_set_message(t('The selected testimonials were deleted successfully.'));
  unset($_SESSION['testimonial_data']);
}

/**
 * Exports unpublished testimonials (by specific nid or from yesterday) to PDF
 */
function testimonial_export_testimonials($nids, $download = TRUE, $cron_running = FALSE, $filter_date_start = NULL, $filter_date_end = NULL) {
  
  $html = variable_get('testimonial_generated_pdf_title', '') . '<br /><br />';
  
  $result = testimonial_get_unpublished_testimonials($nids, $filter_date_start, $filter_date_end); 
  
  if (!empty($result)) {
    foreach ($result as $item) {
      $html .= testimonial_get_formatted_pdf_row($item);
    }
  }
  else {
    $html .= t('<h3>No entries have been submitted within the past 24 hours</h3>');
  }
  
  
  module_load_include('php', 'testimonial', 'lib/MPDF53/mpdf');
  $mpdf = new mPDF();
  $mpdf->WriteHTML(testimonial_get_pdf_css(), 1);
  $mpdf->WriteHTML($html);
  
  if ($download) {
    //In case cron is running the file generated is saved inside testimonials module
    if ($cron_running) {
      $mpdf->Output( drupal_get_path('module', 'testimonial') . '/testimonials.pdf', 'F');
    }
    //Otherwise print the outpu file for the user on the browser.
    else {
      $mpdf->Output('testimonials.pdf', 'D');
      drupal_exit();  
    }
  }
  else {
    return $mpdf->Output('', 'S');
  }
  
}

/**
 * Get CSS file content for generated PDF.
 */
function testimonial_get_pdf_css() {
  return file_get_contents(drupal_get_path('module', 'testimonial') . '/theme/testimonial_pdf.css');
}

/**
 * Format testimonial for generated pdf
 * @return string HTML content for PDF
 */
function testimonial_get_formatted_pdf_row($item) {
  $html = '
      <table>
        <tr class="even">
          <td width="15%"><b>ID<b></td>
          <td>' . $item['id'] . '</td>
        </tr>
        <tr>
          <td><b>Date<b></td>
          <td>' . $item['created'] . '</td>
        </tr>
        <tr class="even">
          <td><b>Product<b></td>
          <td>' . $item['product']['product_title'] . '</td>
        </tr>
        <tr>
          <td><b>First Name<b></td>
          <td>' . $item['publisher_name'] . '</td>
        </tr>
        <tr class="even">
          <td><b>E-mail<b></td>
          <td>' . $item['publisher_email'] . '</td>
        </tr>
        <tr>
          <td><b>Story Title<b></td>
          <td>' . $item['story_title'] . '</td>
        </tr>
      </table>
        <div class="story-wrapper">
          <div class="story-label"><b>Story</b></div>
          <div class="story-body">' . nl2br($item['story_body']) . '</div>
          <div class="clear-both"></div>
        </div>
      <hr />';
  return $html;
}

/**
 * Get from database all testimonials in unpublished state.
 * @param array $nids
 * @return array List of testimonials unpublished 
 */
function testimonial_get_unpublished_testimonials($nids = 'all', $filter_date_start = NULL, $filter_date_end = NULL) {
  $result = db_select('node', 'n')
                   ->fields('n')
                   ->condition('n.type', 'testimonial', '=')
                   ->condition('n.status', 0, '=');
  if ($nids != 'all' && is_array($nids)) {
    $result->condition('n.nid', $nids, 'in');
  }      
  
  if (!empty($filter_date_start)) {
    $result->condition('n.created', $filter_date_start, '>=');
  }
  
  if (!empty($filter_date_end)) {
    $result->condition('n.created', $filter_date_end, '<');
  }
  
  $result = $result->execute();
  
  $testimonials = array();
  foreach ($result as $item) {
    $node = node_load($item->nid);
    if ($node) {
	  //var_dump($node);
	  //die();
	  if (!empty($node->field_testimonial_prod_nid['und'][0]['nid'])) {
        $product = node_load($node->field_testimonial_prod_nid['und'][0]['nid']);
        if ($product) {
          $product = array(
            'id' => $product->nid,  
            'product_title' => $product->title,
          );
        }
       
        $testimonials[$node->nid] = array(
          'id' => $node->nid,
          'publisher_name' => $node->field_testimonial_firstname['und'][0]['value'],
          'publisher_email' => $node->field_testimonial_email['und'][0]['value'],  
          'story_title' => $node->title,
          'story_body' => $node->body['und'][0]['value'],
          'created' => format_date($node->created),
          'product' => $product,
        );
      }
    }
  }
  
  return $testimonials;
}