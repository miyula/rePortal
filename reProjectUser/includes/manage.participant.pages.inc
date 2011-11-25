<?php

/**
 * Menu callback for project/%/manage/persons/participant/remove/%
 */
function project_manage_persons_participant_remove_callback($path, $id){
    $project = prepare_project_manage_page($path);
    $remove = "DELETE FROM {research_projects_participants_cards} WHERE id=%d AND project=%d";
    db_query($remove, $id, $project->id);
    drupal_goto("project/$path/manage/persons");
    
}

/**
 * Menu callback for project/%/manage/persons/participant/new
 */
function project_manage_persons_participants_new_callback($path){
    $project = prepare_project_manage_page($path); 
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    $breadcrumb[] = l(t('Manage'), generate_project_url($path).'/manage');
    $breadcrumb[] = l(t('Persons'), generate_project_url($path).'/manage/persons');
    drupal_set_breadcrumb($breadcrumb);
    
    $project->path_category = 'manage';
    $project->path_category_subitem = 'persons'; 
    
    $project->subtitle = 'New participants';
    $project->right_part = drupal_get_form('new_participant_info_card_form', $project);
    return theme('project_manage_page',$project);
}

/**
 * Menu callback for project/%/manage/persons/participant/edit/%
 */
function project_manage_persons_participant_edit_callback($path, $id){
    $project = prepare_project_manage_page($path);
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    $breadcrumb[] = l(t('Manage'), generate_project_url($path).'/manage');
    $breadcrumb[] = l(t('Persons'), generate_project_url($path).'/manage/persons');
    drupal_set_breadcrumb($breadcrumb);
    
    $project->path_category = 'manage';
    $project->path_category_subitem = 'persons'; 
    
    //get person info
    $get = "SELECT id, project, name, email, phone, notes FROM {research_projects_participants_cards} WHERE id=%d AND project=%d";
    $person = db_fetch_object(db_query($get, $id, $project->id));
    if($person){
        
        $project->subtitle = 'Edit person information';
        $project->right_part = drupal_get_form('edit_person_info_card_form', $person);
        return theme('project_manage_page',$project);

    }else{
        drupal_not_found();
    }
    
}
/** new person info card form **/
/** New participant info card form **/
function new_participant_info_card_form($form_state, $project){
    $form['project'] = array(
        '#type'         => 'hidden',
        '#value'        => $project->id,
    );
    $form['name'] = array(
        '#title'        => t('Participant name'),
        '#type'         => 'textfield',
        '#size'         => 20,
    );
    $form['email'] = array(
        '#title'        => t('Email'),
        '#type'         => 'textfield',
        '#size'         => 20,
    );
    $form['phone'] = array(
        '#title'        => t('Phone'),
        '#type'         => 'textfield',
        '#size'         => 20,
    );
    $form['notes'] = array(
        '#title'        => t('Notes'),
        '#type'         => 'textarea',
        '#cols'         => 15,
        '#rows'         => 3,
        '#description'  => t('Additional infomation of this participant.'),
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Add'),
    );
    return $form;
}
function new_participant_info_card_form_validate($form, &$form_state){
    $name = $form_state['values']['name'];
    if(empty($name)){
        form_set_error('name','The name is required.');
    }
    
    $email = $form_state['values']['email'];
    if(!empty($email) && !valid_email_address($email)){
        form_set_error('email','Invalid email address.');
    }
    
    $phone = $form_state['values']['phone'];
    if(!empty($phone) && !valid_phone_number($phone)){
        form_set_error('phone','Invalid phone number.');
    }
    
    if(empty($email) && empty($phone)){
        form_set_error('email', 'The email and phone should not all be empty!');
        form_set_error('phone', 'The email and phone should not all be empty!');
    }
    
}
function new_participant_info_card_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    $name = $form_state['values']['name'];
    $email = $form_state['values']['email'];
    $phone = $form_state['values']['phone'];
    $notes = filter_filter('process', 0, -1, $form_state['values']['notes']);
    $insert = 'INSERT INTO {research_projects_participants_cards}(project, name, email, phone, notes, created, changed) VALUES(%d, "%s","%s", "%s","%s", NOW(), NOW())';
    db_query($insert, $project, $name, $email, $phone, $notes);
    
}

/** Edit person info card form **/
function edit_person_info_card_form($form_state, $person){
    $form['id'] = array(
        '#type'         => 'hidden',
        '#value'        => $person->id, 
    );
    $form['project'] = array(
        '#type'         => 'hidden',
        '#value'        => $person->project,
    );
    $form['name'] = array(
        '#type'         => 'textfield',
        '#title'        => t('Name'),
        '#default_value'        => $person->name,
        '#required'     => TRUE,
    );
    $form['email'] = array(
        '#type'         => 'textfield',
        '#title'        => t('Email'),
        '#value'        => $person->email,
        '#disabled'   => TRUE,
    );
    $form['phone'] = array(
        '#type'         => 'textfield',
        '#title'        => t('Phone'),
        '#default_value'        => $person->phone,
        '#description'  => t('The phone number should start with +358.'),
    );
    $form['notes'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Notes'),
        '#default_value'        => $person->notes,
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Save'),
    );
    return $form;
}
function edit_person_info_card_form_validate($form, &$form_state){
    //validate email
    $email = $form_state['values']['email'];
    if(!empty($email) && !valid_email_address($email)){
        form_set_error('email', 'Invalid email address.');
    }
    
    //validate phone number
    $phone = $form_state['values']['phone'];
    if(!empty($phone) && !valid_phone_number($phone)){
        form_set_error('phone', 'Invalid phone number.');
    }
}
function edit_person_info_card_form_submit($form, &$form_state){
    $id = $form_state['values']['id'];
    $project = $form_state['values']['project'];
    $name = $form_state['values']['name'];
    $email = $form_state['values']['email'];
    $phone = $form_state['values']['phone'];
    $notes = filter_filter('process', 0, -1, $form_state['values']['notes']);
    
    //update
    $update = 'UPDATE {research_projects_participants_cards} SET name="%s", email="%s", phone="%s", notes="%s" WHERE id=%d and project=%d';
    db_query($update, $name, $email, $phone, $notes, $id, $project);
    $pObj = load_project($project, 'id');
    drupal_goto('project/'.$pObj->path.'/manage/persons');
}