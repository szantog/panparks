<?php
/*
 *
 */
?>
<?php foreach ($tweets as $delta => $tweet): ?>
  <div id ="tweet-<?php print $delta; ?>" class="tweet">
    <?php print theme('hashtag_counter_tweet', $tweet); ?>
  </div>
<?php  endforeach; ?>
