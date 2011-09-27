<?php

/**
 * @file contextual_links_wrapper.tpl.php
 * Default template implementation to display contextual links around image field.
 * */
?>
<div class="<?php print $classes; ?>" <?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print $content ?>
  <?php print render($title_suffix); ?>
</div>
