<div id="container" class="<?php print $page['content']['sidebar_class']; ?>">

  <!-- Wrapper content -->
  <div id="wrapper_content">
    <!-- Header -->
    <div id="header">
      <?php print render( $page['header'] ); ?>
    </div>
    <!-- END Header -->
	  <!-- Wrapper Navigation -->
  <div id="wrapper_navigation">
    <!-- Navigation -->
    <div id="navigation">
      <?php print render( $page['navigation'] ); ?>
    </div>
    <!-- END Navigation -->
  </div>
  <!-- END Wrapper Navigation -->
    <!-- Left Sidebar -->
    <?php if ( !empty( $page['left_sidebar']) ) : ?>
    <div id="left_sidebar">
      <?php print render( $page['left_sidebar'] ); ?>
    </div>
    <?php endif; ?>
    <!-- END Left Sidebar -->
    
     <!-- Right Sidebar -->
    <?php if ( !empty( $page['right_sidebar'] ) ): ?>
    <div id="right_sidebar">
      <?php print render( $page['right_sidebar'] ); ?>
    </div>
    <?php endif; ?>
    <!-- END Right Sidebar -->

    <div class="holder <?php print $page['content']['sidebar_class']; ?>">
      <!-- Content -->
      <?php print drupal_render($tabs) ?>
      <?php if ( !empty( $page['content'] ) ): ?>
        <div id="content">
          <div id="breadcrumb"><?php print $breadcrumb; ?></div>
          <?php print $messages; ?>
          <?php print render( $page['content'] ); ?>
        </div>
      <?php endif; ?>
      <!-- END Content -->
      <!-- Content Bottom -->
      <?php if ( !empty( $page['content_bottom'] ) ): ?>
        <div id="content_bottom">
          <?php print render( $page['content_bottom'] ); ?>
        </div>
      <?php endif; ?>
      <!-- END Content Bottom -->
    </div>
    <div class="clear_both"></div>
  </div>
  <!-- END Wrapper content -->
  <!-- Wrapper Footer -->
  <div id="wrapper_footer">
    <!-- Footer -->
    <?php if ( !empty( $page['footer'] ) ): ?>
    <div id="footer">
      <?php print render( $page['footer'] ); ?>
    </div>
    <?php endif; ?>
    <!-- END Footer --> 
  </div>
  <!-- END Wrapper Footer -->
</div>