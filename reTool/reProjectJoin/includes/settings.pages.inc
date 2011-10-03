<?php

/**
 * Menu callback for project/%/manage/settings/join
 */
function project_manage_settings_join_callback($path){
    $project = prepare_project_manage_page($path);
    $project->subtitle = 'Participation options settings';
    $project->right_part = $content;    
    return theme('project_manage_page',$project);
}