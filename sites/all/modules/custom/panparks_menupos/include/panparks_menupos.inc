<?php
/**
 * Menu callback: confirms mass deletion of rule.
 */
function panparks_menupos_massdelete($form, &$form_state) {
  return confirm_form($form, t('Are you sure you want to delete all menu position rule?'), 'admin/structure/menu-position', NULL, t('Delete'), t('Cancel'));
}

/**
 * Handles form submission for menu_position_delete_rule_form().
 */
function panparks_menupos_massdelete_submit($form, &$form_state) {
  $rules = db_query('SELECT rid FROM {menu_position_rules} ORDER BY weight, rid');
  $delta = $rules->rowCount();
  if ($delta == 0) {
    drupal_set_message(t('No rules.'));

  }
  else {
    module_load_include('inc', 'menu_position', 'menu_position.admin');
    foreach ($rules as $rule) {
    menu_position_delete_rule($rule->rid);
    }
    drupal_set_message(t('The all menu position rule has been deleted.'));
  }

  $form_state['redirect'] = 'admin/structure/menu-position';
}

/**
 * menu_position condition callback
 */
function panparks_menupos_menu_position_condition_parknoderef($variables) {
  if ($variables['enabled']) {
    // $node is dynamically added and may not exist.
    $node = isset($variables['context']['node']) ? $variables['context']['node'] : NULL;

    if (isset($node->{$variables['field']})) {
      $field = field_get_items('node', $node, $variables['field']);
      if (!empty($field) && $field['0']['nid'] == $variables['nid']) {
//        dpm($variables);
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Add form element menu_position rule.
 */
function panparks_menupos_menu_position_rule_parknoderef_form(&$form, &$form_state) {
  // If this is an existing rule, load the variables stored in the rule for this parknoderef.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['parknoderef']) ? $form_state['#menu-position-rule']['conditions']['parknoderef'] : array();

  $form['conditions']['parknoderef'] = array(
    '#type' => 'fieldset',
    '#title' => t('Park reference'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['conditions']['parknoderef']['noderef_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => !empty($variables['enabled']) ? $variables['enabled'] : 0,
    '#description' => t('Apply this rule only on pages written in lolspeak.'),
  );

  $form['conditions']['parknoderef']['noderef_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Node reference field name'),
    '#default_value' => !empty($variables['field']) ? $variables['field'] : '',
  );

  $form['conditions']['parknoderef']['noderef_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node reference value (nid)'),
    '#default_value' => !empty($variables['nid']) ? $variables['nid'] : '',
  );

  $form['#validate'][] = 'panparks_menupos_menu_position_rule_parknoderef_form_validate';

  // Add a submit handler.
  $form['#submit'][] = 'panparks_menupos_menu_position_rule_parknoderef_form_submit';
}

function panparks_menupos_menu_position_rule_parknoderef_form_validate(&$form, &$form_state) {
  if ($form_state['values']['noderef_enabled']) {
    if (empty($form_state['values']['noderef_field'])) {
      form_set_error('noderef_field', t("Invalid field name."));
    }

    if (!is_numeric($form_state['values']['noderef_nid'])) {
      form_set_error('noderef_nid', t("Invalid node id."));
    }
  }
}

function panparks_menupos_menu_position_rule_parknoderef_form_submit($form, &$form_state) {
  if ($form_state['values']['noderef_enabled']) {
    $variables = array(
      'enabled' => $form_state['values']['noderef_enabled'],
      'field' => $form_state['values']['noderef_field'],
      'nid' => $form_state['values']['noderef_nid'],
    );
    $form_state['values']['conditions']['parknoderef'] = $variables;
  }
}

/**
 * Set  menu position some panparks CT
 */
function panparks_menupos_set_menu_position($node) {
  // set menu position this content type
  $mp_ct = array('park', 'accommodation', 'tours_and_holdiays');

  if (in_array($node->type, $mp_ct)) {
    if (!isset($node->menu)) {
      menu_node_prepare($node);
    }

    try {
      $conditions = array(
        'content_type' => array(
          'content_type' => array('member' => $node->type)
        ),
        'pages' => array(
          'pages' => empty($node->path['alias']) ? 'node/' . $node->nid : $node->path['alias'],
        ),
      );

      if ($node->type == 'park') {
        $conditions['pages']['pages'] .= '/*';
        if (isset($node->menu) && $node->menu['mlid'] == 0) {
          throw new Exception('Park is not included in the menu.');
        }
      }

      // set menu position rule
      $rule = array(
        'admin_title' => $node->type . ' nid: ' . $node->nid,
        'conditions'  => $conditions,
        'menu_name'   => $node->menu['menu_name'],
        'plid'        => $node->menu['mlid'],
      );

      if ($node->type == 'accommodation' || $node->type == 'tours_and_holdiays') {
        $park_field = ($node->type == 'tours_and_holdiays') ? 'field_parks' : 'field_park';
        $park_id = field_get_items('node', $node, $park_field);
        if (empty($park_id)) {
          throw new Exception('No reference to park.');
        }

        $park = node_load($park_id['0']['nid']);
        if (empty($park)) {
          throw new Exception('Park node is empty.');
        }

        menu_node_prepare($park);
        if (isset($park->menu) && $park->menu['mlid'] === '0') {
          throw new Exception('Park reference is not included in the menu.');
        }

        // set menu position rule
        unset($rule['conditions']['pages']);
        $rule['conditions']['parknoderef'] = array(
          'enabled' => 1,
          'field' => $park_field,
          'nid' => $park->nid,
        );

        $rule['admin_title'] = $node->type . ' - ' . $park_field . ' nid: ' . $park->nid . ' mlid: ' . $park->menu['mlid'];
        $rule['menu_name'] = $park->menu['menu_name'];
        $rule['plid'] = $park->menu['mlid'];
//        dpm($rule);
      }

      // rule id is loaded
      $rid = db_select('menu_position_rules', 'mp')
        ->fields('mp', array('rid'))
        ->condition('admin_title', $rule['admin_title'], '=')
        ->execute()
        ->fetchField();

      if ($rid) {
        // update menu position rule
        $rule['rid'] = $rid;
        module_load_include('inc', 'menu_position', 'menu_position.admin');
        menu_position_edit_rule($rule);
      }
      else {
        // create menu position rule
        module_load_include('inc', 'menu_position', 'menu_position.admin');
        menu_position_add_rule($rule);
      }
    }
    catch (Exception $e) {
      watchdog_exception('panparks_menupos', $e);
    }
    //dsm(get_defined_vars());
  }
}

