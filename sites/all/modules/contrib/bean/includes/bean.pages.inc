<?php

/**
 * @file
 * Bean Functions
 */

/**
 * View the Bean
 */
function bean_view($bean) {
  drupal_set_title($bean->title);
  return $bean->view();
}

/**
 * Edit bean page callback
 */
function bean_edit($bean) {
  return drupal_get_form('bean_form', $bean);
}

/**
 * Add bean page callback
 */
function bean_add($type) {
  $bean = bean_create(array('type' => $type));

  return drupal_get_form('bean_form', $bean);
}

/**
 * Menu Callback to list available bean types to add
 */
function bean_add_page() {
  $bean_types = bean_get_types();

  // Bypass the block/add listing if only one block type is available.
  if (count($bean_types) == 1) {
    $bean_type = array_shift($bean_types);
    drupal_goto('block/add/' . $bean_type->buildURL());
  }

  return theme('bean_add_list', array('content' => $bean_types));
}

function bean_list() {
  $rows = array();

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'b.label', 'sort'=>'asc'),
    'type' => array('data' => t('Type'), 'field' => 'b.type'),
    'operations' => array('data' => t('Operations')),
  );

  $sql_query = db_select('bean', 'b');

  $sql_query->fields('b', array('bid', 'label', 'type'));
  $sql_query = $sql_query->extend('TableSort')->extend('PagerDefault');
  // Add filter
  $filters = bean_get_filter();
  if (empty($filters)) {
    $sql_query->limit(50);
  }
  else {
    $sql_query->limit($filters['per_page']);
    $sql_query->condition('type', $filters['types'], 'IN');
  }

  $beans = $sql_query->orderByHeader($header)->execute();

  $destination = drupal_get_destination();

  $beans = $beans ? $beans : array();

  foreach($beans as $bean) {
    $rows[$bean->bid] = array (
    'title' => array(
      'data' => array(
        '#type' => 'link',
        '#title' => $bean->label,
        '#href' => 'block/' . $bean->bid,
      ),
    ),
    'type' => check_plain($bean->type),
    );

    // Build a list of all the accessible operations for the current bean.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'block/' . $bean->bid . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'block/' . $bean->bid . '/delete',
      'query' => $destination,
    );
    // Render an unordered list of operations links.
    $rows[$bean->bid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__bean_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $bean_filter_form = drupal_get_form('bean_filter_form');
  $output = drupal_render($bean_filter_form);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager');

  return $output;
}

/**
 * Returns HTML for a list of available bean types for bean creation.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: An array of bean types.
 *
 * @ingroup themeable
 */
function theme_bean_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="bean-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l('Add ' . $item->getLabel(), 'block/add/' . str_replace('_', '-', $item->type)) . '</dt>';
    }
    $output .= '</dl>';
  }
  else {
    //$output = '<p>' . t('You have not created any block types yet. Go to the <a href="@create-content">block type creation page</a> to add a new bean type.', array('@create-content' => url('admin/structure/block/types/add'))) . '</p>';
    $output = '<p>' . t('You have not created any block types yet.') . '</p>';
  }
  return $output;
}

/**
 * Bean form
 */
function bean_form($form, &$form_state, $bean) {

  // During initial form build, add the bean entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['bean'])) {
    if (!isset($bean->label)) {
      $bean->label = NULL;
    }
    $form_state['bean'] = $bean;
  }
  else {
    $bean = $form_state['bean'];
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#default_value' => $bean->label,
    '#description' => t('Name that displays in the admin interface'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The Title of the block.'),
    '#default_value' => $bean->title,
  );

  $form['bean'] = array(
    '#type' => 'value',
    '#value' => $bean,
  );

  // Get the Bean type form
  $form += $bean->getForm();

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 200,
    '#submit' => array('bean_form_submit'),
  );
  if (!empty($bean->bid) && user_access('delete any ' . $bean->type . ' bean')) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 201,
      '#submit' => array('bean_form_delete_submit'),
    );
  }

  $form['#validate'][] = 'bean_form_validate';
  if (!isset($form['#submit']) && function_exists($bean->type . '_bean_form_submit')) {
    $form['#submit'][] = $bean->type . '_bean_form_submit';
  }
  $form += array('#submit' => array());

  field_attach_form('bean', $bean, $form, $form_state);
  return $form;
}

/**
 * Validation for bean form
 */
function bean_form_validate($form, &$form_state) {
  $bean = $form_state['values']['bean'];
  $bean->label = $form_state['values']['label'];
  $bean->title = $form_state['values']['title'];

  field_attach_form_validate('bean', $bean, $form, $form_state);
  $form_state['values']['bean'] = $bean;

  $bean->validate($form_state['values'], $form_state);
}

/**
 * Submit function for bean form
 */
