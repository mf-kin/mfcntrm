<div id="gmapslivesearch-box-wrapper">
  <div class="gmapslivesearch-box find-store">
    <h3><?php print variable_get('gmapslivesearch_where_to_buy_page_address_field_label'); ?></h3>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_ADDRESS_TO_SEARCH]); ?>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_MILESSEARCH_WITHIN]); ?>
    <?php print variable_get('gmapslivesearch_where_to_buy_page_address_search_desc'); ?>
    <?php if (isset($form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE])): ?>
      <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_STORES_CONTENT_TYPE]); ?>
    <?php endif; ?>
    <?php print drupal_render($form['submit']); ?>
  </div>
  <div class="gmapslivesearch-box buy-online">
    <h3><?php print variable_get('gmapslivesearch_where_to_buy_page_store_field_label'); ?></h3>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_STORES_SEARCH]); ?>
    <?php print variable_get('gmapslivesearch_where_to_buy_page_store_search_desc'); ?>
    <?php print drupal_render($form[GMAPSLIVESEARCH_FIELD_GOTO_ONLINE_STORE_BTN]); ?>
  </div>
  <div class="clear_both "></div>
</div>
<?php print drupal_render($form['form_build_id']); ?>
<?php print drupal_render($form['form_id']); ?>
