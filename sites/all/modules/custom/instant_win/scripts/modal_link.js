(function($) {

  $(function() {
  
    $('div.modal-link').each(function() {
      var $this = $(this);
      var modalLink = $this.find('a'),
          modalId = $this.attr('id'),
          dialog = getIframeObject(modalLink.attr('href'), modalId)
            .dialog({ 
              autoOpen: false,
              modal: true,
              dialogClass: modalId + '-dialog-wrapper',
              resizable: false,
              draggable: false,
              width: 746,
              height: 500,
              close: function(event, ui) {
                resetIframeForm(modalId);
                
                return false;
              }
            });
            
            
      modalLink.click(function() {
        dialog.dialog('open');
        
        return false;
      });
      
      function getIframeObject(iframeSrc, modalId) {
        return $('<iframe></iframe>')
          .attr('name', modalId + '-iframe')
          .attr('id', modalId + '-iframe')
          .attr('src', iframeSrc)
          .attr('frameBorder', 0);
      }
      
      function resetIframeForm(modalId) {
      
        var form = $('#' + modalId + '-iframe')
          .contents()
          .find('#' + modalId + '-form');
        
        var successPageCloseButton = form.find('#' + modalId + '-success-page-close-button');
        
        if (successPageCloseButton.length) {
          successPageCloseButton.click();
        }
        else {
          form.find('div.messages').remove();
          form.find('input:text').val('');
        }
      }
      
    });

  });
  
})(jQuery);
