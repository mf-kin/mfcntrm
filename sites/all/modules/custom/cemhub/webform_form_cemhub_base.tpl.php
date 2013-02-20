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
    <h1><?php print variable_get('cemhub_default_template_title'); ?></h1>
    <h3><?php print variable_get('cemhub_default_template_description'); ?></h3>
    <h2><?php print variable_get('cemhub_default_template_subtitle'); ?></h2>
    <p class="info"><span title="<?php print t('This field is required.'); ?>" class="form-required">*</span> <?php print t('Required Fields'); ?></p>
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
  	<?php print drupal_render($form['submitted']['field_dob_year']); ?>
      <?php print drupal_render($form['submitted']['field_dob_month']); ?>
    </div>
    <?php print drupal_render($form['submitted']['field_gender']); ?>
    <?php print drupal_render($form['submitted']['field_children_under_age']); ?>
  </div>
  
  <div class="webform-bottom-fields">
    <h2><?php print variable_get('cemhub_default_template_optins_subtitle'); ?></h2>
    <div class="webform-bottom-fields-left"><?php print drupal_render($form['submitted']['field_site_contact_agreement']); ?></div>
    <div class="webform-bottom-fields-right"><?php print drupal_render($form['submitted']['field_pfizer_contact_agreement']); ?></div>
    <p class="additional-info"><?php print variable_get('cemhub_default_template_disclaimer'); ?></p>
  </div>
</div>
<?php
  // Always print out the entire $form. This renders the remaining pieces of the
  // form that haven't yet been rendered above.
  print drupal_render_children($form);
  