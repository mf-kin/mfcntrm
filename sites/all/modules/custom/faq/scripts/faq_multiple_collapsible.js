(function ($) {
  $(document).ready(function() {
    $("div.views-field-field-faq-question").click(function() {
      $("div.faq-answer-expandable").slideUp('slow');
      var answer = $(this).next('div.faq-answer-expandable');
      answer.filter(':not(:animated)').slideToggle('slow').toggleClass('expanded').toggleClass('');
    });
  })
})(jQuery);