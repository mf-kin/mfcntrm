(function($) {
  
  $(function() {
  
    $('.use-snippet').each(function(i, e) {
      var wrapper = $(this).closest('div.form-item'),
          elementType = getElementType($(this)),
          isCollapsed = true;
      updateSnippetCode(wrapper, elementType, isCollapsed);
      
      $(this).keyup(function() {
        isCollapsed = false;
        updateSnippetCode(wrapper, getElementType($(this)), isCollapsed);
      });
    });
    
    function getElementType(element) {
      return elementType = element.is('textarea') ? 'textarea' :
                           element.is(':text') ? 'textfield' :
                           'unknown';
    }
    
    function applySnippetStyle(codeSnippet, elementType, isCollapsed) {
      var snippetStyle = getDefaultSnippetStyle();
      
      if (elementType == 'textarea') {
        $.extend(snippetStyle, { collapse : true, startCollapsed : isCollapsed, showNum : true });
      }
      else if (elementType == 'textfield') {
        $.extend(snippetStyle, { collapse : false, showNum : false });
      }
      
      codeSnippet.snippet("javascript", snippetStyle);
    }
    
    function getDefaultSnippetStyle() {
      return { style: 'darkness' };
    }
    
    function updateSnippetCode(wrapper, elementType, isCollapsed) {
      wrapper.find('.snippet-container').remove();
      
      var textareaValue = wrapper.find('.use-snippet').val();
      wrapper.append($('<pre></pre>').addClass('javascript-snippet').html(textareaValue));
      
      var codeSnippet = wrapper.find('pre.javascript-snippet');
      applySnippetStyle(codeSnippet, elementType, isCollapsed);
    }
    
  });
  
})(jQuery);