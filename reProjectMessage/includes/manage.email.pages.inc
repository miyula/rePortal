<?php

/**
 * Menu callback for project/%/manage/message/email
 */
function project_manage_message_email_callback($path){
    $project = prepare_project_manage_page($path);
    
    //add css
    drupal_add_css(drupal_get_path('module','reProjectMessage').'/message.css');
    //add js
    drupal_add_js(drupal_get_path('module','reProjectMessage').'/person_list.js');
    
    //get person list
    $get = 'SELECT id, user, name, email, phone, tags, notes FROM {research_projects_participants_cards} WHERE project=%d ORDER BY created';
    $res = db_query($get, $project->id);
    $list = array();
    while($p=db_fetch_object($res)){
        $list[] = $p;
    }
    $content.= theme('project_person_list_div', $list, 'mail');
    $content.= drupal_get_form('message_email_sending_form');
    
    $project->subtitle = t('Send message');
    $project->right_part = $content;
    return theme('project_manage_page',$project);
}

/** email sending form **/

function message_email_sending_form(){
    $form['mail-list'] = array(
        '#type'         => 'textarea',
        '#title'        => t('To'),
        '#description'  => t('The email addresses should be splited by ";"'),
        '#required'     => TRUE,
        '#default_value'=> '',
    );
    $form['templates'] = array(
        '#type'         => 'select',
        '#title'        => t('Select a template for email'),
        '#options'      => array('--No template--'),
        '#required'     => FALSE,
    );
    $form['title'] = array(
        '#type'         => 'textfield',
        '#title'        => t('Subject'),
        '#description'  => t('The subject for mail.'),
        '#required'     => TRUE,
    );
    $form['body'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Body'),
        '#description'  => t('The body of the mail.'),
        '#required'     => TRUE,
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Send'),
    );
    return $form;
}
function message_email_sending_form_validate($form, &$form_state){
    $to = $form_state['values']['mail-list'];
    //generate to_mail array
    $to_mail = array();
    $token = strtok($to,  ';');
    while ($token !== false && valid_email_address($token))
    {
        $to_mail[] = trim($token);
        $token = strtok(";");
    }
    if(empty($to_mail)){
        form_set_error('mail-list', 'No valid email address');
    }else{
        $form_state['values']['to-mail'] = $to_mail;
    }
}
function message_email_sending_form_submit($form, &$form_state){
    $to = $form_state['values']['mail-list'];
    $subject = $form_state['values']['title'];
    $body = $form_state['values']['body'];
    $to_mail = $form_state['values']['to-mail']; 
    
    message_send_http_mail($to_mail,$subject,$body,TRUE);
}
