/**
 * ALL THIS CODE IS COMMENTED AND AIMS TO SERVE AS AN EXAMPLE OF HOW OMNITURE TRACKING IS PERFORMED
 * THIS EXAMPLE SHOWS HOW TO SEND DATA ON PAGE LOAD USING omniture AND omnitureInfo OBJECTS
 * UPDATE ALL CODE TO FOLLOW THE TRACKING SPECIFICATION OF _YOUR WEBSITE_
 *
 * REMEMBER TO ___ERASE___ THIS COMMENT AND UNCOMMENT THE CODE UNDERNEATH
 */
/*
  // Page not found
  if (omnitureInfo.pageTitle == 'Not Found') {
    s.linkTrackVars = 'pageType';
    s.pageType = 'ErrorPage';
    s.pageName = omnitureInfo.pageNotFoundTitle + ' : ' + window.location.href;
  }

  var currentYear = new Date().getFullYear();
  var currentUserTimezone = new Date().getTimezoneOffset() / -60;

  var onPageLoadVariables = {
    'variables' : {
      'pageName'      : omnitureInfo.pageTitle,
      'channel'       : omnitureInfo.channel,
      'prop1,eVar1'   : [omnitureInfo.siteID, omnitureInfo.channel].join('/') + '/', // asked for the last trailing slash
      'prop9,eVar9'   : omnitureInfo.pageTitle,
      'prop15,eVar15' : s.getTimeParting('h', currentUserTimezone, currentYear),
      'prop16,eVar16' : s.getTimeParting('d', currentUserTimezone, currentYear),
      'prop17,eVar17' : s.getTimeParting('w', currentUserTimezone, currentYear),
      'prop18,eVar18' : omniture.getVisitorStatus(),
      'prop23,eVar23' : document.URL.toLowerCase()
    },
    'events' : [
      'event7'
    ]
  };

  // Product Page Views
  if (omnitureInfo.object != null && omnitureInfo.object.type == 'product') {
    onPageLoadVariables.events[onPageLoadVariables.events.length] = 'event6';
    onPageLoadVariables.variables.channel = 'Products';
    onPageLoadVariables.variables['prop1,eVar1'] = [omnitureInfo.siteID, 'Products'].join('/') + '/';
  }

  // HP Page Views
  if (omnitureInfo.isHomePage) {
    onPageLoadVariables.events[onPageLoadVariables.events.length] = 'event5';
  }

  omniture.setData(onPageLoadVariables.variables, onPageLoadVariables.events);
*/