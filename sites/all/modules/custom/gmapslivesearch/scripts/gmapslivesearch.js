(function($){ 
  $(document).ready(function(){
    
    if (typeof(Drupal.settings.gmapslivesearch) != 'undefined'  && typeof(Drupal.settings.gmapslivesearch_input_data) != 'undefined') {

      function gmapsCallback(){
        if ($.browser.msie) {
          $("#gmapslivesearch-stores").jScrollPane({
            verticalDragMinHeight: 0,
            verticalDragMaxHeight: 0
          });
          Drupal.attachBehaviors($('#gmapslivesearch-map-results'));
          $('div.jspTrack').css('height', '440px');
        }
        else {
          $("#gmapslivesearch-stores").jScrollPane({
            verticalDragMinHeight: Drupal.settings.gmapslivesearch_results_scroll_js_drag_height,
            verticalDragMaxHeight: Drupal.settings.gmapslivesearch_results_scroll_js_drag_height
          });
          Drupal.attachBehaviors($('#gmapslivesearch-map-results'));
        }
      }

      $('#gmapslivesearch-map-results').buynow({
          'markerPath' : Drupal.settings.basePath + Drupal.settings.gmapslivesearh_map_bullet_folder,
          'markerExtension' : Drupal.settings.gmapslivesearh_map_bullet_extension,
          'generateCircle' : Drupal.settings.gmapslivesearch_input_data.gmapslivesearch_field_milessearch_within_enabled,
          'radius' : Drupal.settings.gmapslivesearch_input_data.gmapslivesearch_field_milessearch_within,
          'address' : Drupal.settings.gmapslivesearch_input_data.gmapslivesearch_field_address_to_search,
          'searchString': Drupal.settings.gmapslivesearch.gmapslivesearch_field_searchstring,
          'apiKey': Drupal.settings.gmapslivesearch.gmapslivesearch_field_googleapikey
        },
        gmapsCallback
      );
       
    }
    else if (Drupal.settings.gmapslivesearch_enabled_default_result == '1' && Drupal.settings.gmapslivesearch_default_result_latitude != '' && Drupal.settings.gmapslivesearch_default_result_longitude != ''){
      $('#gmapslivesearch-map-results').default_map({
        'default_latitude' : Drupal.settings.gmapslivesearch_default_result_latitude,
        'default_longitude' : Drupal.settings.gmapslivesearch_default_result_longitude,
        'default_zoom' : Drupal.settings.gmapslivesearch_default_result_zoom,
        'apiKey': Drupal.settings.gmapslivesearch_field_googleapikey
      });
    }
    
    //Behavior for search fields. Set the default text and remove it when field is on focus in case
    //user don't type anything get the default value and print on the field otherwise leave the input value.
    $('.gmapslivesearch-field-address-to-search').each(function(index, element){
      var default_text_field_address_to_search = Drupal.settings.gmapslivesearch_field_address_default_label;
      defaultValueFieldAddressToSearch($(element), default_text_field_address_to_search);
      $(element).focus(function() {
    	  $(element).css("font-style", "normal");
    	  clearValueFieldAddressToSearch($(element), default_text_field_address_to_search);
      });
      
      $(element).focusout(function() {
    	  defaultValueFieldAddressToSearch($(element), default_text_field_address_to_search);
      });
    });
    
    //FIND ONLINE form
    var find_online_form = $('form.gmapslivesearch-find-online-form');
    var find_online_select = $('select', find_online_form);
    var find_online_submit = $('a.gmapslivesearch-goto-online-store', find_online_form);

    handleGoToStoreLink(find_online_submit, find_online_select);

    find_online_select.change(function() {
      var box_wrapper = $(this).closest('div.buy-online-box-wrapper');
      var find_online_submit = $('a.gmapslivesearch-goto-online-store', box_wrapper);
      handleGoToStoreLink(find_online_submit, this);
    });
    
    //BUY NOW form
    var buy_now_form = $('form#gmapslivesearch-buy-now-form');
    var buy_now_submit = $('a.gmapslivesearch-goto-online-store', buy_now_form);
    var buy_now_select = $('select.gmapslivesearch-field-stores-search', buy_now_form);

    handleGoToStoreLink(buy_now_submit, buy_now_select);
    
    if (Drupal.settings.gmapslivesearch_remove_buy_now_button){
      buy_now_submit.hide();
      var buy_now_link = $('.form-item-gmapslivesearch-field-stores-search li a:not(:first)', buy_now_form);
      buy_now_link.click(function(){
        buy_now_submit.click();
      });
    }
    
    buy_now_select.change(function() {
      handleGoToStoreLink(buy_now_submit, this);
      if (Drupal.settings.gmapslivesearch_remove_buy_now_button){
        buy_now_submit.click();
      }
    });

    if (Drupal.settings.gmapslivesearh_apply_jqtranform) {
      find_online_form.jqTransform('');
      buy_now_form.jqTransform('');
    }
  });
  
   
  function handleGoToStoreLink(link, store_selected) {
    link.attr('href', $(':selected', store_selected).val());
    link.unbind('click');
   
    if ($(':selected', store_selected).val() == Drupal.settings.basePath || $(':selected', store_selected).val() == '') {
      link.click(function(){
        if (!Drupal.settings.gmapslivesearch_remove_buy_now_button){
          alert(Drupal.t('Please select a retailer.'));
        }
        return false;
      });
    }
    else {
      Drupal.attachBehaviors(link.parent());
    }
  }
  
  function defaultValueFieldAddressToSearch(element, default_text_field_address_to_search) {
    if (element.attr('value') == '' || element.attr('value') == default_text_field_address_to_search){
        element.attr('value', default_text_field_address_to_search);
        element.css("font-style", "italic");
    }
  }

  function clearValueFieldAddressToSearch(element, default_text_field_address_to_search) {

    if (element.attr('value') == default_text_field_address_to_search) {
          element.attr('value', '');
    }
  }
  
})(jQuery);
