<?php

/**
 * Menu callback for user/%user/edit/personal_info
 */
function user_edit_personal_info_callback($user){
   $info = load_user_personal_info($user->uid);
    return drupal_get_form('edit_personal_information_form', $info);
}

/** Edit personal information form**/
function edit_personal_information_form($form_state, $info){
    $form['user'] = array(
        '#type' => 'hidden',
        '#value' => $info->user,
    );
    $form['firstname'] = array(
        '#type' => 'textfield',
        '#title' => t('Fristname'),
        '#maxlength' => 45,
        '#default_value' => isset($info->firstname)?$info->firstname:'',
        '#required' => TRUE,
    );
    $form['lastname'] = array(
        '#type' => 'textfield',
        '#title' => t('Lastname'),
        '#maxlength' => 45,
        '#default_value' => isset($info->lastname)?$info->lastname:'',
        '#required' => TRUE,
    );
    $form['birthday'] = array(
        '#type' => 'textfield',
        '#title' => t('Birthday'),
        '#required' => TRUE,
    );
    $form['weight'] = array(
        '#type' => 'textfield',
        '#title' => t('Weight (kg)'),
        '#default_value' => isset($info->weight)?$info->weight:'',
    );
    $form['height'] = array(
        '#type' => 'textfield',
        '#title' => t('Height (cm)'),
        '#default_value' => isset($info->height)?$info->height:'',
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    return $form;
}

function edit_personal_information_form_validate($form, &$form_state){
    
}
function edit_personal_information_form_submit($form, &$form_state){
    
}