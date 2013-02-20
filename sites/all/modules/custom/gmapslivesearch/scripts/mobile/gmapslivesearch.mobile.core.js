(function($){
  var map;
  var searcher;
  var searchControl;
  var GoogleLinks = new Array();
  var Markers = new Array();  
  var stores = new Array();
  var settings = new Array();
  var infoWindow;

  // Display the dafault Map
  $.fn.default_map = function(options) {
    settings = {
        divMap : 'gmapslivesearch-map'
    }
    $.extend( settings, options );
    
    return this.each(function() {
      loadGoogleJsapi();
      
      // Load Google Maps
      function loadMaps(){
        google.load("maps", "3",  {other_params:"sensor=false","callback": createDefaultMap}); 
      }
      
      // Load Google JSAPI
      function loadGoogleJsapi(){
        $.getScript('https://www.google.com/jsapi?key=' + settings.apiKey, function(data, textStatus){
          loadMaps();
        });
      }
      
      function createDefaultMap(){
        var latlng = new google.maps.LatLng(settings.default_latitude, settings.default_longitude);
        var myOptions = {
            zoom: parseInt(settings.default_zoom),
            center: latlng,
            mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        var map = new google.maps.Map(document.getElementById(settings.divMap), myOptions);
      }
    });
  }

  // Display the Map related to the search
  $.fn.buynow = function(options, callback) {
    settings = {
        apiKey          : 'ABQIAAAANfhoe0INRhmAE6PvohZqUxT2yXp_ZAY8_ufC3CFXhHIE1NvwkxTopbkOmnwf3iP4JAZtWKM2LGWOwg',
        searchString    : 'coffee',
        address         : '10010',
        divMap          : 'gmapslivesearch-map',
        divWrapper      : 'gmapslivesearch-wrapper',
        markerPath      : null,
        markerExtension : 'png',
        divStores       : 'gmapslivesearch-stores',
        directionsTxt   : Drupal.t('Map & Directions'),
        zoom            : 15,
        radius          : 1,
        strokeColor     : "#A6CE39",
        strokeOpacity   : 0.8,
        strokeWeight    : 2,
        fillColor       : "#D2E363",
        fillOpacity     : 0.35,
        generateCircle  : false
    };
    $.extend( settings, options );

    var buyNowObj = $(this);

    buyNowObj.css('visibility','hidden');

    return this.each(function() {
      loadGoogleJsapi();

      if ($('#gmapslivesearch-printresults').length > 0) {
        $('#gmapslivesearch-printresults').attr('href', 'http://maps.google.com/maps?pw=2&q=' + settings.searchString +
            ' : ' + settings.address);
      }

      // Load Google Maps
      function loadMaps(){
        google.load("maps", "3",  {other_params:"sensor=false","callback": loadSearch}); 
      }
      // Load Google Local Search
      function loadSearch(){
        google.load('search' , '1',  {"callback": getSearchResultsAndCreateMap}); 
      }
      // Load Google JSAPI
      function loadGoogleJsapi(){
        $.getScript('https://www.google.com/jsapi?key=' + settings.apiKey, function(data, textStatus){
          loadMaps();
        });
      }

      //Generates an info window with the given content.
      function createInfoWindow(content) {
        if (infoWindow == undefined) {
          infoWindow = new google.maps.InfoWindow({content: content});
        }
        else {
          infoWindow.setContent(content);
        }
        return infoWindow;
      }

      // Get Search Results
      function getSearchResultsAndCreateMap(){
        var mapContainer = document.getElementById(settings.divMap); // build the map div

        // We're ready to build our map...
        var myOptions = {
            zoom: settings.zoom,
            mapTypeControl: true,
            mapTypeControlOptions: {
          style: google.maps.MapTypeControlStyle.DROPDOWN_MENU
        },
        navigationControl: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        map = new google.maps.Map(mapContainer, myOptions);

        // So let's build the search control
        searchControl = new google.search.SearchControl();
        searchControl.setResultSetSize(google.search.Search.LARGE_RESULTSET);
        // Initialize a LocalSearch instance
        searcher = new google.search.LocalSearch(); // create the object
        // Set the map's center point and finish!
        // Create a SearcherOptions object to ensure we can see all results
        var options = new google.search.SearcherOptions(); // create the object
        options.setExpandMode(google.search.SearchControl.EXPAND_MODE_OPEN);
        // Add the searcher to the SearchControl
        searchControl.addSearcher(searcher , options);
        // Set the Local Search center point
        searcher.setCenterPoint(settings.address);
        // Specify search quer(ies)
        searcher.execute(settings.searchString);
        // Draw the control
        searchControl.draw(document.getElementById("searcher"));      
        document.getElementById("searcher").innerHTML = "";
        // Set searchComplete as the callback function when a search is complete.
        // The localSearch object will have results in it.
        searchControl.setSearchCompleteCallback(searcher , function() {
          stores = searcher.results;
          addMarkersAndListStores();
          $('#gmapslivesearch-btn-list').show();
          $('#gmapslivesearch-stores').hide();
          $('div#gmapslivesearch-loader').hide();
        });
      }

      // Create markers fot the resulting map.
      function createMarkers(result, index, image) {
        var markerLatLng = new google.maps.LatLng(parseFloat(result.lat), parseFloat(result.lng));
        var marker = new google.maps.Marker({
          position: markerLatLng,
          map: map,
          icon: image,
          optimized: false
        });
        var storeInfo = '';

        google.maps.event.addListener(marker, 'click', function() {
          marker.setZIndex(9999);
          storeInfo += "<div class='GMAP_infowindow'>";
          storeInfo += "<div class='GMap_Title'>" + result.titleNoFormatting + "</div>";
          storeInfo += "  <div class='GMap_Phone'>";
          try {
            for (var p = 0; p<result.phoneNumbers.length; p++) {
              if (result.phoneNumbers[p].type == "") {
                storeInfo += "    <span>Phone:</span> " + result.phoneNumbers[p].number + "<br />";
              } else {
                storeInfo += "    <span>" + result.phoneNumbers[p].type + ": </span>" +
                result.phoneNumbers[p].number + "<br />";
              }
            }
          } catch(err) {
          }

          storeInfo += " </div>";
          storeInfo += "<span class='GMap_Else'>" + result.streetAddress + "<br>" + result.city + ", " +
          result.region + "</span><br>";
          storeInfo += "<span class='Links'><a href='javascript:goGoogle("+ index +");' class='Links mapDirections' rel='"
          + index +"'>" + Drupal.t('Get Directions') + "</a>";
          storeInfo += "</span></div>";
          createInfoWindow(storeInfo);
          infoWindow.open(map, this);
          storeInfo = '';
        });

        return marker;
      }
      
      //Function used to calculate the distance between a set of two lat/lon points on the surface of the Earth
      function haversineDistance(lat1, lon1, lat2, lon2){
        var R = 6371; // Earth's mean radius in km
        var dLat = toRad(lat2-lat1); // delta latitude
        var dLon = toRad(lon2-lon1); // delta longitude
        lat1 = toRad(lat1); //Converting the values to radians
        lat2 = toRad(lat2);

        var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
        Math.sin(dLon/2) * Math.sin(dLon/2) * Math.cos(lat1) * Math.cos(lat2); 
        var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
        return R * c;
      }
      
      
      function parseDeg(degree) {
          if (!isNaN(degree)) return Number(degree);                    // signed decimal degrees without NSEW
          var degLL = degree.replace(/^-/,'').replace(/[NSEW]/i,'');  // strip off any sign or compass dir'n
          var dms = degLL.split(/[^0-9.]+/);                        // split out separate d/m/s
          for (var i in dms) if (dms[i]=='') dms.splice(i,1);       // remove empty elements (see note below)
          switch (dms.length) {                                     // convert to decimal degrees...
            case 3:                                             // interpret 3-part result as d/m/s
              var deg = dms[0]/1 + dms[1]/60 + dms[2]/3600; 
              break;
            case 2:                                                         // interpret 2-part result as d/m
              var deg = dms[0]/1 + dms[1]/60; 
              break;
            case 1:                                                         // decimal or non-separated dddmmss
              if (/[NS]/i.test(degree)) degLL = '0' + degLL;                  // - normalise N/S to 3-digit degrees
              var deg = dms[0].slice(0,3)/1 + dms[0].slice(3,5)/60 + dms[0].slice(5)/3600; 
              break;
            default:
              return NaN;
          }
          
          if (/^-/.test(degree) || /[WS]/i.test(degree)) 
            deg = -deg;            // take '-', west and south as -ve
          
          return deg;
      }
      
      function toRad(value) {  // convert degrees to radians
        return value * Math.PI / 180;
      }

      function toDeg(value) {  // convert radians to degrees (signed)
        return value * 180 / Math.PI;
      }
      
      function kmToMiles(value) {
        var valueKm = value / 1.609344;
        valueKm = Math.round( valueKm * Math.pow( 10 , 1 ) ) / Math.pow( 10 , 1 );
        return valueKm;
      }
      
      function treatEmptyResult(divMap, divStores, msg){
        $('div#' + divMap).parent().parent().css('display','none').parent().append('<h3>' + msg + '</h3>');
      }

      // Add Makers to the Map, and Create the list of stores.
      function addMarkersAndListStores(){
        var controlContainer = document.getElementById(settings.divStores); // build the control div
        var storeInfo = '';
        
        var centerX = searcher.resultViewport.center.lng; //Get search location latitude
        var centerY = searcher.resultViewport.center.lat; //Get search location longitude
        
        
        var storeX = 0; //This will keep the store's latitude
        var storeY = 0; //This will keep the store's longitude
         
        var distance = 0;
        var distanceMiles = 0;
        
        var greatDistanceFlag = false; // We need to check if there's a recommendation further than 50mi from the search point
                                       // If we do, we have to show a message later.

        var maxDistance = Drupal.settings.gmapslivesearch_max_distance;
        
        if(settings.generateCircle != 0 && typeof(settings.radius) != 'undefined') {
          maxDistance = settings.radius;
        }
        var maxDistanceMsg = Drupal.settings.gmapslivesearch_max_distance_message.replace('#distance#' , maxDistance);
        var count = 0;
        
        storesListHtml = "<ul>";
        
        if(stores.length == 0){
          treatEmptyResult(settings.divMap,settings.divStores, maxDistanceMsg);
          return;
        }
        
        for (var i = 0; i < stores.length; i++) {
          var result = stores[i]; // Get the specific result
          //Add directions link to this store to the array
          GoogleLinks[count] = result.ddUrlToHere;
          storeX = result.lng;
          storeY = result.lat;
          distance = haversineDistance(parseDeg(centerY), parseDeg(centerX), parseDeg(storeY), parseDeg(storeX));
          distanceMiles = kmToMiles(distance);
          
          if(distance > maxDistance && Drupal.settings.gmapslivesearch_max_distance_enabled) {
            //We don't want stores that are farther away than the radius limit
            continue;
          }
          var imageHeight = 43;
          var imageWidht = 35;
          var imageWidhtSize = imageHeightSize = '';
          if (Drupal.settings.gmapslivesearch_map_bullet_height != undefined ) {
           
            imageHeight =  parseInt(Drupal.settings.gmapslivesearch_map_bullet_height);
            imageHeightSize = "height='"+ imageHeight +"' ";
          }
          if (Drupal.settings.gmapslivesearch_map_bullet_width != undefined ) {
            imageWidht =  parseInt(Drupal.settings.gmapslivesearch_map_bullet_width);
            
            imageWidhtSize = "width='"+ imageWidht +"' ";
          }
          var image = settings.markerPath + (count + 1) + settings.markerExtension;
          
          var icon = image;
          
          if (imageWidht != '' && imageWidhtSize != '' ) {
            icon =  new google.maps.MarkerImage(image,
                new google.maps.Size(imageWidht,imageHeight),
                null, 
                null,
                new google.maps.Size(imageWidht,imageHeight)
            );
          }
          
          var sharePhoneNumbers = '';
          var htmlPhoneNumbers = '';
          
          try {
            for (var p = 0; p<result.phoneNumbers.length; p++) {
              if (result.phoneNumbers[p].type == "") {
                sharePhoneNumbers += 'Phone: ' + result.phoneNumbers[p].number + '%0d%0a';
                htmlPhoneNumbers += "    <span>Phone:</span> " + result.phoneNumbers[p].number + "<br />";
              } else {
                sharePhoneNumbers += result.phoneNumbers[p].type + ': ' + result.phoneNumbers[p].number + '%0d%0a';
                htmlPhoneNumbers += "    <span>" + result.phoneNumbers[p].type + ": </span>" +
                result.phoneNumbers[p].number + "<br />";
              }
            }
          } catch(err) {
          }
          
          
          var sendToAFriendHtml = '';
          
          if (Drupal.settings.gmapslivesearch_show_share) {
            var shareBody = result.titleNoFormatting + '%0d%0a' + sharePhoneNumbers + result.streetAddress + '%0d%0a' + result.city + ', ' + result.region;
            sendToAFriendHtml = $('<a />').addClass('share').text('Share').attr('href', 'mailto:?body=' + shareBody).attr('outerHTML');
          }
          
          Markers.push(createMarkers(result, count, icon));
          //Add the store to the list                
          storesListHtml += "<li>";
          storesListHtml += " <div class='GMap_Info'>";
          storesListHtml += "<div class='GMap_Marker'>";
          storesListHtml += "<a href='#" + settings.divWrapper + "'  class='centerMap' rel='"+ count +"'><img src='" +
          image + "' border='0' class='imgnumber' "+ imageHeightSize + imageWidhtSize +" /> <span class='distance'>" + distanceMiles + " miles</span></a>";
          storesListHtml += "</div>"; 
          storesListHtml += " <div class='GMap_Store_Info'>";
          storesListHtml += "  <div class='GMap_Title'>" + result.titleNoFormatting + sendToAFriendHtml + "</div>";
          storesListHtml += "  <div class='GMap_Phone'>";
          storesListHtml += htmlPhoneNumbers;
          storesListHtml += " </div>";
          storesListHtml += "  <div class='GMap_Address'>" + result.streetAddress + "</div>";
          storesListHtml += "  <div class='GMap_City_And_Region'>" + result.city + ", " + result.region + "</div>";
          storesListHtml += " </div>";
          storesListHtml += "<span class='general-sprite'></span>";
          storesListHtml += " <div class='LocationFooter'><a href='" + result.ddUrlToHere +
          "' class='Links mapDirections' rel='"+ count +"'>" + settings.directionsTxt + "</a></div>";
          storesListHtml += " </div>";          
          storesListHtml += "</li>";
          
          //Count is a separate counter because some results may be too far away, and we don't want to include those.
          //So 'i' is used to run through the result list that Google sent to us, and count is used to number these results
          count++;
        }
        
        if(count == 0){
          //TODO add redirect
          treatEmptyResult(settings.divMap,settings.divStores, maxDistanceMsg);
          return;
        }
        storesListHtml += "</ul>";

        if(greatDistanceFlag){
            var distanceMsg = $("<div>").attr("class","error inline-error").html(maxDistanceMsg);
            $("div.gmapslivesearch-box div.form-item-gmapslivesearch-field-address-to-search").after(distanceMsg);
        }        
        
        controlContainer.innerHTML = storesListHtml;
        if (settings.generateCircle) {
          //TODO Refactor this to center to the search spot, not on the first result.
          var myCircle = new google.maps.Circle({
            center: Markers[Markers.length - 1].getPosition(),
            map: map,
            radius: settings.radius * 1609 ,
            strokeColor: settings.strokeColor,
            strokeOpacity: settings.strokeOpacity,
            strokeWeight: settings.strokeWeight,
            fillColor: settings.fillColor,
            fillOpacity: settings.fillOpacity
          });
          var myBounds = myCircle.getBounds();

          map.setCenter(Markers[Markers.length - 1].getPosition());
          generateCircle = false;
          map.fitBounds(myBounds);
          map.setZoom(map.getZoom() + 1);       

        }
        else {

          if(Markers.length == 0) {          
            treatEmptyResult(settings.divMap,settings.divStores, maxDistanceMsg);
          } else {
            markersbounds = new google.maps.LatLngBounds();
            for(i=0; i< Markers.length; i++){
              markersbounds.extend(Markers[i].getPosition());
            }

            map.fitBounds(markersbounds);
            map.setCenter(markersbounds.getCenter());
            if(map.getZoom() > 16){
             map.setZoom(16);              
            };  
          }

        }

        if (typeof callback == 'function') { // make sure the callback is a function
          callback.call(this); // brings the scope to the callback
        }

        buyNowObj.css('visibility','visible');     

        /*$('.mapDirections', controlContainer).click(function() {
        openDirections($(this).attr('rel'));
      });*/
        

        $('div#gmapslivesearch-stores li').each(function() { 
          var currentMarker = Markers[$('.centerMap', this).attr('rel')];
          lat = currentMarker.position.lat();
          lng = currentMarker.position.lng();
          
          $('> *:not(div.GMap_Store_Info), div.GMap_Store_Info > *:not(a.share)', this).click(function() { 
            centerLocation(lat,lng);
            currentMarker.setZIndex(9999);
            google.maps.event.trigger(currentMarker, 'click');
            $('#gmapslivesearch-btn-map').hide();
            $('#gmapslivesearch-stores').hide();
            $('#gmapslivesearch-map').show();
            $('#gmapslivesearch-btn-list').show();
            $("body").scrollTop( $('#main-wrapper').offset().top );
          });
        });
      }
      // Center the Maker
      function centerLocation(Lat, Lng) {
        bounds = map.getBounds();
        center = bounds.getCenter();
        ne = bounds.getNorthEast();
        // r = radius of the earth in statute miles
        var r = 3963.0;  
        // Convert lat or lng from decimal degrees into radians (divide by 57.2958)
        var lat1 = center.lat() / 57.2958; 
        var lon1 = center.lng() / 57.2958;
        var lat2 = ne.lat() / 57.2958;
        var lon2 = ne.lng() / 57.2958;
        // distance = circle radius from center to Northeast corner of bounds
        var dis = r * Math.acos(Math.sin(lat1) * Math.sin(lat2) + 
            Math.cos(lat1) * Math.cos(lat2) * Math.cos(lon2 - lon1));
        map.setCenter(new google.maps.LatLng(Lat, Lng), 13);
      }
      // Open Google Maps page with directions to the store address
      function openDirections(LinkId) {
        if (GoogleLinks[LinkId] != '') {
          window.open(GoogleLinks[LinkId]);
        }
      }
    });
  }
})(jQuery);

function goGoogle(index) {
  jQuery('div#gmapslivesearch-stores ul li:eq('+ index +') a.mapDirections').trigger('click');
}