<?php

/**
 * @file
 * Shows the overview screen with all links to entities.
 */

/**
 * Menu callback: Show the layout list.
 */
function ds_layout_list() {
  $build = array();

  // All entities.
  $rows = array();
  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity) {
    if ((isset($entity['fieldable']) && $entity['fieldable']) || isset($entity['ds_display'])) {

      $entity_rows = array();
      foreach ($entity['bundles'] as $bundle_type => $bundle) {
        $row = array();

        $path = isset($bundle['admin']['real path']) ? $bundle['admin']['real path'] : (isset($bundle['admin']['path']) ? $bundle['admin']['path'] : '');
        if (empty($path)) {
          continue;
        }

        $row[] = $bundle['label'];
        $row[] = l(t('Manage display'), $path . '/display');
        $entity_rows[] = $row;
      }

      if (!empty($entity_rows)) {
        $rows[] = array(array('data' => $entity['label'], 'colspan' => '2', 'style' => 'background-color: #E1E2DC; border: 1px solid #BEBFB9;'));
        $rows = array_merge($rows, $entity_rows);
      }
    }
  }

  if (!empty($rows)) {
    $variables = array(
      'header' => array(),
      'rows' => $rows,
    );
    $build['list'] = array('#markup' => theme('table', $variables));
  }
  else {
    // This will probably never happen.
    $build['list'] = array('#markup' => t('No content found'));
  }

  return $build;
}

/**
 * Emergency page
 */
function ds_emergency() {

  $form['ds_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable adding fields through Display suite'),
    '#description' => t('In case you get an error after configuring a layout with DS which prints something like "Fatal error: Unsupported operand types", you can temporarily disable adding fields from DS by toggling this checkbox. You probably are trying to render an node inside a node, for instance through a view, which is simply not possible. See <a href="http://drupal.org/node/1264386">http://drupal.org/node/1264386</a>.'),
    '#default_value' => variable_get('ds_disable', FALSE),
  );

  return system_settings_form($form);
}
