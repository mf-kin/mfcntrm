(function ($) {

  $(document).ready(function() {
    var exposed_faq_filter = $('div#block-views-exp-faqs-faqs-page');
    var mobileType =  'desktop';
    var mobileGroup = '';
    if (Drupal.settings.mobileDevice != undefined) {
      mobileType =  Drupal.settings.mobileDevice.type;
      mobileGroup = Drupal.settings.mobileDevice.group;
    }
    if (mobileType == 'mobile') {
      exposed_faq_filter = $('div#block-views-exp-faqs-faq-page-mobile');
    }
    /**
     * Clone the input search content to looking
     * for question and answer
     */
    $('div.views-submit-button input', exposed_faq_filter).click(function() {
      $('div.form-item-field-faq-question-value input', exposed_faq_filter).attr('value', $('div.form-item-search input', exposed_faq_filter).attr('value'));
      
      if(!isValidSearchTerm()) {
        $('input#edit-search').addClass('edit-search-error');
        return false;
      }
      else{
        $('input#edit-search').removeClass('edit-search-error');
      }
    });
    
    $('input#edit-search').click(function(){
      $(this).removeClass('edit-search-error');
    });

    /**
     * Change - Any - label to View All
     */
    if($('div#edit-tid-all a', exposed_faq_filter).length > 0) {
      $('div#edit-tid-all a', exposed_faq_filter).text(Drupal.t(Drupal.settings.faq.faq_view_all_text));
    } else {
      $('div#edit-tid-all', exposed_faq_filter).text(Drupal.t(Drupal.settings.faq.faq_view_all_text));
    }


    /**
     * Change the urls of filter to exclude the inputs text content
     */
    $('div.views-exposed-widget a', exposed_faq_filter).each(function() {
      var tid_matches = $(this).attr('href').match(/tid=[^&]+/);
      if (tid_matches) {
        var tid = tid_matches[0];
        var url = Drupal.settings.faq.faq_path + '?' + tid;
        $(this).attr('href', url);
      }
    });


    /**
     * Create "Search results for" section
     */
    var message = Drupal.t(Drupal.settings.faq.faq_search_results_text);
    var message_view_all = '<strong>' + Drupal.t(Drupal.settings.faq.faq_view_all_text) + '</strong>';
    message = message.replace('%filter_text%', message_view_all);
    var search_results_for = $('<p class="search-message">' + message + '</p>');
    var searched_term_label = $('strong', search_results_for);
    
    if($('input#edit-search', exposed_faq_filter).length > 0) {
      var default_search_text = Drupal.t(Drupal.settings.faq.faq_search_faqs_text);

      var input = $('input#edit-search', exposed_faq_filter);

      input.focus(function() {
        if (input.attr('value') == default_search_text) {
          input.css("font-style", "normal");
          input.attr('value', '');
        }
      });

      input.focusout(function() {
        if (input.attr('value') == '') {
          input.attr('value', default_search_text);
          input.css("font-style", "italic");
        }
      });

      var input_value = input.attr('value');
      if(isValidSearchTerm()) {
        $('div.views-exposed-form', exposed_faq_filter).append(search_results_for);
          if (mobileType == 'desktop') {
            $('div.views-exposed-form', exposed_faq_filter).append(search_results_for);
            var number_of_results_message = Drupal.settings.faq.faq_search_results_number;
            if (number_of_results_message) {
              var number_of_results = $('div.views-field-field-faq-question').length;
              number_of_results_message = number_of_results_message.replace('%results_number%', number_of_results);
              number_of_results_message = $('<p class="number-of-results">' + number_of_results_message + '</p>');

              $('div.views-exposed-form', exposed_faq_filter).append(number_of_results_message);
            }
          }
        // View All is not a link. It's just a HTML text instead.
        if($('div.views-exposed-widget div#edit-tid-all a', exposed_faq_filter).length == 0) {
          var url = Drupal.settings.faq.faq_path + '?tid=All';
          var view_all_wrapper = $('div.views-exposed-widget div#edit-tid-all', exposed_faq_filter);
          var text = view_all_wrapper.html();
          var view_all_link = $('<a href=' + url +'>' + text + '</a>');
          view_all_wrapper.html(view_all_link);
          $('div#edit-tid-all a').addClass("active");
        }
        
        $('div.views-exposed-form p.search-message').addClass('normal-search');
        if (number_of_results_message) {
          $('div.views-exposed-form p.number-of-results').addClass('normal-search');
        }
        
        input_value = input_value.replace(/(<([^>]+)>)/ig,'');
        searched_term_label.text(input_value);
      } else {
        input.attr('value', default_search_text);
        input.css("font-style", "italic");

        var search_matches = $(location).attr('search').match(/tid=([^&]+)/);
        if (search_matches) {
          var tid = search_matches[1].toLowerCase();
          var text = $('div#edit-tid-' + tid, exposed_faq_filter).text();
          text = text.replace(/(<([^>]+)>)/ig,'');
          searched_term_label.text(text);
        }
        
        else if (mobileType == 'mobile') {
          if($('div.views-exposed-widget div#edit-tid-all a', exposed_faq_filter).length == 0) {
            var url = Drupal.settings.faq.faq_path + '?tid=All';
            var view_all_wrapper = $('div.views-exposed-widget div#edit-tid-all', exposed_faq_filter);
            var text = view_all_wrapper.html();
            var view_all_link = $('<a href=/' + url +'>' + text + '</a>');
            view_all_wrapper.html(view_all_link);
          }
        }
        $('div.views-exposed-form p.search-message').addClass('category-search');
        if (number_of_results_message) {
          $('div.views-exposed-form p.number-of-results').addClass('category-search');
        }
        
      }
    }
  });
  
  function isValidSearchTerm() {
    var exposed_faq_filter;
    var mobileType =  'desktop';
    if (Drupal.settings.mobileDevice != undefined) {
      mobileType =  Drupal.settings.mobileDevice.type;
    }
    if (mobileType == 'mobile') {
      exposed_faq_filter = $('div#block-views-exp-faqs-faq-page-mobile');
    } else {
      exposed_faq_filter = $('div#block-views-exp-faqs-faqs-page');
    }
    
    var searchTerm = $('input#edit-search', exposed_faq_filter).attr('value');
    var default_search_text = Drupal.t(Drupal.settings.faq.faq_search_faqs_text);    
    var isValidSearchTerm = (searchTerm != '') && (searchTerm != default_search_text);
    
    return isValidSearchTerm;
  }

})(jQuery);