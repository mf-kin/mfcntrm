(function($) {
  function communicationToolsAttach(context) {
    $('.communication-tools-share-link', context).click(function(e){
      e.preventDefault();
    
      $('#communication_tools_overlay_iframe').remove();
      $('#communication_tools_overlay').remove();

      appendOverlayToBody();

      $('#communication_tools_overlay').dialog({
        title: Drupal.t('Email to a Friend'),
        draggable: false,
        modal: true,
        resizable: false,
        height: 380,
        width: 617,
        dialogClass: 'send_to_a_friend',
        create: function(event, ui) {
          var src = $('.communication-tools-share-link').attr('href');
          var iframe = getIFrameHTML(src);
          $('#communication_tools_overlay').append(iframe);

          var preloaderImageSrc = Drupal.settings.communication_tools_preloader_image_path;
          $('div.ui-dialog.send_to_a_friend').append('<img id="communication_tools_preloader" src="' + preloaderImageSrc + '">');
          
          Drupal.attachBehaviors($(event.target).parents().filter('.ui-dialog.send_to_a_friend'));
          
          $(iframe).load(function(){
              if ($.browser.webkit) { //FIX FOR CHROME/SAFARI (NEEDS TO BE RESIZED TWO TIMES)
                  autoRezisePopup();
              } 
              autoRezisePopup();
          });
          //$(event.target).css({display: 'none'});
        },
        close: function(event, ui) { 
          // fix for scrollbars in IE
          if ( $.browser.msie ) {
            $('body').css('overflow', 'auto');
          }
        }
      });

      return false;
    });
  }

  function autoRezisePopup() {
    var iframe = $('#communication_tools_overlay').find('iframe').get(0);
    var actualHeight = $(iframe).contents().find('body').outerHeight(true);
    var pos_top = $(iframe).position().top;
    
    $(iframe).height(actualHeight).parent().height(actualHeight+pos_top);
  }
  
  function appendOverlayToBody() {
    $('body').prepend(getOverlayHTML());
  }

  function getOverlayHTML() {
    return '<div id="communication_tools_overlay"></div>';
  }
  
  function getIFrameHTML(src) {
    var iframe = $('<iframe></iframe>')
      .attr({
        'id'                : 'communication_tools_overlay_iframe',
        'noresize'          : 'noresize',
        'frameborder'       : '0',
        'border'            : '0',
        'cellspacing'       : '0',
        'scrolling'         : 'no',
        'marginwidth'       : '0',
        'marginheight'      : '0',
        'allowtransparency' : true,
        'src'               : src
      })
      .css({
        'display' : 'none'
      })
      .load(function() {
        $('img#communication_tools_preloader').remove();
        $('#communication_tools_overlay').css({
          height: 'auto', padding: 0
        });

        $('#communication_tools_overlay_iframe').css({
          width: '100%',
          padding: '0',
          display: 'block'
        });
      }
    );
    
    return iframe;
  }


  Drupal.behaviors.communicationTools = {
    attach: function(context){
      communicationToolsAttach(context);
    }
  }

  function communicationToolsCloseOverlay(){
    parent.jQuery('a.ui-dialog-titlebar-close').click();
    return false;
  }

  function communicationToolsIframreAttach(context) {
    $('#edit-communication-tools-cancel-button').click(function(index){
      communicationToolsCloseOverlay();
    });
  }

  Drupal.behaviors.communicationToolsIframe = {
    attach: function(context){
      communicationToolsIframreAttach(context);
    }
  }

  function communicationToolsAttachCloseOverlay(){
    $(".com_tls_exit_overlay").click(function(){
      communicationToolsCloseOverlay();
    });
  }

  Drupal.behaviors.com_msg_close_overlay = {
    attach: function(){
      communicationToolsAttachCloseOverlay();
    }
  }
  
  $(document).ready(function(){
    if ($('a#communication_tools_print_link').length > 0) {
      $('a#communication_tools_print_link').click(function() {
        var printProperties = {};
        if (Drupal.settings.communication_tools.site_logo_print_path != '') {
          printProperties.logo = Drupal.settings.communication_tools.site_logo_print_path;
        }
        
        $('div#content').printElement(printProperties);
        return false;
      });
    }
  });
  
})(jQuery);
