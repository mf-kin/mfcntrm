<?php
$lang = LANGUAGE_NONE;

if (isset($node->language)) {
  $lang = $node->language;
}
$count_tabs = 1;
global $base_url;?>

<div id="product-detail">
  <?php for ($i = 0; $i < $count_tabs; $i++): ?>
    <div class="product-detail-content">
      <div class="product-images">
      <?php if (isset($node->field_product_image_mobile[$lang][$i])): ?>
        <?php $node->field_product_image_mobile[$lang][$i]['path'] = $node->field_product_image_mobile[$lang][$i]['uri']; ?>
        <?php print theme('image', $node->field_product_image_mobile[$lang][$i]); ?>
      <?php endif; ?>
      </div>

      <div class="product-description">
        <div class="product-description-title">
        <?php if (isset($node->title)): ?>
          <h2><?php print $node->title ?></h2>
            <?php endif; ?>
        </div>
        <div class="product-description-subhead">
        <?php if (isset($node->field_details_subhead_mobile[$lang][$i]['value'])): ?>
          <?php print $node->field_details_subhead_mobile[$lang][$i]['value'] ?>
        <?php endif; ?>
        </div>
        <div class="product-description-body">
        <?php if (isset($node->	field_description_mobile[$lang][$i]['value'])): ?>
          <?php print $node-> field_description_mobile[$lang][$i]['value'] ?>
        <?php endif; ?>
        </div>
      </div>

      <div class="product-button-detail">
        <a class="get-coupon-offers" href="<?php print $base_url?>/offers"><?php print t('GET A COUPON'); ?></a>
        <?php $options_stores = (list_extract_allowed_values(variable_get(GMAPSLIVESEARCH_FIELD_STORES_SEARCH, ''), 'list_text', FALSE)); ?>
        <span class="span-buy-online" style="display:none"><?php print t('BUY ONLINE'); ?></span>
        <select class = "buy-now buy-now-select-masked-button">
          <option value="-1" ><?php print t('BUY ONLINE'); ?></option>
          <?php foreach ($options_stores as $key => $option): ?>
            <option value="<?php print $key ?>"><?php print $option ?></option>
          <?php endforeach; ?>
        </select>
        <a class="go-to-store-link" href="#"></a>
      </div>

      <div class="product-dosing-information">
      <?php if (isset($node-> field_dosing_information_mobile[$lang][$i]['value'])): ?>
        <?php print $node->	field_dosing_information_mobile[$lang][$i]['value'] ?>
      <?php endif; ?>
      </div>

      <div class="product-dosing-information-table">
      <?php if (isset($node->field_information_table_mobile[$lang][$i]['value'])): ?>
        <?php print $node->field_information_table_mobile[$lang][$i]['value'] ?>
      <?php endif; ?>
      </div>
      
      <div class="product-labeling">
        <div class="product-labeling-expand">
          <h2><?php print t('View Product Labeling'); ?></h2>
          <a class="open-labeling"> <span class="general-sprite"> </span> </a>
        </div>
        <div class="content-labeling" style="display:none">
        <?php if (isset($node->field_product_labeling_mobile[$lang][$i]['value'])): ?>
          <?php print $node->field_product_labeling_mobile[$lang][$i]['value'] ?>
        <?php endif; ?>
        </div>
      </div>
    </div>
  <?php endfor; ?>
</div>