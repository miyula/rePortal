<?php

/**
 * Menu callback for project/%/content/%
 */
function project_content_display_callback($path, $title){
    $project = load_project($path);
    $node = load_project_content_by_title($project->id, $title);
    $content.= '<p><a class="back_link" href="'.url("project/$path").'">Back to '.$project->name.'</a></p>';
    $content.= $node->body;
    return $content;
}