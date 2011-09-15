<?php
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
                l($row->title,'project/'.$path.'/manage/contents/'.$row->id),
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
 * Menu callback for project/%/manage/contents/%
 */
function project_manage_content_edit_callback($path, $id){
    $project = prepare_project_manage_page($path);
        //get content node
        $sql = 'SELECT id, node, type, project FROM {research_projects_contents} WHERE id=%d AND project=%d LIMIT 1';
        $content = db_fetch_object(db_query($sql, $id, $project->id));
        if($content && $content->node>0){
            $node = node_load($content->node);
            $content->node = $node;
            $project->subtitle = "Edit {$node->title}";
            $project->right_part = drupal_get_form('project_content_edit_form', $content);
            return theme('project_manage_page',$project);
        }
    
}

/** Content edit form **/
function project_content_edit_form(&$form_state, $content){

    $form['id'] = array(
        '#type'             => 'hidden',
        '#value'            => $content->id,
    );
    $form['nid'] = array(
        '#type'             => 'hidden',
        '#value'            => $content->node->nid,
    );
    $form['type'] = array(
        '#type'             => 'hidden',
        '#value'            => $content->type,  
    );
    $form['project'] = array(
        '#type'             => 'hidden',
        '#value'            => $content->project,  
    );
    $form['title'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Title'),
        '#required'         => TRUE,
        '#default_value'    => isset($content->node->title)?$content->node->title:'',
    );
    $form['body'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Content'),
        '#default_value'    => isset($content->node->body)?$content->node->body:'',
        '#required'         => TRUE,
    );
    $form['submit'] = array(
        '#type'             => 'submit',
        '#value'            => t('Save'),
    );
    return $form;
}
function project_content_edit_form_submit($form, &$form_state){
    $id = $form_state['values']['id'];
    $nid = $form_state['values']['nid'];
    $type = $form_state['values']['type'];
    $project_id = $form_state['values']['project'];
    $title = $form_state['values']['title'];
    $body = $form_state['values']['body'];
    
    //load $node
    $node = node_load($nid);
    $node->title = $title;
    $node->body = $body;
    node_save($node);
    
    //update {research_projects_contents}
    $update = 'UPDATE {research_projects_contents} SET changed=NOW() WHERE id=%d';
    db_query($update, $id);

    drupal_goto(get_project_url($project_id).'/manage/contents');

}