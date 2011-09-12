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

/**
 * Menu callback for project/%/manage/contents
 */
function project_manage_contents_list_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        
        //get right part info
        $content = '';
        $rows = array();
        $header = array(
            array('data'=>'Title'),
            array('data'=>'Type'),
            array('data'=>'Last update'),
        );
        
        $sql = "SELECT p.id id, n.title title, p.type type, n.changed changed FROM {research_projects_contents} AS p, {node} AS n WHERE p.project=%d AND p.node=n.nid ORDER BY n.changed DESC";
        $res = db_query($sql, $project->id);
        while($row=db_fetch_object($res)){
            $rows[] = array(
                l($row->title,'project/'.$path.'/manage/content/'.$row->id),
                $row->type,
                format_interval(time()-$row->changed).' ago',
            );
        }
        $content.= theme_table($header, $rows);
        $content.= theme('pager');
        $project->subtitle = 'Contents management';
        $project->right_part = $content;    
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}

/**
 * Menu callback for project/%/manage/content/%
 */
function project_manage_content_edit_callback($path, $id){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        //get content node
        $sql = 'SELECT node FROM {research_projects_contents} WHERE id=%d AND project=%d LIMIT 1';
        $nid = db_result(db_query($sql, $id, $project->id));
        if($nid>0){
            $node = node_load($nid);
            $node->project = $project->id;
            $project->subtitle = "Edit {$node->title}";
            $project->right_part = drupal_get_form('project_content_edit_form', $node);
            return theme('project_manage_page',$project);
        }
    }
    
    drupal_not_found();
}