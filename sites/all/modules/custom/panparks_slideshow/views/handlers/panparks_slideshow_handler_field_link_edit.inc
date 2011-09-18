<?php

/**
 * Field handler to present a slideshow edit link.
 */
class panparks_slideshow_handler_field_link_edit extends panparks_slideshow_handler_field_link {
  function construct() {
    parent::construct();

    $this->additional_fields['slid'] = 'slid';
  }

  function render($values) {
    // Ensure the user has access to edit this product.
    $slid = $this->get_value($values, 'slid');

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return l($text, 'slideshow/' . $slid, array('query' => drupal_get_destination()));
  }
}
