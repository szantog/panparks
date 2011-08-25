<?php

/**
 * @file
 * Administrative page callbacks for the Wysiwyg Template module.
 */

/**
 * Wysiwyg template overview page - view templates and edit them
 */
function wysiwyg_template_overview(){
  $rows = array();
  // load the templates and display them on the admin form
  $templateResults = db_query("SELECT * FROM {wysiwyg_templates}");
  foreach ($templateResults as $template){
    $row = array();
    $row[] = $template->title;
    $row[] = $template->description;
    $row[] = l(t('edit'), "admin/config/content/wysiwyg-templates/edit/" . $template->tid);
    $row[] = l(t('delete'), "admin/config/content/wysiwyg-templates/delete/" . $template->tid);
    
    $rows[] = $row;
  }
  
  if (empty($rows)){
    $rows[] = array(array(
      'data' => t('No templates available.'), 
      'colspan' => 4
    ));
  }
  $rows[] = array(array(
    'data' => l(t('Create a new template'), 'admin/config/content/wysiwyg-templates/add'),
    'colspan' => 4,
  ));
  
  $header = array(t('Name'), t('Description'), array('data' => t('Operations'), 'colspan' => 2));
  
  $help = t('<p>The Wysiwyg Template module allows you to create templates to be used with a Wysiwyg editor of your choice. The template button for the editor will need to be enabled from the !profiles settings. The currently supported editors are FCK Editor, CK Editor and TinyMCE.</p>', array('!profiles' => l(t('WYSIWYG Profile'), 'admin/config/content/filters')));
  
  return $help . theme('table', array(
    'header' => $header,
    'rows' => $rows, 
    'id' => 'wysiwyg_template'
  ));
}

/**
 * Wysiwyg template create/modify form
 */
function wysiwyg_template_template_form($something, $form_state) {
  if(isset($form_state['confirm_delete'])){
    // Rebuild the form to confirm term deletion.
    $form['tid'] = array('#type' => 'value', '#value' => $form_state['values']['tid']);
    $form['delete'] = array('#type' => 'value', '#value' => true);
    return confirm_form($form, 
      t('Are you sure you want to delete the template %title?', array('%title' => $form_state['values']['title'])),
      'admin/content/wysiwyg-template',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }

  if(isset($form_state['build_info']['args'][0])){
    // add the current values as defaults to the form, if editing an existing item
    $form_state['values'] = $form_state['build_info']['args'][0];
  }
  
  $form = array();
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Template Name'),
    '#default_value' => isset($form_state['values']['title']) ? $form_state['values']['title'] : '',
    '#description' => t('Select a name for this template.'),
    '#maxlength' => 80,
    '#required' => true
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Template Description'),
    '#default_value' => isset($form_state['values']['description']) ? $form_state['values']['description'] : '',
    '#description' => t('A description to be shown with the template.')
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title'  => t('HTML Template'),
    '#rows'  =>  10,
    '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] : '',
    '#required' => true
  );
  if(module_exists('token')){
    $form['token_help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(),
      '#global_types' => true
    );
    $form['body']['#title'] = theme('token_help', 'node');
  }
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save')
  );
  
  if(isset($form_state['values']['tid'])){
    // if it's an existing template, offer a delete button
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete')
    );
    // and keep the template id going through a hidden field
    $form['tid'] = array(
      '#type' => 'hidden',
      '#value' => $form_state['values']['tid'],
    );
  }
  return $form;
}

/**
 * Wysiwyg template form submit - delete and save handlers
 */
function wysiwyg_template_template_form_submit($form, &$form_state) {
  //if we're deleting the template
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    //show the confirmation
    $form_state['rebuild'] = true;
    $form_state['confirm_delete'] = true;
    return;
  }
  // delete confirmation provided
  if (isset($form_state['values']['delete'])){
    if ($form_state['values']['delete'] === TRUE) {
      wysiwyg_template_delete_template($form_state['values']['tid']);                             
      drupal_set_message(t('The template has been deleted.'));    
      $form_state['redirect'] = 'admin/config/content/wysiwyg-templates';
      return;
    }
  }

  // save the template
  if(wysiwyg_template_save_template($form_state['values'])){
    drupal_set_message(t('The template has been saved.'));
  }
  else{
    drupal_set_message(t('There was an error saving the template to the database.'));
  }
  // redirect back to the overview page
  $form_state['redirect'] = 'admin/config/content/wysiwyg-templates';
}

/**
 * Menu callback -- ask for confirmation of rule deletion.
 */
function wysiwyg_template_delete_confirm($something, &$form_state) {
  
  $form['tid'] = array(
    '#type' => 'value',
    '#value' => isset($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : 0,
  );

  $template = wysiwyg_template_load_template(isset($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : 0);
  return confirm_form($form,
    t('Are you sure you want to delete the template %title?', array('%title' => $template['title'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/content/wysiwyg-templates',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function wysiwyg_template_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    wysiwyg_template_delete_template($form_state['values']['tid']);
  }

  $form_state['redirect'] = 'admin/config/content/wysiwyg-templates';
  return;
}
