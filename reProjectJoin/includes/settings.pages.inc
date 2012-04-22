<?php

/**
 * Menu callback for project/%/manage/settings/recruitment
 */
function project_manage_settings_join_callback($path){
    $project = prepare_project_manage_page($path);
    $project->subtitle = 'Recruitment Setting';
    
    //get recruitment info
    $get= 'SELECT id, key_name, value FROM {research_experitmental_recruitment} WHERE project=%d';
    $res = db_query($get, $project->id);
    while($r = db_fetch_object($res)){
        if($r->key_name=='status'){
            $project->status_key = $r->id;
            $project->status = $r->value;
        }elseif($r->key_name=='term_of_use'){
            $project->term_of_use_key = $r->id;
            $project->term_of_use = $r->value;
        }
    }
    
    $content = drupal_get_form('recruitment_settings_form', $project);
    
    $project->right_part = $content;
    return theme('project_manage_page',$project);
}

/** recruitment settings form **/
function recruitment_settings_form(&$form_state, $project){
    $status_option = array(
        '1' => t('open'),
        '0' => t('close'),
    );
    $form['project'] = array(
        '#type' => 'hidden',
        '#value'=> $project->id,
    );
    $form['status_key'] = array(
        '#type' => 'hidden',
        '#value'=> $project->status_key,
    );
    $form['status'] = array(
        '#type' => 'radios',
        '#title' => t('Recruitment'),
        '#options' => $status_option,
        '#default_value' => isset($project->status)?$project->status:0,
    );
    if($project->status==1){
        $form['recruitment_url'] = array(
            '#type' => 'textfield',
            '#title'=> 'Recruitment Page URL',
            '#size' => 80,
            '#value' => 'http://'.$_SERVER['HTTP_HOST'].url(generate_project_url($project->path).'/recruitment'),
            '#description' => t('You can copy this link to mail list, facebook or other website. '),
        );
    }
    $form['privacy_term_of_use_key'] = array(
        '#type' => 'hidden',
        '#value'=> $project->term_of_use_key,
    );
    $node_option = array(
        '0' => t('--Please choose a page--'),
    );
    //get content list
    $sql = 'SELECT node, n.title FROM {research_projects_contents} AS c, {node} AS n WHERE project=%d and node=n.nid ORDER BY c.changed';
    $res = db_query($sql, $project->id);
    while($r = db_fetch_object($res)){
        $node_option[$r->node] = $r->title;
    }
    $form['privacy_term_of_use'] = array(
        '#type' => 'select',
        '#title' => t('Privacy Policy & Terms of Use'),
        '#required' => true,
        '#options' => $node_option, 
        '#default_value' => isset($project->term_of_use)?$project->term_of_use:0,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    return $form;
}
function recruitment_settings_form_validate($form, &$form_state){
    $term_of_use = $form_state['values']['privacy_term_of_use'];
    if($term_of_use==0){
        form_set_error('privacy_term_of_use', 'Privacy Policy & Terms of Use is required.');
    }
}
function recruitment_settings_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    
    //update status
    $status = $form_state['values']['status'];
    $status_key = $form_state['values']['status_key'];
    if(!empty($status_key)){
        //update
        $update_status = 'UPDATE {research_experitmental_recruitment} SET value="%s", changed=NOW() WHERE id=%d AND project=%d AND key_name="status"';
        db_query($update_status, $status, $status_key, $project);
        
    }else{
        //insert new
        $insert_status = 'INSERT INTO {research_experitmental_recruitment} (project, key_name, value, created, changed) VALUES(%d, "%s", "%s", NOW(), NOW())';
        db_query($insert_status, $project, 'status', $status);
    }
    
    //update term of use
    $term = $form_state['values']['privacy_term_of_use'];
    $term_key = $form_state['values']['privacy_term_of_use_key'] ;
    if(!empty($term_key)){
        //update
        $update_term = 'UPDATE {research_experitmental_recruitment} SET value="%s", changed=NOW() WHERE id=%d AND project=%d AND key_name="term_of_use"';
        db_query($update_term, $term, $term_key, $project);
        
    }else{
        //insert new
        $insert_term = 'INSERT INTO {research_experitmental_recruitment} (project, key_name, value, created, changed) VALUES(%d, "%s", "%s", NOW(), NOW())';
        db_query($insert_term, $project, 'term_of_use', $term);
    }
    drupal_set_message(t('Settings have been saved.'), 'status');
}