/* SiteCatalyst code version: H.24.1.
Copyright 1996-2012 Adobe, Inc. All Rights Reserved
More info available at http://www.omniture.com */
var omnitureInfo = Drupal.settings.omniture;

var s_account = omnitureInfo.sAccount;
var s = s_gi(s_account);

//@CONFIG_SECTION_PLACEHOLDER@

/* WARNING: Changing any of the below variables will cause drastic
changes to how your visitor data is collected.  Changes should only be
made when instructed to do so by your account manager.*/
s.visitorNamespace = omnitureInfo.visitorNamespace;
s.dc = omnitureInfo.dc;

if (jQuery.trim(omnitureInfo.trackingServer) != '') {
  s.trackingServer = omnitureInfo.trackingServer;
}
if (jQuery.trim(omnitureInfo.trackingServerSecure) != '') {
  s.trackingServerSecure = omnitureInfo.trackingServerSecure;
}

;(function($) {
  
  (function(window, undefined) {
   
    var omniture = (function() {
    
      var omniture = function(){};
      
      omniture.prototype = {
        setData: function(variables, events) {
          var omnitureData = function(){};
          
          if (typeof(variables) != 'undefined') {
            $.each(variables, function(index, element) {
              if (index != '' && element != '') {
                s.linkTrackVars = s.apl(s.linkTrackVars, index, ',', 2);
                
                var currentElementData = element;
                $.each(index.split(','), function(index, element) {
                  omnitureData.prototype[element] = currentElementData;
                });
              }
            });
          }
          
          if (typeof(events) != 'undefined') {
            s.linkTrackVars = s.apl(s.linkTrackVars, 'events', ',', 2);
            
            $.each(events, function(index, element) {
              if (element != '') {
                s.linkTrackEvents = s.apl(s.linkTrackEvents, element, ',', 2);
                s.events = s.apl(s.events, element, ',', 2);
              }
            });
          }
          
          $.extend(s, new omnitureData());
        },
      
        sendData: function(omnitureData, stringValue) {
          this.resetTrackingVariables();
          
          this.setData(omnitureData.variables, omnitureData.events);
          s.tl(true, 'o', stringValue || 'Custom tracking');
          
          this.resetTrackingVariables();
        },
        
        resetTrackingVariables: function() {
          s.linkTrackVars = 'None';
          s.linkTrackEvents = 'None';
          s.pageName = '';
        },
        
        getVisitorStatus: function() {
          var visitor_status = getCookie('visitor_status');
          if (visitor_status == '') {
            setCookie('visitor_status', 'repeat', 365);
            visitor_status = 'new';
          }
          else {
            visitor_status = 'repeat';
          }
          
          return visitor_status;
        },
        
        getButtonTitle: function(button) {
          var buttonTitle;
          if (button.find('img').length) {
            buttonTitle = button.find('img').attr('alt');
          }
          else {
            buttonTitle = button.attr('title');
          }
          
          if (!buttonTitle.length) {
            buttonTitle = button.text();
          }
          
          return $.trim(buttonTitle);
        }

      };
      
      $.extend(this, omnitureInfo);
   
      return new omniture();
    })();

    window.omniture = omniture;

  })(window);

  var omniture = window.omniture;
  
  $(function() {
  
//@CONFIGURABLE_JS_TRACKING_CODE_PLACEHOLDER@
  
    // write s_code to page;
    var s_code = s.t();
    if (s_code) {
      document.write(s_code);
    }
    
    // reset variables for next trackings
    omniture.resetTrackingVariables();
  });
  

//@GENERAL_SELECTORS_CODE_PLACEHOLDER@

})(jQuery);

//@GLOBAL_SCOPE_CONFIGURABLE_JS_TRACKING_CODE_PLACEHOLDER@

// -------------------------------------------------------
// ------------------------------------ Helper functions -
// -------------------------------------------------------
// function to set a cookie (from w3schools)
function setCookie(c_name, value, expiredays) {
  var exdate = new Date();
  exdate.setDate(exdate.getDate() + expiredays);
  document.cookie = c_name +"="+ escape(value) + ((expiredays == null) ? "" : ";expires="+ exdate.toUTCString());
}

// function to get a cookie (from w3schools)
function getCookie(c_name) {
  if (document.cookie.length > 0) {
    c_start = document.cookie.indexOf(c_name +"=");
    if (c_start != -1) {
      c_start = c_start + c_name.length + 1;
      c_end = document.cookie.indexOf(";", c_start);
      
      if (c_end == -1) {
        c_end=document.cookie.length;
      }
      
      return unescape(document.cookie.substring(c_start, c_end));
    }
  }
  
  return "";
}
// -------------------------------------------------------

//@PLUGINS_SECTION_PLACEHOLDER@

//@MODULES_SECTION_PLACEHOLDER@

//@S_CODE_CORE_PLACEHOLDER@
