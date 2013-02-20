<div class="coupon-thank-page">
  <h1><?php print t('Thank you for your interest.');?></h1>
  <iframe src="<?php print $node->generation_path; ?>"
          name="iframecouponimage"
          id="iframe-coupon-image"
          frameborder="0"
          scrolling="no"
          width="1"
          height="1"
          class="coupon-image-code">
  </iframe>  
  <div id="loader-print-button"></div>
  <input type="button" id="coupon-print-button" style="display:none;" value="<?php print t('Print Coupon'); ?>" class="print-coupon-button" alt="<?php print t('Print Coupon'); ?>" title="<?php print t('Print Coupon'); ?>"/>
</div>