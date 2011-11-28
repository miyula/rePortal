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
    
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l($project->name, generate_project_url($path));
    drupal_set_breadcrumb($breadcrumb);
    
    $node = load_project_content_by_title($project->id, $title);
    $content.= add_translation_widget($node->body);
    return $content;
}