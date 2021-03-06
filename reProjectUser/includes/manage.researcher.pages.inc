<?php

/**
 * Menu callback for project/%/manage/persons/researcher/add
 */
function project_manage_persons_researcher_add_callback($path){
    $project = prepare_project_manage_page($path);
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    $breadcrumb[] = l(t('Manage'), generate_project_url($path).'/manage');
    $breadcrumb[] = l(t('Persons'), generate_project_url($path).'/manage/persons');
    drupal_set_breadcrumb($breadcrumb);
    
    $project->path_category = 'manage';
    $project->path_category_subitem = 'persons'; 
    
    $project->subtitle = 'Invite new staff';
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
        '#required' => TRUE,
        '#description' => t('The email of the person you want to invite. '.l('Edit the invitation.', get_project_url($pid).'/manage/settings/templates')),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );
    
    return $form;
}
function project_invite_research_form_validate($form, &$form_state){
    $pid = $form_state['values']['pid'];
    $email = $form_state['values']['email'];
    if(!valid_email_address($email)){
        form_set_error('email', 'The email address you input is invalid.');
    }
    
    //check if the email user already in project
    $check = 'SELECT name, active FROM {users} AS u, {research_projects_persons_list} AS p WHERE u.mail="%s" AND p.user=u.uid AND p.project=%d AND p.role IN ("researcher", "creator") AND active=1 ';
    $r = db_fetch_object(db_query($check, $email, $pid));
    
    if($r){
        //if the user already in researcher list
        form_set_error('email', 'The staff is already in your project.');
    }
    
}
function project_invite_research_form_submit($form, &$form_state){
    $pid = $form_state['values']['pid'];
    $email = $form_state['values']['email'];
    
    send_invitation_email($pid, 'researcher', $email);
    
    //log the operation
    global $user;
    $content = "Send mail to invite $email to join project as a research.";
    log_operation($pid, 'persons', $content, get_project_url($pid).'/manage/persons/researcher/add', 'reProjectUser');
    
    $form_state['redirect']  = get_project_url($pid).'/manage/persons';
    
    
}

/**
 * Menu callback for project/%/manage/persons/researcher/remove/%
 */
function project_manage_persons_researcher_remove_callback($path, $id){
    $project = prepare_project_manage_page($path);
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    $breadcrumb[] = l(t('Manage'), generate_project_url($path).'/manage');
    $breadcrumb[] = l(t('Persons'), generate_project_url($path).'/manage/persons');
    drupal_set_breadcrumb($breadcrumb);
    
    $project->path_category = 'manage';
    $project->path_category_subitem = 'persons'; 
    
    $project->subtitle = 'Remove staff';
    
    //find the researcher to be removed
    $find = 'SELECT id, name, uid FROM {research_projects_persons_list}, {users} WHERE id=%d AND project=%d AND user=uid';
    $staff = db_fetch_object(db_query($find, $id, $project->id));
    if($staff){
        $staff->after_remove_path = "project/$path/manage/persons";
        $project->right_part = drupal_get_form('project_remove_research_form', $staff);
        return theme('project_manage_page',$project);
    }else{
        drupal_not_found();
    }
}

/** Form for remove researcher **/
function project_remove_research_form(&$form_state, $staff){
    $form['id'] = array(
        '#type' => 'hidden',
        '#value'=> $staff->id,
    );
    $form['after_remove_path'] = array(
        '#type' => 'hidden',
        '#value'=> $staff->after_remove_path,
    );
    return confirm_form($form, 
        t('Are you sure you want to remove %user_name from the staff list?', 
                array('%user_name' => $staff->name, '%section' => $page->section_title)
            ), 
        $staff->after_remove_path, 
        t('This action cannot be undone.'), 
        t('Remove'), 
        t('Cancel'));
}
function project_remove_research_form_submit($form, &$form_state){
    $id = $form_state['values']['id'];
    $delete = "DELETE FROM {research_projects_persons_list} WHERE id=%d";
    db_query($delete, $id);
    $form_state['redirect'] = $form_state['values']['after_remove_path'];
}