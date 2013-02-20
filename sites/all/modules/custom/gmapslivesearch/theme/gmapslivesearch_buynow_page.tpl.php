<div id="buy-now-page">
  <div id="buy-now-page-header">
    <h1><?php print variable_get('gmapslivesearch_where_to_buy_page_title'); ?></h1>
    <h3><?php print variable_get('gmapslivesearch_where_to_buy_page_subtitle'); ?></h3>
  </div>
  <div id="buy-now-content">
    <!-- Search box container -->
    <?php print $search_box; ?>
    <!-- Results box container -->
    <?php print $results_box; ?>
  </div>
</div>