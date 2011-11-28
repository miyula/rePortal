<?php

/**
 * Menu callback for project/%/manage/settings/invitations
 */
function manage_settings_invitations_callback($path){
    $project = prepare_project_manage_page($path);
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    $breadcrumb[] = l(t('Manage'), generate_project_url($path).'/manage');
    $breadcrumb[] = l(t('Settings'), generate_project_url($path).'/manage/settings');
    drupal_set_breadcrumb($breadcrumb);
    
    $project->path_category = 'settings';
    $project->path_category_subitem = 'persons'; 
    $project->subtitle = 'Set invitation templates';
    $project->right_part = drupal_get_form('set_invitation_templates_form', $project->id);    
    return theme('project_manage_page',$project);
}

/** Form for invitation templates **/
function set_invitation_templates_form($form_state, $pid){
    $form['project'] = array(
        '#type' => 'hidden',
        '#value' => $pid,
    );
    
    $form['invite-researcher'] = array(
        '#type' => 'fieldset',
        '#title' => t('Invite researcher'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,  
    );    
    $r_t = get_invite_template($pid, 'researcher');
    $form['invite-researcher']['researcher-template-id'] = array(
        '#type' => 'hidden',
        '#value' => $r_t->template,
    );
    $form['invite-researcher']['researcher-template-title'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => $r_t->title,
        '#required' => TRUE,
    );
    $form['invite-researcher']['researcher-template-content'] = array(
        '#type' => 'textarea',
        '#rows' => 15,
        '#title' => t('Content'),
        '#default_value' => $r_t->content,
        '#required' => TRUE,
    );
    
    $form['invite-participant'] = array(
        '#type' => 'fieldset',
        '#title' => t('Invite participant'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,  
    );
    $p_t = get_invite_template($pid, 'participant');
    $form['invite-participant']['participant-template-id'] = array(
        '#type' => 'hidden',
        '#value' => $r_t->template,
    );
    $form['invite-participant']['participant-template-title'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => $r_t->title,
        '#required' => TRUE,
    );
    $form['invite-participant']['participant-template-content'] = array(
        '#type' => 'textarea',
        '#rows' => 15,
        '#title' => t('Content'),
        '#default_value' => $r_t->content,
        '#required' => TRUE,
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    return $form;
}

function set_invitation_templates_form_validate($form, &$form_state){
    $pid = (int)$form_state['values']['project'];
    if($pid <= 0){
        form_set_error('project', t('Invalid project id.'));
    }
    
    $check = 'SELECT COUNT(*) FROM {project_message_templates} WHERE project=%d AND id=%d';
    $researcher_template = $form_state['values']['researcher-template-id'];
    if(db_result(db_query($check, $pid, $researcher_template))<=0){
         form_set_error('researcher-template-id', t('Invalid template id.'));
    }
    
    $participant_template = $form_state['values']['participant-template-id'];
    if(db_result(db_query($check, $pid, $participant_template))<=0){
         form_set_error('participant-template', t('Invalid template id.'));
    }
}

function set_invitation_templates_form_submit($form, &$form_state){
    $pid = (int)$form_state['values']['project'];
    $r_id = $form_state['values']['researcher-template-id'];
    $p_id = $form_state['values']['participant-template-id'];
    
    $r_template = get_message_template_by_id($r_id);
    $r_title = $form_state['values']['researcher-template-title'];
    $r_content = $form_state['values']['researcher-template-content'];
    $r_template->title = $r_title;
    $r_template->content = $r_content;
    save_message_template($r_template);
    
    $p_template = get_message_template_by_id($p_id);
    $p_title = $form_state['values']['participant-template-title'];
    $p_content = $form_state['values']['participant-template-content'];
    $p_template->title = $p_title;
    $p_template->content = $p_content;
    save_message_template($p_template);
    
    $log_content = "Changed invitation templates.";
    log_operation($pid, 'message', $log_content, get_project_url($pid).'/manage/settings/invitations', 'reProjectUser');
    
    drupal_set_message(t('Invitation templates settings have been updated.'),'status');
}