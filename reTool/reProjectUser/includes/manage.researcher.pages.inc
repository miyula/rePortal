<?php

/**
 * Menu callback for project/%/manage/persons/researcher/add
 */
function project_manage_persons_researcher_add_callback($path){
    $project = prepare_project_manage_page($path);
    $project->subtitle = 'Add new researcher';
    $project->right_part = drupal_get_form('project_invite_research_form', $project->id);
    return theme('project_manage_page',$project);
}

/** Invite researcher form **/
function project_invite_research_form(&$form_state, $pid){
    $form['pid'] = array(
        '#type' => 'hidden',
        '#value' => $pid,
    );
    $form['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email address'),
        '#description' => t('The email account of the researcher in this protal.'),
        '#required' => TRUE,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );
    
    return $form;
}
function project_invite_research_form_validate($form, &$form_state){
    $email = $form_state['values']['email'];
    $find = "SELECT uid FROM {users} WHERE mail='%s'";
    $uid = db_result(db_query($find, $email));
    if(!$uid){
        form_set_error('Can not find the user in system. Please check the email address.','email');
    }
}
function project_invite_research_form_submit($form, &$form_state){
    $pid = $form_state['values']['pid'];
    $email = $form_state['values']['email'];
    
    //find user
    $find = "SELECT uid, name FROM {users} WHERE mail='%s'";
    $user = db_fetch_object(db_query($find, $email));
    
    if($user){
        add_user_to_project($pid, $user->uid, PROJECT_RESEARCHER);
        drupal_set_message("Added {$user->name} as researcher",'status');
    }else{
        drupal_set_message('Can not find the user in system. Please check the email address.','error');
    }
    drupal_goto(get_project_url($pid).'/manage/persons');
    
}