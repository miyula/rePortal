<?php

/**
 * Menu callback for project/%/manage/settings/invitations
 */
function manage_settings_invitations_callback($path){
    $project = prepare_project_manage_page($path);
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
    $form['invite-researcher']['researcher-template'] = array(
        '#type' => 'select',
        '#title' => t('templates'),
        '#options' => get_message_templates_options_by_type($pid, 'email'),
        '#default_value' => isset($r_t->template)?$r_t->template:0,
        '#required' => TRUE,
    );
    
    $form['invite-participant'] = array(
        '#type' => 'fieldset',
        '#title' => t('Invite participant'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,  
    );
    $p_t = get_invite_template($pid, 'participant');
    $form['invite-participant']['participant-template'] = array(
        '#type' => 'select',
        '#title' => t('templates'),
        '#options' => get_message_templates_options_by_type($pid, 'email'),
        '#default_value' => isset($p_t->template)?$p_t->template:0,
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
    $researcher_template = $form_state['values']['researcher-template'];
    if(db_result(db_query($check, $pid, $researcher_template))<=0){
         form_set_error('researcher-template', t('Please select a template.'));
    }
    
    $participant_template = $form_state['values']['participant-template'];
    if(db_result(db_query($check, $pid, $participant_template))<=0){
         form_set_error('participant-template', t('Please select a template.'));
    }
}

function set_invitation_templates_form_submit($form, &$form_state){
    $pid = (int)$form_state['values']['project'];
    $researcher_template = $form_state['values']['researcher-template'];
    $participant_template = $form_state['values']['participant-template'];
    
    set_invite_template_for($pid, $researcher_template, 'researcher');
    set_invite_template_for($pid, $participant_template, 'participant');
    
    drupal_set_message(t('Invitation templates settings have been updated.'),'status');
}