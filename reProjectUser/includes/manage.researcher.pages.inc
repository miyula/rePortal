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
    
    print $invite_link;
    exit;
    //prepare invite letter
    /*$project = load_project($pid);
    global $user;
    $subject = "{$project->name} invite you to join their researcher group";
    $body = '<p>Hi,</p>'
               . "<p>{$user->name} invites you to join research project {$project->name}. </p>"
               .'<p>Click the link to accept the term of use:</p>'
               ."<p>$invite_link</p>"
               .'<p>If clicking the link doesn\'t work, please copy and paste it into your browser.</p>'
               .'<p>Click the link to visit project home page: </p>'
                .'<p>'.get_project_url($pid).'</p>'
               .'<p>Please don\'t reply this mail.</p>'
               .'<p>Best Regards,</p>'
               .'<p>Yours,</p>'
               ."<p>{$project->name}</p>";

   
        //send invite mail
        message_send_http_mail($email,$subject,$body);
        
        //add_user_to_project($pid, $user->uid, PROJECT_RESEARCHER);
        //drupal_set_message("Added {$user->name} as researcher",'status');
   
        //drupal_set_message('Can not find the user in system. Please check the email address.','error');

        drupal_goto(get_project_url($pid).'/manage/persons');*/
    
}