function bean_form_submit($form, &$form_state) {
  $bean = bean_form_submit_build_bean($form, $form_state);
  $insert = empty($bean->bid);
  $bean->setValues($form_state['values']);
  field_attach_submit('bean', $bean, $form, $form_state);
  $bean->save();

  $watchdog_args = array('@type' => $bean->type, '%title' => $bean->label);
  $t_args = array('@type' => $bean->typeName(), '%title' => $bean->label);

  if ($insert) {
    watchdog('bean', '@type: added %title.', $watchdog_args, WATCHDOG_NOTICE, $bean->viewURL());
    drupal_set_message(t('@type %title has been created.', $t_args));
  }
  else {
    watchdog('bean', '@type: updated %title.', $watchdog_args, WATCHDOG_NOTICE, $bean->viewURL());
    drupal_set_message(t('@type %title has been updated.', $t_args));
  }
  if ($bean->bid) {
    $form_state['redirect'] = $bean->viewURL();
  }
  else {
    // In the unlikely case something went wrong on save, the bean will be
    // rebuilt and bean form redisplayed the same way as in preview.
    drupal_set_message(t('The block could not be saved.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Updates the form state's bean entity by processing this submission's values.
 *
 * This is the default builder function for the bean form. It is called
 * during the "Save" and "Preview" submit handlers to retrieve the entity to
 * save or preview. This function can also be called by a "Next" button of a
 * wizard to update the form state's entity with the current step's values
 * before proceeding to the next step.
 *
 * @see bean_form()
 * @see node_form_submit_build_node()
 */
function bean_form_submit_build_bean($form, &$form_state) {

  // @todo Legacy support for modules that extend the bean form with form-level
  //   submit handlers that adjust $form_state['values'] prior to those values
  //   being used to update the entity. Module authors are encouraged to instead
  //   adjust the bean directly within a hook_bean_submit() implementation. For
  //   Drupal 8, evaluate whether the pattern of triggering form-level submit
  //   handlers during button-level submit processing is worth supporting
  //   properly, and if so, add a Form API function for doing so.
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);

  $bean = $form_state['bean'];
  entity_form_submit_build_entity('bean', $bean, $form, $form_state);

  foreach (module_implements('bean_submit') as $module) {
    $function = $module . '_bean_submit';
    $function($bean, $form, $form_state);
  }
  return $bean;
}

/**
 * Button submit function: handle the 'Delete' button on the bean form.
 */
function bean_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $bean = $form_state['values']['bean'];
  $form_state['redirect'] = array('block/' . $bean->bid . '/delete', array('query' => $destination));
}

/**
 * Menu callback -- ask for confirmation of bean deletion
 */
function bean_delete_confirm($form, &$form_state, $bean) {
  $form['#bean'] = $bean;

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $bean->label)),
    'block/' . $bean->bid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute bean deletion
 */
function bean_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $bean = $form['#bean'];
    $bean->delete();
    watchdog('block', '@type: deleted %title.', array('@type' => $bean->type, '%title' => $bean->label));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => $bean->type, '%title' => $bean->label)));
  }

  $form_state['redirect'] = '<front>';
}

function bean_filter_form($form, &$form_state) {
  $filters = bean_get_filter();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  $type_options = array();
  foreach (bean_get_types() as $type) {
    $type_options[$type->type] = $type->getLabel();
  }

  $form['filters']['types'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#multiple' => TRUE,
    '#options' => $type_options,
    '#default_value' => $filters['types'],
  );

  $form['filters']['per_page'] = array(
    '#type' => 'select',
    '#title' => t('Items Per Page'),
    '#multiple' => FALSE,
    '#options' => array(20 => '20', 50 => '50', 100 => '100', 200 => '200', 500 => '500'),
    '#default_value' => $filters['per_page'],
  );

  $form['filters']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('bean_filter_submit'),
  );
  $form['filters']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('bean_reset_filter_submit'),
  );

  return $form;
}

/**
 * Submit function for filter
 */
function bean_filter_submit($form, &$form_state) {
  $filters = array(
    'types' => $form_state['values']['filters']['types'],
    'per_page' => $form_state['values']['filters']['per_page'],
  );

  bean_set_filter($filters);

  drupal_goto('admin/content/blocks');

}

function bean_reset_filter_submit($form, &$form_state) {
  bean_reset_filter();

  drupal_goto('admin/content/blocks');
}

/**
 * Set the filter in the session
 */
function bean_set_filter($filter) {
  $_SESSION['bean-filter'] = $filter;
}

/**
 * Get the filter
 */
function bean_get_filter() {
  $filters = !empty($_SESSION['bean-filter']) ? $_SESSION['bean-filter'] : array();
  $types = empty($filters['types']) ? array() : $filters['types'];
  $per_page = empty($filters['per_page']) ? array() : $filters['per_page'];
  return compact('types', 'per_page');
}

/**
 * Reset the filter
 */
function bean_reset_filter() {
  unset($_SESSION['bean-filter']);
}