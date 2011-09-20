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
 * Menu callback for project/%/manage/persons/participant/edit/%
 */
function project_manage_persons_participant_edit_callback($path, $id){
    $project = prepare_project_manage_page($path);
    //get person info
    $get = "SELECT id, project, name, email, phone, notes FROM {research_projects_participants_cards} WHERE id=%d AND project=%d";
    if($person=db_fetch_object(db_query($get, $id, $project->id))){
        return drupal_get_form('edit_person_info_card_form', $person);
    }else{
        drupal_not_found();
    }
    
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
        '#default_value'        => $person->email,
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
    $notes = $form_state['values']['notes'];
    
    //update
    $update = 'UPDATE {research_projects_participants_cards} SET name="%s", email="%s", phone="%s", notes="%s" WHERE id=%d and project=%d';
    db_query($update, $name, $email, $phone, $notes, $id, $project);
    $pObj = load_project($project, 'id');
    drupal_goto('project/'.$pObj->path.'/manage/persons');
}