<?php

/**
 * Menu callback for project/%/manage/persons/tagsget/%
 */
function ajax_get_participant_tags_callback($path, $participant){
    drupal_set_header('Content-Type: text/plain; charset: utf-8');
   
    $project = load_project($path);
    $all_tags = get_project_all_participants_tags($project->id);
    $tags = get_participant_all_tags($participant, $project->id);
    $result = array(
        'tags' => $tags,
        'all'  => $all_tags,
    );
    print json_encode($result);

}

/**
 * Menu callback for project/%/manage/persons/tagsadd/%/%
 */
function ajax_add_participant_tag_callback($path, $participant, $tag){
    drupal_set_header('Content-Type: text/plain; charset: utf-8');
    print $tag;
}

/**
 * Menu callback for project/%/manage/persons/tagsremove/%/%
 */
function ajax_remove_participant_tag_callback($path, $participant, $tag){
    drupal_set_header('Content-Type: text/plain; charset: utf-8');
}

