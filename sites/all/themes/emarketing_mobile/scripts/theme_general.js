//PUT ALL THE JS FUNCTIONS, AND CODES INSIDE THE (function ($) {* INSIDE HERE *})(jQuery);
(function ($) {
  var myScroll;
  Drupal.platform = new Object();
  $(document).ready(function() {
    Drupal.platform.themeGeneralInit();
    Drupal.platform.treatMobileDevice();
    Drupal.platform.treatExpandedMenuBehaviors();
    Drupal.platform.treatMenuNavigationBehaviors();
    Drupal.platform.treatFakeActiveClass();
    Drupal.platform.treatCarouselHandler();
    Drupal.platform.treatGmapsBehavior();
    Drupal.platform.openProductLabeling();
    Drupal.platform.openSelectBuyNow();	
    Drupal.platform.openOverlaySubmenu();
    if (typeof Drupal.settings.faq !== 'undefined') {
      Drupal.platform.faqFilterMobile(); 
    }
});

    
  Drupal.platform.themeGeneralInit = function() {
    $('img').css({
      'max-width': '100%',
      'height': 'auto'
    });
    $('#gmapslivesearch-map img').css({
      'max-width': '',
      'height': ''
    });
  }

  Drupal.platform.treatMobileDevice = function() {
    if(_isMobileIPhone()) {
      window.location.hash = "ios";
      $('body').addClass('ios-mobile-device');
    }
  }
  
  Drupal.platform.treatExpandedMenuBehaviors = function() {
    $("#main-navigation li.expanded ul a").addClass("child");
    $("#main-navigation a.no-link").attr('href', 'javascript:void(0);');

    $("#main-navigation li.expanded a").click(function() {
      $(this).parent('li').toggleClass("opened");
      $(this).next('ul').slideToggle();
    });
    
    // Adding span to the last item on the carousel to add an arrow
    $('div#block-system-main-menu a#home').append('<span class="general-sprite-arrow"></span>');
  }

  Drupal.platform.treatMenuNavigationBehaviors = function() {
    $(".open-menu").click(function(event) {
      event.preventDefault();
      $(this).toggleClass("opened");
      $("div#overlay-mask").toggle();
      $("#main-navigation").toggleClass('opening');
      
      // Call the active trail function
      Drupal.platform.openActiveTrailSubmenu();
      
      $("#main-navigation li:not(.active-trail).expanded ul").hide();
      
      Drupal.platform.setMenuNavigationPosition();
      
      $("#main-navigation").slideToggle('fast',function() {
        $("#main-navigation li:not(.active-trail).expanded").removeClass('opened');
      });
      
      //Disable selects to prevent losing focus in the menu
      if ($(this).hasClass('opened')) {
        $('select:not([disabled])').addClass('disabled-select').attr('disabled','disabled');
      } else {
        $('select.disabled-select').removeClass('disabled-select').removeAttr('disabled');
      }
    });
  }
 
  Drupal.platform.treatFakeActiveClass = function() {
    var elem =  'a, .views-field-field-faq-question p, select.buy-now, div.product-labeling-expand, a.button-back-faq, a.google-maps-live-search-go-button, a.gmapslivesearch-btn-list,'+
	            'a.gmapslivesearch-btn-map, a.View Full Site, a.footer-pfizer, a.footer-privace-police, a.footer-privace-police-faqs, a.footer-terms-of-use, button.extlink-ok-btn,'+
				'button.extlink-cancel-btn, #edit-email-submit ';
    
    var elemOurProducts = '.views-field-field-image-thumbnail-mobile .field-content a, .views-field-title span a, .views-field-field-products-subhead-mobile div a, .views-field-view-node span a';
    
    // Fake Active elem
    if (navigator.userAgent.toLowerCase().indexOf("android") > -1) {
      $(elem)
      .bind("touchstart", function () {
        $(this).addClass("fake-active");
      })
      .bind("touchend", function() {
        $(this).removeClass("fake-active");
      })
      .bind("touchcancel", function() {
      // sometimes Android fires a touchcancel event rather than a touchend. Handle this too.
        $(this).removeClass("fake-active");
      });
    }
    else { 
      $(elem)      
      .click(function () {
        var obj = $(this);
        $(this).addClass("fake-active");
        setTimeout( function() {
          obj.removeClass("fake-active");
        },500);
      });
    }
    
    // Fake Active elemOurProducts
    if (navigator.userAgent.toLowerCase().indexOf("android") > -1) {
        $(elemOurProducts)
        .bind("touchstart", function () {
        	$(this).parents('div.view-id-products .view-content div.views-row').addClass("fake-active");
        })
        .bind("touchend", function() {
        	$(this).parents('div.view-id-products .view-content div.views-row').removeClass("fake-active");
        })
        .bind("touchcancel", function() {
        // sometimes Android fires a touchcancel event rather than a touchend. Handle this too.
        	$(this).parents('div.view-id-products .view-content div.views-row').removeClass("fake-active");
        });
        
      }
      else { 
        $(elemOurProducts)      
        .click(function () {
          var obj = $(this).parents('div.view-id-products .view-content div.views-row');
          obj.addClass("fake-active");
          setTimeout( function() {
            obj.removeClass("fake-active");
          },500);
        });
     }
  }
  
  Drupal.platform.openActiveTrailSubmenu = function() {
    var isActiveTrailOpened = $("#main-navigation li.active-trail.expanded").hasClass('opened');
    var isMainMenuOpening = $("#main-navigation").hasClass('opening');
    var shouldOpenActiveTrail = (!isActiveTrailOpened && isMainMenuOpening);

    if (shouldOpenActiveTrail) {
      $("#main-navigation li.active-trail.expanded").addClass('opened');
      $("#main-navigation li.active-trail.expanded ul").show();
    }
  }
  
  Drupal.platform.setMenuNavigationPosition = function() {  
    var headerHeight = $("header#header").height();
    var pageTopHeight = $("div#header-region").height();
    var openMenuButtonHeight = $(".open-menu").height();

	  // Get the header height and take the value of the padding (5)
    var menu_position = headerHeight - ( pageTopHeight / 2 ) + openMenuButtonHeight - 4;
    $("#main-navigation").css('top',menu_position);
  }

  Drupal.platform.openOverlaySubmenu = function() {
    $('div.content a.open-menu').click(function() {
      $('div#overlay-mask').css('height',($(document).height()));
    });
  }

  /**
   * Carousel Functions
   */

  Drupal.platform.treatCarouselHandler = function() {
    if ($('#carousel-views-wrapper').length > 0) {
      $('#carousel-views-wrapper').infiniteCarousel();
    }
  }

  Drupal.platform.treatGmapsBehavior = function() {
    // Hiding the button to stylize the <a>
    $('#gmapslivesearch-box-wrapper input[type=submit]').hide();
    $('#google-maps-live-search-go-button').click(function() {
      $('#gmapslivesearch-box-wrapper input[type=submit]').click();
  	   $('div#gmapslivesearch-loader').show();
    });
  }
  
  Drupal.platform.openSelectBuyNow = function() {
    //Show span-buy-online
    $('select.buy-now').focus(function() {
      setTimeout( function() {
        $('span.span-buy-online').css('display','block'); 
      },100);
    });

    //Hide span-buy-online
    $('select.buy-now').focusout(function() {
      $('span.span-buy-online').css('display','none'); 	
    });

    $('select.buy-now').change(function() {
      if(this.options[this.selectedIndex].value != -1){
        $('.go-to-store-link').attr('href', this.options[this.selectedIndex].value);
        Drupal.attachBehaviors($('.go-to-store-link').parent());
        $('.go-to-store-link').click();
        $('html, body').animate({scrollTop:0}, 'slow');
        var select = $('select.buy-now');
        select.val($('option:first', select).val());
        $('span.span-buy-online').css('display','none');
      }
    });

  }

  Drupal.platform.openProductLabeling = function() {
  	$('div.product-labeling-expand').click(function() {
  	  $('.open-labeling').toggleClass("opened");
  	  $(".content-labeling").slideToggle('slow');
  	});
  }
  
  /** Faqs functions **/
  Drupal.platform.faqs = new Object();
  Drupal.platform.faqFilterMobile = function() {
    var edit_submit = $('div.views-exposed-form input.form-text');
    var button_edit = $('div.views-exposed-form input.edit-search');
    var default_search_value = Drupal.settings.faq.faq_search_faqs_text;
    var default_search_btn_label = $('input#edit-submit-faqs').attr('value');

    // inputbox search behaviors
    $.each(edit_submit, function() {
      $(this).focus(function() {
        $(this).css("font-style", "normal");
        Drupal.platform.faqs.clearValueSearch(default_search_value, $(this));
      });
  
      $(this).focusout(function() {
        Drupal.platform.faqs.defaultValueSearch(default_search_value, $(this));
      });

      Drupal.platform.faqs.defaultValueSearch(default_search_value, $(this));
      Drupal.platform.handleSearchButton(default_search_btn_label, $('input#edit-submit-faqs', $(this).closest('div.views-exposed-form')), default_search_value, $(this));
    });
    
    $(edit_submit).keydown(function(e) {
      if ($('input#edit-search', $(this).closest('div.views-exposed-form')).attr('value') == ''
        && e.keyCode == 13) {
        return false;
      }
    });

    $(edit_submit).keyup(function(e) {
      Drupal.platform.handleSearchButton(default_search_btn_label, $('input#edit-submit-faqs', $(this).closest('div.views-exposed-form')), default_search_value, $(this));
    });
    //adding span filters
    $('div.bef-select-as-links div div a').append('<span class="general-sprite"></span>');
    $('div.views-field-field-faq-question p').append('<span class="general-sprite"></span>');

    filter_matches = $(location).attr('search').match(/tid=([^&]+)/);
    search_matches = $(location).attr('search').match(/search=([^&]+)/);
    // Remove filtes
    if((filter_matches) || (search_matches)){
      $('div.views-widget-form-after-filter').remove();
      // Show button back
      $('.button-back-faq').show();
      
      // Alter order button back no-results-found
      if($('div.view-id-faqs div').hasClass('view-empty')){
        $('div.view-empty').addClass('no-results-found'); 
        $('div.header-faq').addClass('no-results-found');
      };
    }
  }
  
  Drupal.platform.faqs.clearValueSearch = function(default_search_value, element) {
    if (element.attr('value') == default_search_value) {
      element.attr('value', '');
    }
  }
  
  Drupal.platform.faqs.defaultValueSearch = function(default_search_value, element) {
    if (element.attr('value') == ''){
      element.attr('value', default_search_value);
      element.css("font-style", "italic");
    }
  }
  Drupal.platform.handleSearchButton = function(default_search_btn_label, submit_button, default_search_value, element) {
    var buttonDisabled = $('.btn-disabled', element.closest('div.views-exposed-form'));
    
    if (element.attr('value') == default_search_value || element.attr('value') == '' ) {
      submit_button.hide();
      
      if (buttonDisabled.length == 0) {
        submit_button.parent().append('<div class="btn-disabled"><input type="button" title="Search" id="edit-submit-faqs-disabled" value="'+ default_search_btn_label +'" class="faq-submit disabled"/></div>');
      }
      else {
        buttonDisabled.show();
      }
    }
    else {
      buttonDisabled.hide();
      submit_button.show();
    }
  }

  /** Other functions **/
  function _isMobileIPhone() {
    if(Drupal.settings.mobileDevice.type != 'desktop' && (Drupal.settings.mobileDevice.group == 'iphone' || Drupal.settings.mobileDevice.group == 'ipod')) {
      return true;
    }
    return false;
  }
  function formatString(string) {
    return string.replace(/\s/g, "-").replace('&amp;','and').toLowerCase();
  }
  
})(jQuery);