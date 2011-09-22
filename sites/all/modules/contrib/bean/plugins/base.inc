<?php

/**
 * @file
 * Base Plugin Class
 */

if (!interface_exists('bean_type_plugin_interface')) {
  require_once drupal_get_path('module', 'bean') . '/includes/bean.core.inc';
}

abstract class bean_plugin implements bean_type_plugin_interface {
  protected $plugin_info;
  public $type;

  /**
   * Get Plugin info
   */
  public function getInfo($key = NULL) {
    if (!empty($key) && isset($this->plugin_info[$key])) {
      return $this->plugin_info[$key];
    }

    return $this->plugin_info;
  }

  /**
   * Build the URL string
   */
  public function buildURL() {
    return str_replace('_', '-', $this->type);
  }

  /**
   * Get the label
   */
  public function getLabel() {
    return $this->getInfo('label');
  }

  public function __construct($plugin_info) {
    $this->plugin_info = $plugin_info;
    $this->type = $plugin_info['name'];
  }

  /**
   * Add a Bean to the plugin
   */
  public function setBean($bean) {
    $this->bean = $bean;
  }

  /**
   * Is the bean type editable
   */
  public function isEditable() {
    return $this->getInfo('editable');
  }

  /**
   * THe plugin submit function
   */
  public function validate($values, &$form_state) {}

  /**
   * Return the block content.
   *
   * @param $bean
   *   The bean object being viewed.
   * @param $content
   *   The default content array created by Entity API.  This will include any
   *   fields attached to the entity.
   * @param $view_mode
   *   The view mode passed into $entity->view().
   * @return
   *   Return a renderable content array.
   */
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {}
}

/**
 * Example Implementation
 */
class bean_default extends bean_plugin {
  public function values() {
    return array();
  }

  public function form($bean) {
    return array();
  }

  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {
    return $content;
  }
}
