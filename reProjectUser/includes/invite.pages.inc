<?php

/**
 * Menu callback for invite/%
 */
function invite_redirect_to_project_callback($code){
    if(!invitation_code_is_not_used($code)){
        $invite_message = '<font color="RED">Sorry, the invitation code does not exist or it has already been used!</font>';
    }else{
        $check = 'SELECT code, c.project AS project, c.email AS email, c.phone AS phone, c.person AS person, used, p.user AS user, p.role AS role, p.active AS active  '
                .'FROM {research_projects_onetime_invite_code} AS c,  {research_projects_persons_list} AS p '
                .'WHERE c.code="%s" AND c.person=p.id';
        $result = db_fetch_object(db_query($check, $code));
        
        $project = load_project($result->project,'id');
        $invite_message = 'You are to join project <strong>'.l($project->name, generate_project_url($project->path), array('attributes'=>array('target'=>'_blank')))."</strong> as a {$result->role}.";
    
        global $user;
        if($user->uid && $user->status){
            
            if($result->active==1){
                //change code to used
                $set_code_used = 'UPDATE  {research_projects_onetime_invite_code} SET used=1, changed=NOW() WHERE code="%s"';
                db_query($set_code_used, $code);
                drupal_goto(get_project_url($result->project));
                exit;
            }
            
            $person->exiting_user_join_form = drupal_get_form('exiting_user_join_project_form', $result);
            $person->username = $user->name;
        }else{
            $person->login_to_join_form = drupal_get_form('login_to_join_project_from', $code);
            $person->register_to_join_form = drupal_get_form('register_to_join_project_from', $code);
        }
        
    }
    
    drupal_add_css(drupal_get_path('module','reProjectUser').'/css/invite_page.css');
    return theme('invite_join_project_page', $person, $invite_message);
}

/** Existing user join project team form**/
function exiting_user_join_project_form($form_state, $person){
    $form['project'] = array(
        '#type' => 'hidden',
        '#value' => $person->project,
    );
    $form['person'] = array(
        '#type' => 'hidden',
        '#value' => $person->person,
    );
    $form['code'] = array(
        '#type' => 'hidden',
        '#value' => $person->code,
    );
    $form['accept'] = array(
        '#type' => 'submit',
        '#value' =>t(' Yes, I accept '),
        '#suffix' => '<font size=3>'.l('No, change an account', 'logout').'</font>',  
    );
    return $form;
}

function exiting_user_join_project_form_validate($form, &$form_state){
    
    global $user;
    if(!$user->uid||$user->status<>1){
        drupal_goto('user');    
    }

    $project = $form_state['values']['project'];
    $person = $form_state['values']['person'];
    $code = $form_state['values']['code'];
    
    $check = 'SELECT COUNT(*) FROM {research_projects_onetime_invite_code} AS c,  {research_projects_persons_list} AS p '
                  .'WHERE c.code="%s" AND c.used=0 AND c.project=%d AND c.person=%d AND c.project=p.project AND c.person=p.id';
    if(db_result(db_query($check, $code, $project, $person))==0){
        form_set_error('code', 'Invitation code and project do not match.');
    }
}

function exiting_user_join_project_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    $person = $form_state['values']['person'];
    $code = $form_state['values']['code'];
    global $user;
    
    //change code to used
    $set_code_used = 'UPDATE  {research_projects_onetime_invite_code} SET used=1, changed=NOW() WHERE code="%s"';
    db_query($set_code_used, $code);
    
    //active researcher
    $active = 'UPDATE {research_projects_persons_list} SET active=1, user=%d, changed=NOW() WHERE id=%d';
    db_query($active, $user->uid, $person);
    
    drupal_set_message(t('You have successfully joined the project.'), status);
    drupal_goto(get_project_url($project));
}


/** Login to join project form **/
function login_to_join_project_from($form_state, $code){
    $form = user_login($form_state);
    $form['name'] ['#size'] = 20;
    $form['pass'] ['#size'] = 20;
    $form['code'] = array(
        '#type' => 'hidden',
        '#value' => $code,
    );
    $form['submit']['#value'] = t('Login and accept');
    return $form;
}

function login_to_join_project_from_validate($form, &$form_state){
    
    $code = $form_state['values']['code'];
    if(!invitation_code_is_not_used($code)){
        form_set_error('code', 'invitation code does not exist or it has already been used!');
    }
    
    user_authenticate($form_state['values']);
}

function login_to_join_project_from_submit($form, &$form_state){
    $code = $form_state['values']['code'];
    
    global $user;
    
    if($user->uid){
        //change code to used
        $set_code_used = 'UPDATE  {research_projects_onetime_invite_code} SET used=1, changed=NOW() WHERE code="%s"';
        db_query($set_code_used, $code);
    
        //active researcher
        $p = db_fetch_object(db_query('SELECT project, person FROM {research_projects_onetime_invite_code} WHERE code="%s"', $code));
        $active = 'UPDATE {research_projects_persons_list} SET active=1, changed=NOW() WHERE id=%d';
        db_query($active, $p->person);
    
        drupal_set_message(t('You have successfully joined the project.'), status);
        drupal_goto(get_project_url($p->project));
    }else{
        drupal_goto('user');    
    }

}

/** register to join project form **/
function register_to_join_project_from($form_state, $code){
    $form = user_register();
    $form['name'] ['#size'] = 20;
    $form['name'] ['#description'] = '';
    $form['pass'] ['#size'] = 20;
    $form['mail'] ['#size'] = 20;
    $form['mail'] ['#description'] = '';
    $form['code'] = array(
        '#type' => 'hidden',
        '#value' => $code,
    );
    $form['submit']['#value'] = t('Register and accept');
    return $form;
}

/** register to join project form **/
function register_to_join_project_from_validate($form, &$form_state){
    user_module_invoke('validate', $form_state['values'], $form_state['values'], 'account');
}

/** register to join project form **/
function register_to_join_project_from_submit($form, &$form_state){
    user_register_submit($form, $form_state);
    
    $code = $form_state['values']['code'];
    global $user;
    if($user->uid){
        $set_code_used = 'UPDATE  {research_projects_onetime_invite_code} SET used=1, changed=NOW() WHERE code="%s"';
        db_query($set_code_used, $code);
    
        //active researcher
        $p = db_fetch_object(db_query('SELECT project, person FROM {research_projects_onetime_invite_code} WHERE code="%s"', $code));
        $active = 'UPDATE {research_projects_persons_list} SET active=1, user=%d, changed=NOW() WHERE id=%d';
        db_query($active, $user->uid, $p->person);
    
        drupal_set_message(t('You have successfully joined the project.'), status);
    }
    $form_state['redirect'] = 'project/mylife';
}