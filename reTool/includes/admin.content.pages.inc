<?php

/**
 * Menu callback for admin/content/retools
 */
function admin_content_retools_callback(){
    
    $header = array(
        array('data'=>t('Research tool')),
        array('data'=>t('Short description')),
        array('data'=>t('Operations'), 'colspan'=>2),
    );
    
    $rows = array();
    
    $sql = "SELECT id, name, logo, short_describe FROM {research_tools}";
    $res = pager_query($sql,30);
    
    while($row = db_fetch_object($res)){
        $rows[] = array(
            array('data'=>l($row->name,'research/tools/view/'.$row->id)),
            array('data'=>$row->short_describe),
            array('data'=>l('edit',"research/tools/view/{$row->id}/edit")),
            array('data'=>l('delete',"admin/content/retools/{$row->id}/delete")),
        );
    }
    $content.= theme_table($header, $rows);
    $content.= theme('pager');
    
    return $content;
    
}

/**
 * Menu callback for admin/content/retools/%/delete
 */
function admin_content_retools_delete_callback($tid){
    $tid = (int)$tid;
    //get research tool info
    $sql = "SELECT id, name, logo, short_describe FROM {research_tools} WHERE id=%d";
    $tool = db_fetch_object(db_query($sql, $tid));
    if($tool){
        return drupal_get_form('delete_research_tool_confirm_form',$tool);
    }else{
        return 'The item you want to delete is not existed.';
    }
    
    
}

/** form for delete research tool **/
function delete_research_tool_confirm_form(&$form_state, $tool){
    $form['tid'] = array(
        '#type' => 'value',
        '#value' => $tool->id,
    );
    
    $question = t('Are you sure you want to delete the research tool %name?', array('%name' => $tool->name));
    $description = t('Deleting a research tool will also remove this tool from the projects that are using it.');
    return confirm_form($form, $question, 'admin/content/retools',$description,  t('Delete'), t('Cancel'));
}
function delete_research_tool_confirm_form_submit($form, &$form_state){
    $tid = $form_state['values']['tid'];
    
    //remove links in each project
    $delete_tool_link_in_p = 'DELETE FROM {research_project_tool_links} WHERE tool=%d';
    //remove tool in each project
    $delete_tool_in_p = 'DELETE FROM {research_project_tools_list} WHERE tool=%d';
    //remove tool links
    $delete_tool_links = 'DELETE FROM {research_tool_links} WHERE tool=%d';
    //remove tool
    $delete_tool = 'DELETE FROM {research_tools} WHERE id=%d';
    db_query($delete_tool_link_in_p, $tid);
    db_query($delete_tool_in_p, $tid);
    db_query($delete_tool_links, $tid);
    db_query($delete_tool, $tid);
    $form_state['redirect'] = 'admin/content/retools';
}