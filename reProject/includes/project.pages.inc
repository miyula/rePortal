<?php
/**
 * Menu callback for projects/new
 */
function projects_new_callback(){
    //get project form
    $content = drupal_get_form('create_new_project_form');
    return $content;
}
/**
 * Menu callback for projects/list
 */
function projects_list_callback(){
    
    //get project list
    $sql = "SELECT id, creator, name, url, path, description, created, changed FROM {research_projects} ORDER BY";
    
}

/**
 * Form for create new project
 */
function create_new_project_form(){

    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Project name'),
        '#description'      => t('The name of the project.'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->name)?$project->name:'',
    );
    $form['path'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Define a path'),
        '#description'      => t('Unique path to access the project. By a-z, A-Z, 0-9 and _'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->path)?$project->path:'',
    );
    $form['Introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Write a short introduction for your project'),
        '#description'      => t('A short description of the project'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->introduction)?$project->introduction:'',
    );
    $form['next_step'] = array(
        '#type'             => 'submit',
        '#value'            => t('Next'),
    );
    return $form;
}

/**
 * Handle validation for project form
 */
function create_new_project_form_validate($form, &$form_state){
    //$name = $form_state['values']['name'];
    $path = $form_state['values']['path'];
    //$introduction = $form_state['values']['introduction'];
    
    //validate if the path contain illegal characters
    if(!preg_match('/^([0-9]|[_]|[a-z]|[A_Z])*$/',$path)){
        form_set_error('path','The path contain illegal characters. Only letters, numbers and _ are allowed');
    }
    
    //validate if the path is unique
    $sql = "SELECT COUNT(*) FROM {research_projects} WHERE path='%s'";
    if(db_result(db_query($sql, $path))>0){
        form_set_error('path','Unlucky! The path has alreay been used by another project.');
    }
}

/**
 * Handle submit for project form
 */
function create_new_project_form_submit($form, &$form_state){
    
    //get current user
    global $user;
    if($user->uid==0){
        //not authentication user, redirect to login page
        drupal_goto('user');
    }
    
    $name = $form_state['values']['name'];
    $path = $form_state['values']['path'];
    $introduction = $form_state['values']['introduction'];
    
    $project = new stdClass;
    $project->creator       = $user->uid;
    $project->name          = $form_state['values']['name'];
    $project->path          = $form_state['values']['path'];
    $project->introduction  = $form_state['values']['introduction'];
    
    if(create_new_project($project)){
        //succeed to create new project
        drupal_goto("project/$path/");
    }else{
        drupal_set_message($message = 'Failed to create new project', $type = 'error');
        drupal_goto('user');
    }
}


/**
 * Form for edit research  project
 */
function edit_research_project_form($project=null){
    $form['id'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($project->id)?$project->id:0,
    );
    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Project name'),
        '#description'      => t('The name of the project.'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->name)?$project->name:'',
    );
    $form['path'] = array(
        '#type'             => 'textfield',
        '#title'            => t('The path to access project'),
        '#description'      => t('Unique path to access the project. By a-z, A-Z, 0-9 and _'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->path)?$project->path:'',
    );
    $form['Introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Introduction'),
        '#description'      => t('A short description of the project'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->introduction)?$project->introduction:'',
    );
    return $form;
}

/**
 * Handle validation for project form
 */
function edit_research_project_form_validate($form, &$form_state){
    //$name = $form_state['values']['name'];
    $id = $form_state['values']['id'];
    $path = $form_state['values']['path'];
    //$introduction = $form_state['values']['introduction'];
    
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
    
}

/**
 * Handle operation to create a new project().
 */
function create_new_project($project){
    //step.1 save project info
    if(_research_projects_save($project)){
        //log operation
        log_operation($project->id, 'information', "Create new project", url('/project/create'), REPROJECT_MODULE_NAME);
        return TRUE;
        
    }else{
        drupal_set_message($message = 'Failed to create new project', $type = 'error');
        return FALSE;
    }
    
}

