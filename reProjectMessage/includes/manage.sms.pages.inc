<?php

/**
 * Menu callback for project/%/manage/message/sms
 */
function project_manage_message_sms_callback($path){
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
    $content.= theme('project_person_list_div', $list, 'sms');
    $content.= drupal_get_form('message_sms_sending_form');
    
    $project->subtitle = t('Send message');
    $project->right_part = $content;
    return theme('project_manage_page',$project);
}

/** SMS sending form **/

function message_sms_sending_form(){
    $form['phone-list'] = array(
        '#type'         => 'textarea',
        '#title'        => t('To'),
        '#description'  => t('The phone number should be splited by ";"'),
        '#required'     => TRUE,
        '#default_value'=> '',
    );
    $form['templates'] = array(
        '#type'         => 'select',
        '#title'        => t('Select a template as message.'),
        '#options'      => array('--No template--'),
        '#required'     => FALSE,
    );
    $form['body'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Body'),
        '#description'  => t('The body of the short message.'),
        '#required'     => TRUE,
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Send'),
    );
    return $form;
}
function message_sms_sending_form_validate($form, &$form_state){
    $to = $form_state['values']['phone-list'];
    //generate to_mail array
    $to_phone = array();
    $token = strtok($to,  ';');
    while ($token !== false && valid_phone_number($token))
    {
        $to_phone[] = trim($token);
        $token = strtok(";");
    }
    if(empty($to_phone)){
        form_set_error('phone-list', 'No valid phone number.');
    }else{
        $form_state['values']['to-phone'] = $to_phone;
    }
}
function message_sms_sending_form_submit($form, &$form_state){
    $to = $form_state['values']['phone-list'];
    $body = $form_state['values']['body'];
    $to_phone = $form_state['values']['to-phone']; 
    
    send_group_sms($to_phone,$body);
}