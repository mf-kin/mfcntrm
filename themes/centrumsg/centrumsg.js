(function ($) {

  $(document).ready(function(){
	
	$('#user-login input').attr('autocomplete','off');
	$('#user-login input').attr('autocorrect','off');
	$('#user-login input').attr('autocapitalize','off');
	$('#user-login input').attr('spellcheck','off');
	
    $('.adjust-text').live('click', function(){
		var size = $(this).attr('data-size');
		$('.node .content .content-to-resize').css('font-size', size);
		return false;
	});
	
    $('a.tab').live('click', function(e){
            e.preventDefault();
            var id = $(this).attr('href');
            color = $(this).css('backgroundColor');
            console.log('rgb: ' + color);
            color = hexc(color);
            console.log('hex: ' + color);
            
            
            id = id.substring(1);
            $('.bf-content').children('.panel').each(function () {
                console.log('this id: ' + $(this).attr('id'));
                console.log('reference id: ' + id);
                if($(this).attr('id') == id)
                {
                    $(this).css('display', 'block');
                    $('.bf-content').css('border-color', color);
                }
                else
                {
                    $(this).css('display', 'none');
                }
            });
        });

    });
    
    function hexc(colorval) {
        var parts = colorval.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
        delete(parts[0]);
        for (var i = 1; i <= 3; ++i) {
            parts[i] = parseInt(parts[i]).toString(16);
            if (parts[i].length == 1) parts[i] = '0' + parts[i];
        }
        return '#' + parts.join('');
    }
    
})(jQuery);

    
