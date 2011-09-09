<?php
/**
 * @file
 * Chaos Tools module integration.
 */

function textimage_ctools_export_list() {
  $presets = array();
  foreach (textimage_get_presets() as $preset) {
    $presets[$preset->name] = $preset->name;
  }
  return $presets;
}

function textimage_ctools_export_load($name) {
  $preset = (object) _textimage_preset_load($name);
  unset($preset->pid);
  return $preset;
}
