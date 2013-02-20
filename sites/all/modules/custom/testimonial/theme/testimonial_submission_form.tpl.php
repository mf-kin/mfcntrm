<?php if (isset($form['error_messages'])): ?>
  <?php print drupal_render($form['error_messages']); ?>
<?php endif; ?>

<?php print drupal_render($form['required_fields']); ?>
<div class="left-column">
  <?php print drupal_render($form['name']); ?>
  <?php print drupal_render($form['email']); ?>
  <?php print drupal_render($form['email_confirm']); ?>
  <?php print drupal_render($form['title']); ?>
  <?php print drupal_render($form['product']); ?>
</div>
<div class="right-column">
  <?php print drupal_render($form['body']); ?>
  <p class="disclaimer"><?php print drupal_render($form['disclaimer']); ?></p>
</div>
<div class="agreement clear_both">
  <h3><?php print drupal_render($form['release_agreement_title']); ?></h3>
  <?php print drupal_render($form['release_agreement_body']); ?>
  <div class="options">
    <?php print drupal_render($form['agreement']); ?>
  </div>
</div>
<?php
  // Always print out the entire $form. This renders the remaining pieces of the
  // form that haven't yet been rendered above.
  print drupal_render_children($form);
  