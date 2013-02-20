//PUT ALL THE JS FUNCTIONS, AND CODES INSIDE THE (function ($) {* INSIDE HERE *})(jQuery);
(function ($) {
  $(document).ready(function() {
    treatOffersEmailField();
    treatSubmitButton();
  });

  function treatOffersEmailField() {
    $('#edit-email-value').focus(function() {
      if(Drupal.settings.mobileOffersEmailValue != undefined && Drupal.settings.mobileOffersEmailValue == $('#edit-email-value').val()) {
        $('#edit-email-value').val('');
      }
    });

    $('#edit-email-value').blur(function() {
      if(Drupal.settings.mobileOffersEmailValue != undefined && $('#edit-email-value').val() == '') {
        $('#edit-email-value').val(Drupal.settings.mobileOffersEmailValue);
      }
    });
  }

  function treatSubmitButton() {

    //Makes the div(thank-you-alert) look like an alert field.
    $( ".thank-you-alert" ).dialog({
      resizable: false,
      height: 100,
      draggable: false,
      modal: true,
    });
    //Close 'button' that closes the div and redirects to the home page
    $('.thank-you-alert-button').click(function() {
      $(".thank-you-alert").dialog('close');
      window.location = document.location.href="/";
    });
  }
})(jQuery);