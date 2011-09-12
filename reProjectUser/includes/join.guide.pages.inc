<?php

/**
 * Menu callback for project/%/termofuse/%
 */
function project_termofuse_callback($path, $inviteCode){
    //get project
    $project = load_project($path);
    if($project){
        $termofuse = load_term_of_use_node($project);
        $project->termofuse = $termofuse;
        return theme('project_termofuse_page', $project);
    }
    drupal_not_found();
}