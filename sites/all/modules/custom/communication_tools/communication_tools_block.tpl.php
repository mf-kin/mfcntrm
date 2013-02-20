<div class="tool_box">
  <a id="communication_tools_share_link" class="communication-tools-share-link" href="<?php print $variables['share_page_url']; ?>" title="<?php print t('Share with a friend'); ?>"><?php print t('Share with a friend'); ?></a>
  <?php if($variables['show_facebook_icon']): ?>
    <span class="pipe"></span>
    <a id="communication_tools_facebook" href="<?php print($variables['facebook_fan_url']); ?>" title="<?php print t('Facebook page'); ?>"><?php print t('Facebook page'); ?></a>
  <?php endif; ?>
</div>
