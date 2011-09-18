<?php

/**
 * Implementation of hook_views_default_views()
 */
function panparks_slideshow_views_default_views() {
  $view = new view;
  $view->name = 'slideshow';
  $view->description = 'Front page slideshow (slider)';
  $view->tag = 'slideshow block';
  $view->base_table = 'panparks_slideshow';
  $view->human_name = 'Slideshow';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'slideshow';
  $handler->display->display_options['style_options']['slideshow_type'] = 'views_slideshow_cycle';
  $handler->display->display_options['style_options']['slideshow_skin'] = 'default';
  $handler->display->display_options['style_options']['skin_info'] = array(
    'class' => 'default',
    'name' => 'Default',
    'module' => 'views_slideshow',
    'path' => '',
    'stylesheets' => array(),
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'slideshow_title' => 0,
    'slideshow_picture' => 0,
    'slideshow_link' => 0,
    'slideshow_body' => 0,
    'slideshow_picture_1' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['top']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['enable'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['type'] = 'views_slideshow_pager_fields';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_fields'] = array(
    'slideshow_body' => 'slideshow_body',
    'slideshow_picture_1' => 'slideshow_picture_1',
    'slideshow_title' => 0,
    'slideshow_picture' => 0,
    'slideshow_link' => 0,
  );
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_pager']['views_slideshow_pager_fields_hover'] = 1;
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['weight'] = '1';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_controls']['type'] = 'views_slideshow_controls_text';
  $handler->display->display_options['style_options']['widgets']['bottom']['views_slideshow_slide_counter']['weight'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['timeout'] = '5000';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['speed'] = '700';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['delay'] = '0';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['pause'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['action_advanced'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['start_paused'] = 1;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['remember_slide_days'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['items_per_slide'] = '1';
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartype'] = 0;
  $handler->display->display_options['style_options']['views_slideshow_cycle']['cleartypenobg'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Panparks slideshow: Title */
  $handler->display->display_options['fields']['slideshow_title']['id'] = 'slideshow_title';
  $handler->display->display_options['fields']['slideshow_title']['table'] = 'field_data_slideshow_title';
  $handler->display->display_options['fields']['slideshow_title']['field'] = 'slideshow_title';
  $handler->display->display_options['fields']['slideshow_title']['label'] = '';
  $handler->display->display_options['fields']['slideshow_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['slideshow_title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['slideshow_title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['slideshow_title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['slideshow_title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_title']['field_api_classes'] = 0;
  /* Field: Panparks slideshow: Big picture */
  $handler->display->display_options['fields']['slideshow_picture']['id'] = 'slideshow_picture';
  $handler->display->display_options['fields']['slideshow_picture']['table'] = 'field_data_slideshow_picture';
  $handler->display->display_options['fields']['slideshow_picture']['field'] = 'slideshow_picture';
  $handler->display->display_options['fields']['slideshow_picture']['ui_name'] = 'Panparks slideshow: Big picture';
  $handler->display->display_options['fields']['slideshow_picture']['label'] = '';
  $handler->display->display_options['fields']['slideshow_picture']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['external'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['alter']['html'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['slideshow_picture']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['slideshow_picture']['hide_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['empty_zero'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_picture']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['slideshow_picture']['settings'] = array(
    'image_style' => 'slideshow_big',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['slideshow_picture']['field_api_classes'] = 0;
  /* Field: Panparks slideshow: Link */
  $handler->display->display_options['fields']['slideshow_link']['id'] = 'slideshow_link';
  $handler->display->display_options['fields']['slideshow_link']['table'] = 'field_data_slideshow_link';
  $handler->display->display_options['fields']['slideshow_link']['field'] = 'slideshow_link';
  $handler->display->display_options['fields']['slideshow_link']['label'] = '';
  $handler->display->display_options['fields']['slideshow_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['slideshow_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['slideshow_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['slideshow_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['slideshow_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['slideshow_link']['field_api_classes'] = 0;
  /* Field: Panparks slideshow: Body */
  $handler->display->display_options['fields']['slideshow_body']['id'] = 'slideshow_body';
  $handler->display->display_options['fields']['slideshow_body']['table'] = 'field_data_slideshow_body';
  $handler->display->display_options['fields']['slideshow_body']['field'] = 'slideshow_body';
  $handler->display->display_options['fields']['slideshow_body']['label'] = '';
  $handler->display->display_options['fields']['slideshow_body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['slideshow_body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['slideshow_body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['slideshow_body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['slideshow_body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['slideshow_body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_body']['field_api_classes'] = 0;
  /* Field: Panparks slideshow: Small picture */
  $handler->display->display_options['fields']['slideshow_picture_1']['id'] = 'slideshow_picture_1';
  $handler->display->display_options['fields']['slideshow_picture_1']['table'] = 'field_data_slideshow_picture';
  $handler->display->display_options['fields']['slideshow_picture_1']['field'] = 'slideshow_picture';
  $handler->display->display_options['fields']['slideshow_picture_1']['ui_name'] = 'Panparks slideshow: Small picture';
  $handler->display->display_options['fields']['slideshow_picture_1']['label'] = '';
  $handler->display->display_options['fields']['slideshow_picture_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['slideshow_picture_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['slideshow_picture_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['slideshow_picture_1']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['slideshow_picture_1']['settings'] = array(
    'image_style' => 'slideshow_small',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['slideshow_picture_1']['field_api_classes'] = 0;
  /* Sort criterion: Panparks slideshow: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'panparks_slideshow';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Filter criterion: Panparks slideshow: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'panparks_slideshow';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['display_description'] = 'Slideshow front page block';

  $views[$view->name] = $view;

  return $views;
}
