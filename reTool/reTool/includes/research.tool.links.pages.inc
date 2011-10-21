<?php

/**
 * Menu callback for research/tools/view/%/links/add
 */
function research_tools_links_add_callback($tool_id){
    $link = new stdClass;
    $link->tool = $tool_id;
    $content.= drupal_get_form('research_tool_link_form', $link);
    return $content;
}

/**
 * Menu callback for research/tools/view/%/links/edit/%
 */
function research_tools_links_edit_callback($tool_id, $link_id){
    //get link info
    $get = "SELECT id, tool, title, url FROM {research_tool_links} WHERE id=%d AND tool=%d";
    if($link = db_fetch_object(db_query($get, $link_id, $tool_id))){
        return drupal_get_form('research_tool_link_form', $link);
    }else{
        return 'Can not find link info. '.l('Go back to tool page', 'research/tools/view/'.$tool_id);
    }
}

/**
 * Menu callback for research/tools/view/%/links/delete/%
 */
function research_tools_links_delete_callback($tool_id, $link_id){
    //get link info
    $get = "SELECT l.id id, tool, title, url, t.name name FROM {research_tool_links} AS l, {research_tools} AS t "
          ."WHERE l.id=%d AND l.tool=%d AND l.tool=t.id";
    if($link = db_fetch_object(db_query($get, $link_id, $tool_id))){
        $content = 'You are deleting '.link($link->url).' from reserach tool <strong>'.$link->name.'</strong>';
        $content.= drupal_get_form('research_tool_link_delete_form');
    }else{
        return 'Can not find link info. '.l('Go back to tool page', 'research/tools/view/'.$tool_id);
    }
}

/** tool link form **/
function research_tool_link_form(&$form_state, $link){
    
    $form['id'] = array(
        '#type'     => 'hidden',
        '#value'    => isset($link->id)?$link->id:0,
    );
    $form['tool'] = array(
        '#type'     => 'hidden',
        '#value'    => isset($link->tool)?$link->tool:0,
    );
    $form['title'] = array(
        '#type'     => 'textfield',
        '#title'    => t('Link title'),
        '#required' => TRUE,
        '#default_value'    => isset($link->title)?$link->title:'',
    );
    $form['url'] = array(
        '#type'     => 'textfield',
        '#title'    => t('URL'),
        '#required' => TRUE,
        '#default_value'    => isset($link->url)?$link->url:'',
    );
    $form['submit'] = array(
        '#type'     => 'submit',
        '#value'    => t('Save'),
    );
    if(isset($link->id)&&$link->id>0){
        $form['submit']['#suffix']=l('Delete', "research/tools/view/{$link->tool}/links/delete/{$link->id}");
    }
    return $form;
}
function research_tool_link_form_submit($form, &$form_state){
    
    $id = (int)$form_state['values']['id'];
    $tool = (int)$form_state['values']['tool'];
    $title = $form_state['values']['title'];
    $url = $form_state['values']['url'];
    
    if($id>0){
        //update record
        $update = "UPDATE {research_tool_links} SET title='%s', url='%s', changed=NOW() WHERE id=%d AND tool=%d";
        db_query($update, $title, $url, $id, $tool);
    }else{
        //insert new
        $insert = "INSERT INTO {research_tool_links} (tool, title, url, created, changed) VALUES(%d, '%s', '%s',NOW(), NOW())";
        db_query($insert, $tool, $title, $url);
    }
    drupal_goto('research/tools/view/'.$tool);
}

/** research tool link delete form **/
function research_tool_link_delete_form(&$form_state, $link){
    $cancel_link = "research/tools/view/{$link->tool}";
    $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $link->id,
    );
    $form['tool'] = array(
        '#type' => 'hidden',
        '#value' => $link->tool,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#suffix' => l(t("Or cancel?"),$cancel_link),
    );
    return $form;
}
function research_tool_link_delete_form_submit($form, &$form_state){
    $id = (int)$form_state['values']['id'];
    $tool = $form_state['values']['tool'];
    $delete = "DELETE FROM {research_tool_links} WHERE id=%d";
    db_query($delete, $id);
    drupal_goto("research/tools/view/$tool");
}