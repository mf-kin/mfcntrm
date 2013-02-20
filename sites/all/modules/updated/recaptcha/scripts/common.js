(function($) {

  // This is done to remove default onclick action for Recaptcha help button
  // This is done to avoid that a window.open is performed, cause it ignores
  // the ext link validation
  $(document).ready(function() {
    var help_link = document.getElementById('recaptcha_whatsthis_btn');
    help_link.onclick = null;
  });
})(jQuery);