(function ($) {

  $(document).ready(function() {
    $("div.views-field-field-faq-question").click(function() {
      var answer = $(this).next('div.faq-answer-expandable');
      answer.filter(':not(:animated)').slideToggle('slow').toggleClass('expanded').toggleClass('');
    }); 
  })
  
})(jQuery);