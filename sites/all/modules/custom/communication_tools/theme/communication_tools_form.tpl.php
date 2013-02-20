<?php if (isset ($form['communication_tools_content_title'])) : ?>
  <p><?php print drupal_render($form['communication_tools_content_title']); ?></p>
<?php endif; ?>

<?php print drupal_render($form['name']); ?>

<label class="require-label"><span class="form-required">*</span> <?php print t('Required fields'); ?></label>
<fieldset>
    <?php print drupal_render($form['communication_tools_name']); ?>
    <?php print drupal_render($form['communication_tools_email_yours']); ?>
</fieldset>

<fieldset>
    <?php print drupal_render($form['communication_tools_friends_name']); ?>
    <?php print drupal_render($form['communication_tools_email_friend']); ?>
</fieldset>

<?php print drupal_render($form['form_build_id']) ?>
<?php print drupal_render($form['form_id']) ?>

<div class="container-buttons clearfix">
    <?php print drupal_render($form['communication_tools_cancel_button']); ?>
    <div id="communication-tools-confirm-button">
        <?php print drupal_render($form['communication_tools_submit_button']); ?>
    </div>
</div>

