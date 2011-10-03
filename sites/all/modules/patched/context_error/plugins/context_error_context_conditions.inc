<?php

/**
 * Expose the 404 error page as the context condition.
 */
class context_error_context_condition_error extends context_condition {
  function condition_values() {
    $values = array();
    $values['403'] = t('403 error');
    $values['404'] = t('404 error');
    return $values;
  }

  function is_error_known($status) {
    $known_errors = array(
        '403' => '403 Forbidden',
        '404' => '404 Not Found',
    );

    return (in_array($status, $known_errors)) ? TRUE : FALSE;

  }

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {

      $headers = drupal_get_http_header();

      foreach ($this->get_contexts() as $context) {
        $errors = $this->fetch_from_context($context, 'values');

        foreach ($errors as $error) {
          if($this->is_error_known($headers['status'])) {
            $this->condition_met($context);
          }
        }
      }
    }
  }
}
