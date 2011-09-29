<?php

/**
 * Menu callback for project/%/manage/tools
 */
function project_manage_tools_list_callback($path){
    $project = prepare_project_manage_page($path);
        
    //get right part info
    $content = '<a href="'.url('project/'.$path.'/manage/tools/selete').'" class="add_link">Add new tool</a>';
    $rows = array();
    $header = array(
        array('data'=>'Tool'),
        array('data'=>'Name'),
        array('data'=>'Operation'),
    );
        
    $sql = "SELECT t.id id, t.name, t.logo FROM {research_project_tools_list} AS p, {research_tools} AS t WHERE p.project=%d AND p.tool=t.id";
    $res = db_query($sql, $project->id);
    while($row=db_fetch_object($res)){
        $rows[] = array(
            "<image src='{$row->logo}' width='100px' />",
            "<a href='".url('research/tools/view/'.$row->id)."' target='_blank'>{$row->name}</a>",
            "<p><a href='".url("project/$path/manage/tools/remove/{$row->id}")."' class='delete_link'>Remove</a></p>"
            ."<p><a href='".url("project/$path/manage/tools/editlink/{$row->id}")."' class='links_link'>Related links</a></p>",
        );
    }
    $content.= theme_table($header, $rows);
    $content.= theme('pager');
    $project->subtitle = 'Tools management';
    $project->right_part = $content;    
    return theme('project_manage_page',$project);
}

/**
 * Menu callback for project/%/manage/tools/selete
 */
function project_manage_tools_selete_list_callback($path){
    $project = prepare_project_manage_page($path);
        
    //get right part info
    $content = "<p><a class='back_link' href='project/$path/manage/tools'>Back</a></p>";
    $rows = array();
    $header = array(
        array('data'=>'Tool'),
        array('data'=>'Name'),
        array('data'=>'Operation'),
    );
        
    $sql = "SELECT t.id id, t.name, t.logo FROM {research_tools} AS t ORDER BY t.changed DESC";
    $res = db_query($sql);
    while($row=db_fetch_object($res)){
        $rows[] = array(
            "<image src='{$row->logo}' width='100px' />",
            "<a href='".url('research/tools/view/'.$row->id)."' target='_blank'>{$row->name}</a>",
            check_project_using_tool($project, $row->id)?'You are using this tool.':"<a href='".url("project/$path/manage/tools/selete/{$row->id}")."' class='add_link'>Add to project</a>",
        );
    }
    $content.= theme_table($header, $rows);
    $content.= theme('pager');
    $project->subtitle = 'Tools management';
    $project->right_part = $content;    
    return theme('project_manage_page',$project);

}

/**
 * Menu callback for project/%/manage/tools/selete/%
 */
function project_manage_tools_selete_new_callback($path, $id){
    $project = prepare_project_manage_page($path);
    project_bind_tool($project, $id);
    drupal_goto('project/'.$path.'/manage/tools');
}

/**
 * Menu callback for project/%/manage/tools/remove/%
 */
function project_manage_tools_remove_callback($path, $tool_id){
    $project = prepare_project_manage_page($path);
    project_remove_tool($project, $tool_id);
    drupal_goto('project/'.$path.'/manage/tools');
}

/**
 * Menu callback for project/%/manage/tools/editlink/%
 */
function project_manage_tools_editlink_callback($path, $tool_id){
    $project = prepare_project_manage_page($path);
    
    drupal_add_css(drupal_get_path('module','reTool').'/reTool.css');
    
    $project->subtitle = 'Edit tool releated links';
    $content.= '<p class="message">The links will be showed on tool block in project home page.</p>';
    
    //get tool info
    $tool = new stdClass;
    $tool->project = $project->id;
    $tool->id = $tool_id;
    
    $rows = array();
    $header = array(
        array('data'=>'Title', 'width'=>'100px'),
        array('data'=>'Link', 'width'=>'320px'),
        array('data'=>'Permission','width'=>'150px'),
        array('data'=>'Weight', 'width'=>'70px'),
        array('data'=>'Operation','width'=>'100px'),
    );
    $rows[] = array(
        array(
            'data' => '<div id="new_tool_link_form_div">'.drupal_get_form('add_new_tool_related_link_form', $tool).'</div>',
            'colspan' => 5,
        ),
    );
    
    //get links list
    $get = 'SELECT id, project, tool, title, url, permission, weight FROM {research_project_tool_links} WHERE project=%d AND tool=%d ORDER BY weight';
    $res = db_query($get, $project->id, $tool_id);
    while($row = db_fetch_object($res)){
        $rows[] = array(
            '<div class="project-tool-title-div">'.$row->title.'</div>',
            '<div class="project-tool-link-div">'.l($row->url,$row->url, array('att'=>'')).'</div>',
            $row->permission,
            $row->weight,
            '<a href="'.url("project/$path/manage/tools/removelink/$tool_id/{$row->id}").'" class="delete_link">Remove</a>'
        );
    }
    
    
    $content.= theme_table($header, $rows);
    
    $project->right_part = $content;
    return theme('project_manage_page',$project);
}

/**
 * Menu callback for project/%/manage/tools/removelink/%/%
 */
function project_manage_tools_link_remove_callback($path, $tool_id, $link_id){
    $project = prepare_project_manage_page($path);
    
    //remove
    $remove = 'DELETE FROM {research_project_tool_links} WHERE id=%d AND project=%d AND tool=%d';
    db_query($remove, $link_id, $project->id, $tool_id);
    drupal_goto("project/$path/manage/tools/editlink/$tool_id");
}
/** add new tool related link form **/
function add_new_tool_related_link_form($form_state, $tool){
    $form['project'] = array(
        '#type'     => 'hidden',
        '#value'    => $tool->project,
    );
    $form['tool'] = array(
        '#type'     => 'hidden',
        '#value'    => $tool->id,
    );
    $form['title'] = array(
        '#type'     => 'textfield',
        '#size'     => 15,
        '#required' => TRUE,
    );
    $form['url'] = array(
        '#type'     => 'textfield',
        '#size'     => 20,
        '#required' => TRUE,
    );
    $options = array(
        'PUBLIC'   => 'Public',
        'CONFIDENTIAL' => 'Only for researchers',
    );
    $form['permission'] = array(
        '#type'     => 'select',
        '#options'  => $options,
    );
    $form['weight'] = array(
        '#type'     => 'weight',
        '#required' => TRUE,
    );
    $form['add'] = array(
        '#type'     => 'submit',
        '#value'    => t('Add'),
    );
    return $form;
}
function add_new_tool_related_link_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    $tool = $form_state['values']['tool'];
    $title = $form_state['values']['title'];
    $url = $form_state['values']['url'];
    $weight = $form_state['values']['weight'];
    $permission = $form_state['values']['permission'];
    
    $add = 'INSERT INTO {research_project_tool_links}(project, tool, title, url, weight, permission, created, changed) VALUES(%d, %d, "%s", "%s", %d, "%s", NOW(), NOW())';
    db_query($add, $project, $tool, $title, $url, $weight, $permission);
}
/** manage_tool_related_links form **/
function manage_tool_related_links_form($form_state, $links){
    
}
