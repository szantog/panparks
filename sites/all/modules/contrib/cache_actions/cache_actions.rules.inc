<?php

/**
 * @file
 * This file provides the rules integration for this module.
 */

/**
 * Implementation of hook_rules_action_info().
 */
function cache_actions_rules_action_info() {
  // Actions that works for everyone
  $actions = array(
    'cache_actions_action_clear_cache' => array(
      'label' => t('Clear cache bins'),
      'parameter' => array(
        'tables' => array(
          'type' => 'list<text>',
          'label' => t('Cache Bins'),
          'save' => TRUE,
          'options list' => '_cache_actions_get_cache_bins',
          'restriction' => 'input',
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_cache',
    ),
    'cache_actions_action_clear_cache_cid' => array(
      'label' => t('Clear a specific cache cid'),
      'parameter' => array(
        'bin' => array(
          'type' => 'text',
          'label' => t('Cache bin'),
          'description' => t('The cache table where the cid is'),
          'options list' => '_cache_actions_get_cache_bins',
          'restriction' => 'input',
          'save' => TRUE,
         ),
        'cid' => array(
          'type' => 'text',
          'label' => t('Cache key'),
          'description' => t('The key to clear'),
          'save' => TRUE,
         ),
        'wildcard' => array(
          'type' => 'boolean',
          'label' => t('Use wildcard'),
          'description' => t('Use wildcards. This means you will be clearing all cache keys that partially matches.'),
          'restriction' => 'input',
          'save' => TRUE,
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_cache_cid'
    ),
    'cache_actions_action_clear_css_js_cache' => array(
      'label' => t('Clear the css and js cache'),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_css_js_cache',
    ),
  );

  // If the Views module is available, then we can clear the cache of individual views.
  if (module_exists('views')) {
    $actions['cache_actions_action_clear_views_cache'] = array(
      'label' => t('Clear the cache for specific views'),
      'parameter' => array(
        'view' => array(
          'type' => 'list<text>',
          'label' => t('Views'),
          'description' => t('The cache of the selected views will be cleared.'),
          'options list' => '_cache_actions_get_views_list',
          'save' => TRUE,
          'restriction' => 'input',
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_views_cache',
    );

    $actions['cache_actions_action_clear_views_display_cache'] = array(
      'label' => t('Clear the cache for views displays'),
      'parameter' => array(
        'displays' => array(
          'type' => 'list<text>',
          'label' => t('Displays'),
          'description' => t('The cache of the selected displays will be cleared'),
          'options list' => '_cache_actions_get_views_displays',
          'restriction' => 'input',
          'save' => TRUE,
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_views_display_cache',
    );
  }

  // If the page manager module is available, then we can clear panels stuff.
  if (module_exists('page_manager')) {
    $actions['cache_actions_action_clear_panels_pane_cache'] = array(
      'label' => t('Clear the cache for panel page panes'),
      'parameter' => array(
        'panes' => array(
          'type' => 'list<text>',
          'label' => t('Panes'),
          'description' => t('Specify the panes to invalidate here.'),
          'options list' => '_cache_actions_get_panes',
          'restriction' => 'input',
          'save' => TRUE,
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_panels_pane_cache',
    );
  }

  if (module_exists('panels_mini')) {
    $actions['cache_actions_action_clear_panels_mini_pane_cache'] = array(
      'label' => t('Clear the cache for mini panel panes'),
      'parameter' => array(
        'panes' => array(
          'type' => 'list<text>',
          'label' => t('Panel panes'),
          'options list' => '_cache_actions_get_mini_panel_panes',
          'description' => t('Specify the mini panel panes you want to invalidate.'),
          'save' => TRUE,
          'restriction' => 'input',
        ),
      ),
      'group' => t('Cache Actions'),
      'base' => 'cache_actions_action_clear_panels_mini_pane_cache'
    );
  }
  return $actions;
}

/**
 * Get all existing handlers from page manager. This is the ugliest function
 * in the whole module =)
 * @param bool $panes set this to true if you want the function to return
 * an array consisting of the panels and the panes.
 * @return the handlers.
 */
function _cache_actions_get_panels_handlers() {
  $available_handlers = array();
  // First, get all tasks. This corresponds to all types of page manager pages
  // that can be used, for for instance pages, node view, node edit...
  $tasks = page_manager_get_tasks();
  foreach ($tasks as $task) {
    // Subtasks are tasks that are created under the primary tasks, for instance
    // a custom page the user has created.
    $subtasks = page_manager_get_task_subtasks($task);
    // If we have subtasks, then that's what we're after.
    if (count($subtasks)) {
      foreach ($subtasks as $subtask) {
        // Subtasks have handlers. These can for instance correspond to a panel variant.
        $handlers = page_manager_load_task_handlers($task, $subtask['name']);
        foreach ($handlers as $handler) {
          // handlers have plugins, in this case we need to get the plugin for
          // this handler.
          $plugin = page_manager_get_task_handler($handler->handler);
          $title = page_manager_get_handler_title($plugin, $handler, $task, $subtask['name']);
          $key = 'subtask:' . $handler->name . ':'. $handler->task . ':'. $handler->subtask;
          // Fetch available panes.
          $handler_panes = _cache_actions_load_panes($handler, $title);
          foreach ($handler_panes as $pane_key => $handler_pane) {
            $available_handlers[$title][$key . ':' . $pane_key] = $handler_pane;
          }
        }
      }
    }
    // Otherwise let's use the task.
    else {
      $handlers = page_manager_load_task_handlers($task);
      if (count($handlers)) {
        foreach ($handlers as $handler) {
          $plugin = page_manager_get_task_handler($handler->handler);
          $title = page_manager_get_handler_title($plugin, $handler, $task, $task['name']);
          // If not, then we have an in-code display. Save off the name, so we can get it.
          $key = 'task:' . $handler->name . ':'. $handler->task;
          // Fetch available panes.
          $handler_panes = _cache_actions_load_panes($handler, $title);
          foreach ($handler_panes as $pane_key => $handler_pane) {
            $available_handlers[$handler->task . ': ' . $title][$key . ':' . $pane_key] = $handler_pane;
          }
        }
      }
    }
  }
  // Otherwise just return the handlers.
  return $available_handlers;
}

function _cache_actions_get_page_variants() {
  $data = _cache_actions_get_panels_handlers(FALSE);
  return $data;
}

/**
 * Get Panel Panes.
 */
function _cache_actions_get_panes() {
  return _cache_actions_get_panels_handlers();
}

function _cache_actions_get_mini_panel_panes() {
  ctools_include('plugins', 'panels');
  $available_panes = array();
  $mini_panels = panels_mini_load_all();
  foreach ($mini_panels as $mini_panel) {
    $i = 1;
    foreach ($mini_panel->display->content as $pane) {
      // The panes must have rule-based caching on in order for invalidation
      // to work properly.
      if (isset($pane->cache['method']) && $pane->cache['method'] == 'rules') {
        $pane_title = panels_get_pane_title($pane);
        if (is_numeric($pane->pid)) {
          $pane_id = 'new-' . $i;
        }
        else {
          $pane_id = $pane->pid;
        }
        $available_panes[$mini_panel->name][$mini_panel->name . ':' . $pane_id] = $pane_title;
      }
      $i++;
    }
  }
  return $available_panes;
}

function _cache_actions_load_panes($handler) {
  ctools_include('plugins', 'panels');
  $available_panes = array();
  if (isset($handler->did)) {
    $display = panels_load_display($handler->did);
  }
  else {
    $display = $handler->conf['display'];
  }
  if (isset($display->content)) {
    // We are going to fake panel pane names for displays which are not in code.
    $i = 1;
    foreach ($display->content as $pane) {
      if (isset($pane->cache['method']) && $pane->cache['method'] == 'rules') {
        // We can't satisfy a context here, so we only get the title if no context is set.
        if (empty($pane->configuration['context'])) {
          $pane_title = panels_get_pane_title($pane);
        }
        else {
          $pane_title = $pane->subtype;
        }
        if (is_numeric($pane->pid)) {
          $pane->pid = 'new-' . $i;
        }
        $available_panes[$pane->pid] = $pane_title;
      }
      $i++;
    }
  }
  return $available_panes;
}

/**
 * Get a display out of a key specified in the UI.
 * @param string $key the key
 * @return panels_display a display.
 */
function _cache_actions_get_display_from_key($key) {
  $info = explode(':', $key);
  if ($info[0] == 'task') {
    list(, $handler, $task) = $info;
    $subtask = $task;
  }
  else {
    list(, $handler, $task, $subtask) = $info;
  }
  $task = page_manager_get_task($task);
  $handler = page_manager_load_task_handler($task, $subtask, $handler);
  // In-code handlers have the displays attached to them.
  if (isset($handler->conf['display'])) {
    $display = $handler->conf['display'];
  }
  // Otherwise we're dealing with a database display.
  elseif (isset($handler->conf['did'])) {
    $display = panels_load_display($handler->conf['did']);
  }
  _cache_actions_get_cache_key($handler, $display);
  return $display;
}

/**
 * Clear the cache of panes in a mini panel.
 * @param string $handler the handler
 * @param array $panes the panes to clear.
 */
function cache_actions_action_clear_panels_pane_cache($panes) {
  if (module_exists('page_manager')) {
    ctools_include('plugins', 'panels');
    $displays = array();
    if (is_array($panes)) {
      foreach ($panes as $key) {
        $display = _cache_actions_get_display_from_key($key);
        $pane = _cache_actions_get_pane_from_key($key);
        $i = 1;
        foreach ($display->content as $panel_pane) {
          if (is_numeric($panel_pane->pid)) {
            $pid = 'new-' . $i;
          }
          else {
            $pid = $panel_pane->pid;
          }
          if ($pid == $pane && !empty($panel_pane->cache['method'])) {
            _cache_actions_clear_pane_cache($panel_pane, $display);
          }
          $i++;
        }
      }
    }
  }
  return FALSE;
}

function _cache_actions_get_pane_from_key($key) {
  $info = explode(':', $key);
  if ($info[0] == 'task') {
    return $info[3];
  }
  else {
    return $info[4];
  }
}

/**
 * Create a unique cache key in the same way as panels does for it's
 * internal cache. This is a sure way to get unique cache ids for both
 * in-code displays and database displays.
 * @param $handler
 * @param $display
 */
function _cache_actions_get_cache_key($handler, &$display) {
  $task_name = page_manager_make_task_name($handler->task, $handler->subtask);
  $display->cache_key = 'panel_context:' . $task_name . ':' . $handler->name;
}

/**
 * Clear the content of a pane.
 * @param $pane the pane to clear
 * @param $display the display it belongs to.
 */
function _cache_actions_clear_pane_cache($pane, $display) {
  // Clearing panes can't be used with any other plugin than the rules plugin.
  if ($pane->cache['method'] == 'rules') {
    $function = panels_plugin_get_function('cache', $pane->cache['method'], 'cache clear');
    // Specify the pane to clear. This is specific for the rules plugin.
    $display->clear_pane = $pane;
    if ($function) {
      $function($display);
    }
  }
}

/**
 * Clear the cache of panes in a mini panel.
 * @param string $handler the handler
 * @param array $panes the panes to clear.
 */
function cache_actions_action_clear_panels_mini_pane_cache($panes) {
  if (module_exists('panels_mini')) {
    ctools_include('plugins', 'panels');
    foreach ($panes as $pane) {
      list($mini, $pane_id) = explode(':', $pane);
      $mini = panels_mini_load($mini);
      if (is_object($mini)) {
        // We need to add the owner name, since this is included when it is cached.
        $i = 1;
        $mini->display->owner->name = $mini->name;
        foreach ($mini->display->content as $pane) {
          if (is_numeric($pane->pid)) {
            $pid = 'new-' . $i;
          }
          else {
            $pid = $pane->pid;
          }
          if ($pane_id == $pid) {
            _cache_actions_clear_pane_cache($pane, $mini->display);
          }
          $i++;
        }
      }
    }
  }
  return FALSE;
}

/**
 * Get a a keyed array of views with machine name as key and human readable name
 * as value.
 * @return array an array of views names
 */
function _cache_actions_get_views_list() {
  $views = views_get_all_views();
  $views_names = array();
  foreach ($views as $view) {
    $views_names[$view->name] = $view->name;
  }
  return $views_names;
}

/**
 * Get all views and their displays. This is a callback
 * for an options list.
 */
function _cache_actions_get_views_displays() {
  $views = views_get_all_views();
  $displays = array();
  foreach ($views as $view) {
    $displays[$view->name] = array();
    foreach ($view->display as $display) {
      // Only list views that actually is shown.
      if ($display->id != 'default') {
        $displays[$view->name][$view->name . ':' . $display->id] = (!empty($display->title) ? $display->title : $display->id);
      }
    }
  }
  return $displays;
}

/**
 * This action clears the cache of a specific view.
 * @param array $views the views to clear.
 */
function cache_actions_action_clear_views_cache($views) {
  if (module_exists('views')) {
    views_include_handlers();
    module_load_include('inc', 'views', 'plugins/views_plugin_cache');
    // Is this an array?
    if (is_array($views)) {
      foreach ($views as $view) {
        _cache_actions_clear_view($view);
      }
    }
    else {
      // Otherwise it's probably an old rule from 1.x, let's just handle that.
      _cache_actions_clear_view($views);
    }
  }
  return FALSE;
}

/**
 * This action clears the cache of a specific view display.
 * @param array $views the views to clear.
 */
function cache_actions_action_clear_views_display_cache($views) {
  if (module_exists('views')) {
    $loaded_views = array();
    views_include_handlers();
    module_load_include('inc', 'views', 'plugins/views_plugin_cache');
    if (is_array($views)) {
      foreach ($views as $entry) {
        list($view_name, $display_id) = explode(':', $entry);
        // Let´s make sure we don't load views unnecessarily.
        if (!isset($loaded_views[$view_name])) {
          $loaded_views[$view_name] = views_get_view($view_name);
        }
        // Check to see that the display has a caching plugin. If it doesn't add the default caching plugin.
        $display = $loaded_views[$view_name]->display[$display_id];
        if (!isset($display->display_options['cache']['type'])) {
          $display->display_options['cache']['type'] = $loaded_views[$view_name]->display['default']->display_options['cache']['type'];
        }
        // And then clear the cache.
        _cache_actions_clear_view_display($loaded_views[$view_name], $display);
      }
    }
  }
  return FALSE;
}

/**
 * Clear out a view and it's displays.
 * @param string $view the view machine name
 */
function _cache_actions_clear_view($view) {
  $view = views_get_view($view);
  if (is_object($view)) {
    // Go through all displays and clear the cache.
    foreach ($view->display as $display) {
      // If we don't have our own cache plugin, then we need to copy
      // the cache settings from default.
      if (!isset($display->display_options['cache']) && isset($view->display['default'])) {
        $display->display_options['cache'] = $view->display['default']->display_options['cache'];
      }
      _cache_actions_clear_view_display($view, $display);
    }
  }
  else {
    watchdog('cache_actions', "The view %view doesn't exist.
    You need to look over your rules and see if there's any rules
    involving that view.", array('%view' => $handler), WATCHDOG_ERROR);
  }
}
/**
 * Clear out a specific display in a view.
 */
function _cache_actions_clear_view_display($view, $display) {
  // We use the cache plugin to clear the cache.
  $view->set_display($display->id);
  $cache_plugin = views_get_plugin('cache', $display->display_options['cache']['type']);
  // If we have a cache plugin, then initiate it and flush the cache.
  if (isset($cache_plugin)) {
    $cache_plugin->init($view, $display);
    $cache_plugin->cache_flush();
  }
}
/**
 * This action that clears all cache bins specified.
 * @param $bins the bins to be cleared
 */
function cache_actions_action_clear_cache($bins) {
  $cache_bins = _cache_actions_get_cache_bins();
  foreach ($bins as $bin) {
    if (in_array($bin, $cache_bins)) {
      cache_clear_all('*', $bin, TRUE);
    }
  }
  // Return False to avoid problems with rules trying to run save.
  return FALSE;
}

/**
 * This action clears a specific cache sid in a cache bin.
 * @param $table the table where the cid should reside.
 * @param $cid the cache cid to clear
 * @param $wildcard wether or not to use wildcard invalidation.
 */
function cache_actions_action_clear_cache_cid($table, $cid, $wildcard) {
  cache_clear_all($cid, $table, $wildcard);
  return FALSE;
}

/**
 * This action clears the css and js cache.
 */
function cache_actions_action_clear_css_js_cache() {
  // Change query-strings on css/js files to enforce reload for all users.
  _drupal_flush_css_js();
  drupal_clear_js_cache();
  drupal_clear_css_cache();
  return FALSE;
}

/**
 * Get all cache bins available.
 */
function _cache_actions_get_cache_bins() {
  $cache_bins = array('cache' => 'cache', 'cache_block' => 'cache_block',
    'cache_filter' => 'cache_filter', 'cache_page' => 'cache_page');
  foreach (module_invoke_all('flush_caches') as $cache_bin) {
    $cache_bins[$cache_bin] = $cache_bin;
  }
  return $cache_bins;
}
