<?php

/**
 * Export Panpark slideshow to Views.
 */

/**
 * Implements hook_views_data()
 */
function panparks_slideshow_views_data() {
  $data = array();

  $data['panparks_slideshow']['table']['group']  = t('Panparks slideshow');

  $data['panparks_slideshow']['table']['base'] = array(
    'field' => 'slid',
    'title' => t('Panparks slideshow'),
    'help' => t('Panparks slideshow.'),
  );

  // Expose the product title.
  $data['panparks_slideshow']['slid'] = array(
    'title' => t('Slideshow ID'),
    'help' => t('The panparks slideshow ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['panparks_slideshow']['uid'] = array(
    'title' => t('Author'),
    'help' => t('Relate content to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
  );

  $data['panparks_slideshow']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the slideshow was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['panparks_slideshow']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the slideshow was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['panparks_slideshow']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the slideshow is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'enabled-disabled' => array(t('Enabled'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['panparks_slideshow']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight of the panpark slideshow.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['panparks_slideshow']['edit_slideshow'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the slideshow.'),
      'handler' => 'panparks_slideshow_handler_field_link_edit',
    ),
  );
  $data['panparks_slideshow']['delete_slideshow'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the slideshow.'),
      'handler' => 'panparks_slideshow_handler_field_link_delete',
    ),
  );

  return $data;
}
