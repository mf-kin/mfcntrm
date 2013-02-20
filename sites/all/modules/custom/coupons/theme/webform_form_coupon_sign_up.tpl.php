<?php

/**
 * @file
 * Customize the display of a complete webform.
 *
 * This file may be renamed "webform-form-[nid].tpl.php" to target a specific
 * webform on your site. Or you can leave it "webform-form.tpl.php" to affect
 * all webforms on your site.
 *
 * Available variables:
 * - $form: The complete form array.
 * - $nid: The node ID of the Webform.
 *
 * The $form array contains two main pieces:
 * - $form['submitted']: The main content of the user-created form.
 * - $form['details']: Internal information stored by Webform.
 */
?>

<?php 
  // If editing or viewing submissions, display the navigation at the top.
  if (isset($form['submission_info']) || isset($form['navigation'])) {
    print drupal_render($form['navigation']);
    print drupal_render($form['submission_info']);
  }
?>

<div id="webform-cemhub-page">
  <div id="webform-cemhub-page-header">
    <?php print drupal_render($form['coupon_addtional_content']['header']); ?>
    <p class="requeried-field"><span title="This field is required." class="form-required"></span> Required</p>
  </div>
</div>
<div id="webform-cemhub">
  <div class="webform-left-fields">
    <?php print drupal_render($form['submitted']['field_first_name']); ?>
    <?php print drupal_render($form['submitted']['field_last_name']); ?>
    <?php print drupal_render($form['submitted']['field_email']); ?>
    <?php print drupal_render($form['submitted']['field_address_line_1']); ?>
    <?php print drupal_render($form['submitted']['field_address_line_2']); ?>
    <?php print drupal_render($form['submitted']['field_city']); ?>
  </div>
  
  <div class="webform-right-fields" >
    <?php print drupal_render($form['submitted']['field_state']); ?>
    <?php print drupal_render($form['submitted']['field_zip']); ?>
    <div class="pos-date">
      <?php print drupal_render($form['submitted']['field_dob_month']); ?>
      <?php print drupal_render($form['submitted']['field_dob_year']); ?>
    </div>
    <?php print drupal_render($form['submitted']['field_gender']); ?>
    <?php print drupal_render($form['submitted']['field_children_under_age']); ?>
    <div id="information-fields">
      <label><?php print t('Choose what information you\'d like to receive'); ?></label>
      <?php print drupal_render($form['submitted']['field_site_contact_agreement']); ?>
      <?php print drupal_render($form['submitted']['field_pfizer_contact_agreement']); ?>
      <?php print drupal_render($form['coupon_addtional_content']['bottom']); ?>
      <div class="clear_both"></div>
    </div>
  </div>
</div>
<?php
  $form['actions']['submit']['#attributes']['title'] = $form['actions']['submit']['#value'];
  // Always print out the entire $form. This renders the remaining pieces of the
  // form that haven't yet been rendered above.
  print drupal_render_children($form);
  