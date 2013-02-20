(function($) {
  
  $(function() {
    initGeneralSelectorsPage();
  });

  /**
   * Stub function. Allows a custom handler when a row is dropped.
   * Overriden to mark all fieldsets as updated.
   */
  Drupal.tableDrag.prototype.onDrop = function () {
    markAllFieldsetsAsUpdated();
    return false;
  };
  
  function initGeneralSelectorsPage() {
    
    // make fieldset column the biggest
    $('#general-selectors-table td:eq(1)').css('width', '90%');
    
    var fieldsets = $('fieldset.selectors-fieldset');
    $('input, textarea', fieldsets).bind('keyup.updated', function() {
      updateFieldsetAfterUserEdition($(this).closest('fieldset.selectors-fieldset'));
    });
    $('select', fieldsets).bind('change.updated', function() {
      updateFieldsetAfterUserEdition($(this).closest('fieldset.selectors-fieldset'));
    });
    
    $('input.delete-checkbox').click(function() {
      if ($(this).is(':checked')) {
        $(this).closest('tr').addClass('deleted');
      }
      else {
        $(this).closest('tr').removeClass('deleted');
      }
    });
  }
  
  function markAllFieldsetsAsUpdated() {
    $('fieldset.selectors-fieldset').each(function(i) {
      markFieldsetAsUpdated($(this));
    });
  }
  
  function updateFieldsetAfterUserEdition(fieldset) {
    markFieldsetAsUpdated(fieldset);
    unbindFieldsetUpdatedBehaviors(fieldset);
    fieldset.addClass('updated');
  }
  
  function markFieldsetAsUpdated(fieldset) {
    fieldset.find('input[type=hidden]').val(1);
  }
  
  function unbindFieldsetUpdatedBehaviors(fieldset) {
    fieldset.find('input, select, textarea').unbind('.updated');
  }
  
})(jQuery);

