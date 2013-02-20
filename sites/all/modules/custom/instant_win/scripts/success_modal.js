(function($) {

  $(function() {
    new successModal();
  });
  
  var successModal = function() {
    var getModalStructure = function() {
          return $('<div></div>')
                  .attr('id', 'registration-success-message')
                  .append(Drupal.settings.instant_win.successMessage)
                  .append(getCloseButton());
        },
        
        getCloseButton = function() {
          var buttonText = Drupal.t('Close');
          
          return $('<a></a>')
                  .attr('id', 'registration-success-message-close')
                  .attr('title', buttonText)
                  .append(buttonText)
                  .click(function() {
                    dialog.dialog('close');
                  });
        },
        
        dialog = getModalStructure()
                  .dialog({
                    id: 'registration-success-message',
                    modal: true,
                    resizable: false,
                    draggable: false,
                    width: 746,
                    height: 500
                  });
    
    return dialog;
  }
  

})(jQuery);