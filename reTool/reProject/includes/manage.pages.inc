<?php

/**
 * Menu callback for project/%/manage
 */
function project_manage_callback($path){
    $project = prepare_project_manage_page($path);
        
    return theme('project_manage_page',$project);
}

/**
 * Menu callback for project/%/manage/settings
 */ 
function project_manage_settings_callback($path){
    $project = prepare_project_manage_page($path);
    $project->subtitle = 'Project settings';
    $project->right_part = drupal_get_form('edit_research_project_form', $project);    
    return theme('project_manage_page',$project);
}

/******************* Edit project form **********************/

/**
 * Form for edit research  project
 */
function edit_research_project_form(&$form_state, $project){
    $form['id'] = array(
        '#type'             => 'hidden',
        '#value'            => $project->id,
    );
    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Project name'),
        '#description'      => t('The name of the project.'),
        '#required'         => TRUE,
        '#default_value'    => $project->name,
    );
    $form['founder'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Founder'),
        '#description'      => t('The name of founders'),
        '#required'         => TRUE,
        '#default_value'    => $project->founder,
    );
    $form['path'] = array(
        '#type'             => 'textfield',
        '#title'            => t('The path to access project'),
        '#description'      => t('Unique path to access the project. By a-z, A-Z, 0-9 and _'),
        '#required'         => TRUE,
        '#default_value'    => $project->path,
    );
    $form['introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Introduction'),
        '#description'      => t('A short description of the project'),
        '#required'         => TRUE,
        '#default_value'    => $project->introduction,
    );
    $form['submit'] = array(
        '#type'             => 'submit',
        '#value'            => t('Save'),
    );
    return $form;
}

/**
 * Handle validation for project form
 */
function edit_research_project_form_validate($form, &$form_state){
    $id = $form_state['values']['id'];
    $path = $form_state['values']['path'];
    
    //validate if the path contain illegal characters
    if(!preg_match('/^([0-9]|[_]|[a-z]|[A_Z])*$/',$path)){
        form_set_error('path','The path contain illegal characters. Only letters, numbers and _ are allowed');
    }
    
    //validate if the path is not used by other project
    $sql = "SELECT id FROM {research_projects} WHERE path='%s'";
    $result = db_result(db_query($sql, $path, $id));
    if($result && $result<>$id){
        form_set_error('path','Unlucky! The path has alreay been used by another project.');
    }
}

/**
 * Handle submit for project form
 */
function edit_research_project_form_submit($form, &$form_state){
    
    $id = $form_state['values']['id'];
    $name = $form_state['values']['name'];
    $founder = $form_state['values']['founder'];
    $path = $form_state['values']['path'];
    $introduction = $form_state['values']['introduction'];
    
    $project = load_project($id, 'id');
    $project->name = $name;
    $project->founder = $founder;
    $project->path = $path;
    $project->introduction = $introduction;
    _research_projects_save($project);
    
    drupal_set_message('Settings have been saved.', 'status');
    drupal_goto('project/'.$path.'/manage/settings');
}
