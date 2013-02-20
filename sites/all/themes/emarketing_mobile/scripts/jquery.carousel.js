/**
* Author: Diego La Monica http://diegolamonica.info
* Detailed description of this work is available on http://diegolamonica.info/improving-jquery-infinite-carousel-part-1
* Based on original script described on http://jqueryfordesigners.com/demo/infinite-carousel.html
*/
(function($){
  $.fn.infiniteCarousel = function () {
    function repeat(str, num) {
      return new Array( num + 1 ).join( str );
    }
     return this.each(function () {
      var currWinOrientation = window.orientation;
      var prevWinOrientation = currWinOrientation;
      var screenSize = treatCarouselGetScreenSize(currWinOrientation);
      var screenItem = screenSize + 'px';

      $('.carousel-items-list li').css('width', screenItem);
      $('.carousel-items-list li img').css('width', screenItem);
      $('.carousel-items-list').css('width', screenItem);
      
      var $wrapper = $('> div', this).css('overflow', 'hidden'),
        $slider = $wrapper.find('> ul'),
        $items = $slider.find('> li'),
        $single = $items.filter(':first'),
        singleWidth = $single.outerWidth(), 
        currentPage = 0;
      page = 0;
        treatCarouselImagePosition(window.orientation, screenSize);
        var interval = treatCarouselAutoPlay();
        
      function recalculateAfterResize() {
        screenSize = treatCarouselGetScreenSize(window.orientation);
        screenItem = screenSize + 'px';
        $('.carousel-items-list li').css('width', screenSize);
        $('.carousel-items-list li img').css('width', screenSize);
        $('.carousel-items-list').css('width', screenSize);
        $('.cloned').hide();
        // Reset to the original carousel condition
        $('.empty', $wrapper).remove();
        $('.cloned', $wrapper).remove();
        
        $items = $slider.find('> li');
        singleWidth = $single.outerWidth();
        $wrapper.visible = Math.floor($wrapper.innerWidth() / singleWidth),
        $wrapper.pages = Math.ceil($items.length / $wrapper.visible);
        // 1. Pad so that 'visible' number will always be seen, otherwise create empty items
        if (($items.length % $wrapper.visible) != 0) {
          $slider.append(repeat('<li class="empty" />', $wrapper.visible - ($items.length % $wrapper.visible)));
          $items = $slider.find('> li');
        }
        // 2. Top and tail the list with 'visible' number of items, top has the last section, and tail has the first
        $items.filter(':first').before($items.slice(- $wrapper.visible).clone().addClass('cloned'));
        $items.filter(':last').after($items.slice(0, $wrapper.visible).clone().addClass('cloned'));
        $items = $slider.find('> li'); // reselect
        if(page == 0) {
          gotoPage(1);
          $('li', $wrapper).show();
        }
        else {
          $wrapper.scrollLeft(singleWidth * currentPage);
        }
        $($slider).css('width', ($items.length+1) * singleWidth);
        page = 1;
      } 

      $(window).resize(function(){
          recalculateAfterResize();
          if(prevWinOrientation != window.orientation) {
            screenSize = treatCarouselGetScreenSize(window.orientation); 
            treatCarouselImagePosition(window.orientation, screenSize);
            prevWinOrientation = window.orientation;
          }
        });
      $(window).bind( 'orientationchange', function(e){
        recalculateAfterResize();
        if(prevWinOrientation != window.orientation) {
          screenSize = treatCarouselGetScreenSize(window.orientation); 
          if (_isMobileIPhone()) {
            treatCarouselImagePosition(window.orientation, screenSize);
            prevWinOrientation = window.orientation;
          }
        }
      });

      if($(this).is(':visible')) {
        recalculateAfterResize();
        if(prevWinOrientation != window.orientation) { 
          screenSize = treatCarouselGetScreenSize(window.orientation); 
          treatCarouselImagePosition(window.orientation, screenSize);
          prevWinOrientation = window.orientation;
        }
      }
      
      // 4. paging function
      function gotoPage(page) {
        var countImages = $('#carousel-bullet-indicator li').length;
        var dir = page < currentPage ? -1 : 1,
        n = Math.abs(currentPage - page),
        left = singleWidth * dir * $wrapper.visible * n;
        $wrapper.filter(':not(:animated)').animate({
          scrollLeft : '+=' + left
        }, 500, function () {
          if (page == 0) {
            $wrapper.scrollLeft(singleWidth * $wrapper.visible * $wrapper.pages);
            page = $wrapper.pages;
          } else if (page > $wrapper.pages) {
            $wrapper.scrollLeft(singleWidth * $wrapper.visible);
            // reset back to start position
            page = 1;
          } 
          document.querySelector('#carousel-bullet-indicator > li.active').className = '';
          document.querySelector('#carousel-bullet-indicator > li:nth-child(' + (page) + ')').className = 'active';
          currentPage = page;
        });                
  
        return false;
      }

      function gotoNext(){
        return gotoPage(currentPage+1);
      };
      
      function gotoPrev(){
        return gotoPage(currentPage-1);
      };
      
     function treatCarouselAutoPlay() {
        var cInterval = 4000;
        if (typeof Drupal.settings.carousel_interval !=='undefined' && !isNaN(Drupal.settings.carousel_interval)) {
            cInterval=Drupal.settings.carousel_interval;
        }

        return window.setInterval(function () {
           gotoNext();
        }, cInterval); 
      }
      
      function _isMobileIPhone() {
        if(Drupal.settings.mobileDevice.type != 'desktop' && (Drupal.settings.mobileDevice.group == 'iphone' || Drupal.settings.mobileDevice.group == 'ipod')) {
          return true;
        }
        return false;
      }
      

      function treatCarouselGetScreenSize(currWinOrientation) {   
        var screenSize = screen.width;
        if (currWinOrientation == 0) {
          screenSize = 480;
        }
        else {
          screenSize = 615;
        }

        return screenSize;
      }
      
      
      function treatCarouselImagePosition(currWinOrientation, screenSize) {
        var imagePositionX = 0;
        var deltaPositionX = 0;
        var screenWidth = screen.width;

        if (_isMobileIPhone()) {
          screenWidth = 480;
        }
        if (currWinOrientation != 0) {
          deltaPositionX =  (screenSize - screenWidth) /2;
          imagePositionX = deltaPositionX * -1;
          $('.carousel-items-list li').css('left', (imagePositionX + 'px'));
        }
        else {
          if (screenSize > screen.width ){
            deltaPositionX =  (screenSize - screen.width  ) /2;
            imagePositionX = deltaPositionX * -1 ;
          $('.carousel-items-list li').css('left', (imagePositionX + 'px'));
         }
        }

        return deltaPositionX;
      }

      $('#carousel-previous-item', $wrapper.parent()).after('<a class="arrow back"></a>');
      $('#carousel-next-item', $wrapper.parent()).after('<a class="arrow forward"></a>');
      $wrapper.touchwipe({
       
        wipeLeft: function(){
          window.clearInterval(interval);
          gotoNext();
        },
        wipeRight: function(){
          window.clearInterval(interval);
          gotoPrev();
        },
        wipeUp: function() { 

          var docScroll = $(document).scrollTop() - 100;
          if (docScroll < 0) docScroll = 0;
          $('html, body').animate({scrollTop: docScroll}, 350);
        },
        wipeDown: function() { 
          var docScroll = $(document).scrollTop() + 100;
          $('html, body').animate({scrollTop: docScroll}, 350);
        }
      });
      // 5. Bind to the forward and back buttons
      $('a.back', this).click(function () {
        window.clearInterval(interval);
        return gotoPrev();                
      });
  
      $('a.forward', this).click(function () {
        window.clearInterval(interval);
        return gotoNext();
      });
  
      // create a public interface to move to a specific page
      $(this).bind('goto', function (event, page) {
        gotoPage(page);
      });
    });  
  };
})(jQuery);