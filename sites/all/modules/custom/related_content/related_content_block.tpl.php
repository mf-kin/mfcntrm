<div class="related-content-block">
  <ul>
    <?php foreach ($nodes as $node_info) : ?>
    <li>
      <?php print l($node_info['title'], 'node/'. $node_info['nid']) ?>
      <?php foreach ($config['fields'] as $field => $field_info) : ?>
      <div>
        <?php print $node_info[$field .'_value'] ?>
      </div>
      <?php endforeach ?>
    </li>
    <?php endforeach; ?>
  </ul>
</div>