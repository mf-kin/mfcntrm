
      <?php if ( !empty( $page['content'] ) ): ?>
        <div id="content">
          <?php print $messages; ?>
          <?php print render( $page['content'] ); ?>
        </div>
      <?php endif; ?>
      <!-- END Content -->
      <!-- Content Bottom -->
      <?php if ( !empty( $page['content_bottom'] ) ): ?>
        <div id="content_bottom" class="clearfix">
          <?php print render( $page['content_bottom'] ); ?>
        </div>
      <?php endif; ?>

