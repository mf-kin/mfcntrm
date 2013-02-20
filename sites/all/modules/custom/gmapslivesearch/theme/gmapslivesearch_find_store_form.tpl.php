<div>
  <div class="find-store-box-wrapper">
    <h2><?php print variable_get('gmapslivesearch_buy_online_store_address_field_label'); ?></h2>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH_FIND_ONLINE]); ?>
    <?php print variable_get('gmapslivesearch_buy_online_address_search_desc'); ?>
    <?php if (isset($form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE])): ?>
      <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE]); ?>
    <?php endif; ?>
    <?php print drupal_render($form['submit']); ?>
  </div>
  <div class="buy-online-box-wrapper">     
    <h2><?php print variable_get('gmapslivesearch_buy_online_store_search_field_label'); ?></h2>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH_FIND_ONLINE]); ?>
    <?php print variable_get('gmapslivesearch_buy_online_store_search_desc'); ?>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN_FIND_ONLINE]); ?>
  </div>
</div>
<?php print drupal_render($form['form_build_id']); ?>
<?php print drupal_render($form['form_id']); ?>
