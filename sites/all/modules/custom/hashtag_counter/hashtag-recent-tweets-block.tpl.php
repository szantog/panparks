<?php

//kpr($tweets); 

foreach ($tweets as $tweet) {
    print '<div class="tweet">';
        print '<div class="tweet-username">' . $tweet->from_user . '</div>';
        print '<div class="tweet-text">' . $tweet->text . '</div>';
        print '<div class="tweet-created-at">' . date('Y-m-d H:i:s', strtotime($tweet->created_at)) . '</div>';
    print '</div>';
}
