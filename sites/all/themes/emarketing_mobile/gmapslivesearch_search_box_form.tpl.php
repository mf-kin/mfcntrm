<div id="gmapslivesearch-box-wrapper">
  <div class="gmapslivesearch-box find-store">
    <?php $error_messages = drupal_get_messages('error'); ?>
    <?php if(!empty($error_messages['error'])): ?>
      <div class="error-messages-find-store messages error">
      <?php foreach($error_messages['error'] as $key => $error): ?>
        <p><?php print $error; ?></p>
      <?php endforeach; ?>
      </div>
    <?php endif; ?>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]); ?>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]); ?>
    <a id="google-maps-live-search-go-button" href="javascript:void(0);" title="<?php print variable_get('gmapslivesearch_field_buy_online_btn_page', 'Go'); ?>"><?php print variable_get('gmapslivesearch_field_buy_online_btn_page', 'Go'); ?></a>
    <?php print drupal_render($form['submit']); ?>
    <a href="javascript:void(0);" id="gmapslivesearch-btn-map" class="map-btn">
      <?php print t('Map');?>
    </a>
    <a href="javascript:void(0);" id="gmapslivesearch-btn-list" class="list-btn">
      <?php print t('List');?>
    </a>
  </div>
</div>
<?php print drupal_render($form['form_build_id']); ?>
<?php print drupal_render($form['form_id']); ?>
