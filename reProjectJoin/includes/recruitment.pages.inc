<?php


/**
 * Menu callback for project/%/manage/recruitment
 */
function project_recruitment_callback($path){
    $project = load_project($path);
    
    if(!$project){
        drupal_not_found();
        exit;
    }
    
    //check the project status 
    if(check_recruitment_status($project->id)){
        $content.=t('<p style="">This is a recruitment for <a href="@link" target="_blank">%project</a>. If you are interested in, please leave your email address. We will send you more about %project.</p>', array('@link'=>url(generate_project_url($project->path)),'%project'=>$project->name)); 
        $content.= '<div style="">'.drupal_get_form('recruitment_email_form', $project).'</div>';
        return $content;
    }else{
        return t('Sorry, the recruitment for <a href="@link">%project</a> is closed now.', array('@link'=>url(generate_project_url($project->path)),'%project'=>$project->name));
    }
    
}

function recruitment_email_form(&$form_state, $project){
    $form['project'] = array(
        '#type' => 'hidden',
        '#value'=> $project->id,
    );
    $form['email'] = array(
        '#type' => 'textfield',
        '#title'=> t('Your email adress'),
        '#required'=>TRUE,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value'=> t('Send to my email'),
    );
    return $form;
}
function recruitment_email_form_validate($form, &$form_state){
    //check project status
    $project = $form_state['values']['project'];
    if(!check_recruitment_status($project)){
        form_set_error('project',t('Sorry, this recruitment is closed.'));
    }
    //check email
    $email = $form_state['values']['email'];
    if(!valid_email_address($email)){
        form_set_error('email', 'The email address you input is invalid.');
    }
    
}
function recruitment_email_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    $email = $form_state['values']['email'];
    
    $project = $form_state['values']['project'];
    $name = 'Anonymous';
    $notes = 'Joined from open recruitment page.';
  
    //check if the email already in project
    $get = 'SELECT id FROM {research_projects_participants_cards} WHERE project=%d AND email="%s"';
    $info_card = db_result(db_query($get, $project, $email));
    if(!$info_card){
       $insert = 'INSERT INTO {research_projects_participants_cards} '
             .'(project, name, email, notes, created, changed) '
             .'VALUES(%d, "%s", "%s", "%s", NOW(), NOW())';
        db_query($insert, $project, $name, $email, $notes);
        $info_card = mysql_insert_id(); 
    }
    send_invitation_email($project, 'participant', $email, $info_card);
    
    drupal_set_message(t('Thank you for your interest. The mail has been sent to %email.', array('%email'=>$email)));
}
