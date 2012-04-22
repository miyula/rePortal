<?php

/**
 * Implementation of hook_menu().
 */
function project_privacy_term_of_use_callback($path){
    $project = load_project($path);
    
    if(!$project){
        drupal_not_found();
        exit;
    }
    
    //get term of use node
    $sql = 'SELECT value FROM {research_experitmental_recruitment} WHERE project=%d AND key_name="term_of_use"';
    $nid = db_result(db_query($sql, $project->id));
    if($nid){
        $node = node_load($nid);
        if($node){
            $breadcrumb = drupal_get_breadcrumb();
            $breadcrumb[] = l($project->name, generate_project_url($path));
            drupal_set_breadcrumb($breadcrumb);
            return $node->body;
        }
    }
    drupal_goto(generate_project_url($path));
}