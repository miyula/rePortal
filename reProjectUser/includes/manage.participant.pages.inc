<?php

/**
 * Menu callback for project/%/manage/persons/participant/remove/%
 */
function project_manage_persons_participant_remove_callback($path, $id){
    $project = prepare_project_manage_page($path);
    $remove = "DELETE FROM {research_projects_participants_cards} WHERE id=%d AND project=%d";
    db_query($remove, $id, $project->id);
    drupal_goto("project/$path/manage/persons");
    
}