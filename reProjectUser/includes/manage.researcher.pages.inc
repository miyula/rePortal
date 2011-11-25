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
    
    $t = get_invite_template($pid, 'researcher');
    if($t==FALSE || $t->template<=0){
        drupal_set_message(t('The template for invitate researcher is missing, please '.l('set one',get_project_url($pid).'/manage/settings/invitations').'.'),'error');    
    }
    
    $form['invite-template'] = array(
        '#type' => 'hidden',
        '#value' => (int)$t->template,
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
    
}
function project_invite_research_form_submit($form, &$form_state){
    $pid = $form_state['values']['pid'];
    $email = $form_state['values']['email'];

    //generate invite link
    $invite_link = generate_invite_link($pid, 'researcher', $email);
    
    //prepare invite letter
    $template = get_invite_template($pid, 'researcher');
    global $user;
    $project = load_project($pid, 'id');
    $pair_value = array(
        '@project_name' => $project->name,
        '!project_link' => $project->url,
        '!invite_link'=> $invite_link,
        '%name' => $user->name, 
    );
    $subject = t($template->title, $pair_value);
    $body = t($template->content, $pair_value);
   
    //send invite mail
    message_send_http_mail($email,$subject,$body);

    $form_state['redirect']  = get_project_url($pid).'/manage/persons';
    
}