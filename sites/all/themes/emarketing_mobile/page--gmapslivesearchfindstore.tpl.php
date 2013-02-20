<?php
$lang = LANGUAGE_NONE;

if (isset($node->language)) {
  $lang = $node->language;
}
$count_tabs = 1;
?>
<div id="wrapper-page">
  <div id="page">
    <!-- Header -->
    <header id="header" role="banner">
      <div class="section clearfix">   
        <!-- Header Region -->
        <div class="section" id="header-region">
          <!-- Logo Image -->
          <?php if ($logo): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>
          <!-- END Logo Image -->
          <?php if(!empty($page['header'])): ?>  
            <?php print render($page['header']); ?>
          <?php endif;?>
        </div> 
        <!-- END Header Region -->
      </div>
    </header>
    <!-- END Header -->

    <?php for ($i = 0; $i < $count_tabs; $i++): ?>
      <!-- Tilte Header -->
        <div class="title-header">
          <p><?php print variable_get('gmapslivesearch_field_buy_now_page_title_mobile'); ?></p>
        </div>
    <?php endfor; ?>
    <!-- END Tilte Header -->
    
    <!-- Carousel -->
    <?php if(!empty($page['carousel']) ): ?>
    <section id="carousel">
      <?php print render($page['carousel']); ?>
    </section>
    <?php endif; ?>
    <!-- END Carousel -->

    <!-- Navigation -->
    <?php if(!empty($page['navigation']) ): ?>
    <nav id="main-navigation">
      <?php print render($page['navigation']); ?>
    </nav>
    <?php endif; ?>
    <!-- END Navigation -->

    <!-- Content -->
    <section id="main-wrapper">    
      <div id="content" class="column" role="main">
        <?php print $messages; ?>
        <?php if ($tabs): ?>
          <div class="tabs"><?php print drupal_render($tabs); ?></div>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </div>
    </section>
    <!-- END Content -->

    <!-- Content Bottom -->
    <?php if(!empty($page['content-bottom']) ): ?>
    <section id="content-bottom">
      <?php print render($page['content-bottom']); ?>
    </section>
    <?php endif; ?>
    <!-- END Content Bottom -->
    
    <!-- Footer -->
    <?php if(!empty($page['footer'])): ?>
    <footer id="footer" role="contentinfo">
      <div class="section" >
        <?php print render($page['footer']); ?>
      </div>  
    </footer>
    <?php endif; ?>
    <!-- END Footer -->
    
    <div id="overlay-mask"></div>
  </div>
</div>
