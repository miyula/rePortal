<?php

/**
 * Menu callback for project/%/manage
 */
function project_manage_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}


