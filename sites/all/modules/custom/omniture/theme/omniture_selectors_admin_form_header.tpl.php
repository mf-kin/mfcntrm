<h1>General Selectors configuration</h1>
<p>From within this page, it's possible to create specific trackings for omniture, being able to send both variables and events. Enter all fields without quotes, they'll be treated as strings by default.</p>
<p>All configurations will be defined as drupal behaviors in Drupal.behaviors.omnitureSelectors.</p>
<p>The configuration aims to be as simple as possible and is defined as follows:</p>

<h3>Configurable fields:</h3>
<ul>
  <li><strong>jQuery Selector:</strong> Just as the name says, a jQuery selector to identify to which elements the event shall be binded to.</li>
  <li><strong>Trigger on:</strong> Which event will trigger the omniture tracking (e.g., click, change, etc.)</li>
  <li><strong>Code snippet:</strong> This code will be inserted inside the binded event body. Which means you can process anything you want here before sending data to omniture. This field have access to <strong>$</strong> jQuery variable and is normally used for discovering things such as the title of the clicked link, getting DOM parent elements information and so on.</li>
  <li><strong>Variables Values:</strong> This field can contain javascript code, here you can use jQuery in order to get a value (e.g., $(this).attr('title')). You can access the omnitureInfo object defined in javascript by default, see bellow for more info. So, for example, if you need to track clicks in a "learn more" button, this field can be set to "omnitureInfo.pageTitle + ' : learn more'". You can access all variables defined in 'Code snippet' field as well.</li>
  <li><strong>Variables:</strong> Which variables you need to send separated by comma, all the variables will be set to the value specified in 'variables values' field. Notice that the 'events' variable does not need to be set here, this is already handled by the events field. For example, if you want to send eVar3, prop3, eVar4, prop4 and the pageName at the same time, fill this field with "eVar3,prop3,eVar4,prop4,pageName". Everything else will be handled by the module. It works by extending objects with the proper fields.</li>
  <li><strong>Events:</strong> Works the same way as the 'variables' field. Place all events separated by comma you want to send when the event of the selector is triggered (e.g., "event1,event5,event7").</li>
  <li><strong>Active:</strong> Whether the selector is active. Only active selectors will generate code, it's a nice way to avoid deleting selectors in order to remove it from the page.</li>
</ul>

<h5>OmnitureInfo object</h5>
<p>The omnitureInfo object is defined in javascript by default by the module, and it contains some page information. It can be accessed both inside 'variables values' and 'code snippet' fields. It has the following information:</p>
<ul>
  <li><strong>omnitureInfo.siteID           :</strong> string,  // defined siteID</li>
  <li><strong>omnitureInfo.pageNotFoundTitle:</strong> string,  // defined title for page not found</li>
  <li><strong>omnitureInfo.isHomePage       :</strong> boolean, // whether it is the home page</li>
  <li><strong>omnitureInfo.pageTitle        :</strong> string,  // title of current page</li>
  <li><strong>omnitureInfo.breadcrumb       :</strong> [],      // an array with the breadcrumb of current page</li>
  <li><strong>omnitureInfo.channel          :</strong> string   // channel of current page</li>
  <li><strong>omnitureInfo.object.type      :</strong> string,  // content type of node being viewd, null if it's not a node</li>
  <li><strong>omnitureInfo.object.title     :</strong> string   // title of the node</li>
</ul>