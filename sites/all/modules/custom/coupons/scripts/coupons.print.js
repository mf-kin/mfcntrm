(function($){
  /* Print coupon */
  function checkIsIE() {
    if (navigator.appName.toUpperCase() == 'MICROSOFT INTERNET EXPLORER') {
      return true;
    }
    else {
      return false; 
    }
  }
  
  function printThisPage() {
    if (checkIsIE() == true) {
      document.iframecouponimage.focus();
      document.iframecouponimage.print();
    }
    else {
      window.frames['iframecouponimage'].focus();
      window.frames['iframecouponimage'].print();
    }
  }
  
  $(document).ready(function(){
    $('input#coupon-print-button').click(function(){
      // http://www.eggheadcafe.com/PrintSearchContent.asp?LINKID=449
      printThisPage();
    });
    
    /*Show button to print coupon only if iframe is completely loaded*/
    $('iframe#iframe-coupon-image').load(function(){
      $('input.print-coupon-button').show();
      $('div#loader-print-button').hide();
    });
  });
  
})(jQuery);