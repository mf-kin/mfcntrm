<?php
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php $field_text =''; ?>
  <?php foreach ($fields as $id => $field): ?>
    <?php if ($field->class == "field-image-thumbnail-mobile"): ?>
      <?php $field_image = $field; ?>  
      <?php else: ?>

      <?php if ($field->class == 'view-node') : ?> 
        <?php $field_link = $field; ?>
      <?php else: ?>
        <?php if (!empty($field->separator)): ?>
          <?php $field_text .= $field->separator; ?>
        <?php endif; ?>
        <?php $field_text .= $field->wrapper_prefix; ?>
        <?php $field_text .= $field->label_html; ?>
        <?php $field_text .= $field->content; ?>
        <?php $field_text .= $field->wrapper_suffix; ?>
      <?php endif; ?>
    <?php endif; ?>
  <?php endforeach; ?>

  <div class="div-image-thumbnail">
      <?php if (!empty($field_image->separator)): ?>
          <?php print $field_image->separator; ?>
        <?php endif; ?>
      
        <?php print $field_image->wrapper_prefix; ?>
        <?php print $field_image->label_html; ?>
        <?php print $field_image->content; ?>
        <?php print $field_image->wrapper_suffix; ?>
      </div>
  <div class="field-title-subhead-link-buy-now">
  <?php print $field_text; ?>
  <?php $options_stores = (list_extract_allowed_values(variable_get(GMAPSLIVESEARCH_FIELD_STORES_SEARCH, ''), 'list_text', FALSE)); ?> 

<div id="button-buy-online"><span class="span-buy-online" style="display:none"><?php print t('BUY ONLINE'); ?></span>
  <select class = "buy-now buy-now-select-masked-button">
    <option  value="-1" ><?php print t('BUY ONLINE'); ?></option>
    <?php foreach ($options_stores as $key => $option): ?>
      <option value="<?php print $key ?>"><?php print $option ?></option>
    <?php endforeach; ?>
  </select>
 </div> 
  <a class="go-to-store-link" href="javascript:void(0);"></a>
  </div>
<div class="link-product">
  <?php if (!empty($field_link->separator)): ?>
    <?php print $field_link->separator; ?>
  <?php endif; ?>
  <?php print $field_link->wrapper_prefix; ?>
  <?php print $field_link->label_html; ?>
  <?php print $field_link->content; ?>
  <?php print $field_link->wrapper_suffix; ?>
</div>