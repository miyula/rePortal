<?php

/**
 * Menu callback for project/%/content/%
 */
function project_content_display_callback($path, $title){
    $project = load_project($path);
    
    if(!$project){
        drupal_not_found();
        exit;
    }
    
    $node = load_project_content_by_title($project->id, $title);
    
    if($node){
        $breadcrumb = drupal_get_breadcrumb();
        $breadcrumb[] = l($project->name, generate_project_url($path));
        drupal_set_breadcrumb($breadcrumb);
        
        if($node->content_type == 'homepage'){
            drupal_goto(generate_project_url($path));
            exit;
        }
        
        if(variable_get('open_g_translation_widget', 'disable')=='enable'){
            return add_translation_widget($node->body);
        }else{
            return $node->body;
        }
        
    }else{
        drupal_not_found();
        exit;
    }
}