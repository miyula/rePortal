<?php

/**
 * Menu callback for project/%/manage/contents/termofuse
 */
function project_manage_content_termofuse_callback($path){
    //get project
    $project = load_project($path);
    if($project){
        $termofuse = load_term_of_use_node($project);
        $project->termofuse = $termofuse;
        $termofuse->project = $project->id;
        $content.= drupal_get_form('project_content_edit_form', $termofuse);
        return $content;
    }
    drupal_not_found();
}

/** Content edit form **/
function project_content_edit_form(&$form_state, $content){
    $form['nid'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($content->nid)?isset($content->nid):0,
    );
    $form['project'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($content->project)?isset($content->project):0,  
    );
    $form['title'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Title'),
        '#required'         => TRUE,
        '#default_value'    => isset($content->title)?$content->title:'',
    );
    $form['body'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Content'),
        '#default_value'    => isset($content->body)?$content->body:'',
        '#required'         => TRUE,
    );
    $form['submit'] = array(
        '#type'             => 'submit',
        '#value'            => t('Save'),
    );
    return $form;
}
function project_content_edit_form_submit($form, &$form_state){
    $nid = $form_state['values']['nid'];
    $project_id = $form_state['values']['project'];
    $title = $form_state['values']['title'];
    $body = $form_state['values']['body'];
    
    //load $node
    $node = node_load($nid);
    die($node);
    $node->title = $title;
    $node->body = $body;
    node_save($node);
    
    //drupal_goto(get_project_url($project_id).'/manage/contents');

}