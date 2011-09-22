Bean (Block Entities Aren't Nodes)
==================================

The bean module was created to have the flexibility of Block Nodes without adding to the node space.

Bean Types
----------

A Bean Type (or Block Type) is a bundle of beans (blocks).  Each Bean type is defined by a ctools plugin and are fieldable.  Currently Bean Types are only defined in hook_bean_plugins().

### Defining a Bean Type

Place this in your mymodule.module file.

```php
/**
 * Implements hook_bean_plugins().
 */
function mymodule_bean_plugins() {
  $plugins = array();
  $plugin_path = drupal_get_path('module', 'mymodule') . '/plugins';

  $plugins['my_plugin'] = array(
    'label' => t('My Title'),
    'handler' => array(
      'path' => $plugin_path,
      'file' => 'plugin.inc',
      'calss' => 'mymodule_plugin',
      'parent' => 'bean',
    ),
  );

  return $plugins;
}
```

Place this in the plugins/plugin.inc file.

```php
/**
 * Define the plugin class
 */
mymodule_plugin extends bean_plugin {
  /**
   * Return an array of the keys of the field and the default values
   */
  public function values() {
    return array(
      'field_1' => t('Field 1'),
      'field_2' => t('Field 2'),
    );
  }

  /**
   * The form to display for the bean
   *
   * @param Bean bean object loaded with either
   * the values from the DB or the defaults.
   *
   * @return array the form element array
   */
  public function form($bean) {
    $form = array();
    $form['field_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Field 1'),
      '#default_value' => $bean->field_1;
    );

    $form['field_2'] = array(
      '#type' => 'textfield',
      '#title' => t('Field 2'),
      '#default_value' => $bean->field_2;
    );

    return $form;
  }

  /**
   * Validate the form
   *
   * @param $values from the form submition
   */
  public function validate($values) {}

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
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {
    return $content;
  }
}
```