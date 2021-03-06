<?php

/**
 * Menu callback for research/tools and research/tools/list
 */
function research_tools_list_callback(){
    
    drupal_add_css(drupal_get_path('module','reTool').'/reTool.css');
    
    if(user_access('create research tool')){
        $content = '<p style="text-align:right;"><a href="'.url('research/tools/add').'" class="add_link">Add new tool</a></p>';
    }
    
    $rows = array();
    $header = array(
        array('data'=>t('Research tool'), 'width'=>'150px'),
        array('data'=>t('Short description'), 'width'=>'500px'),
        array('data'=>t('Ratings')),
    );
    
    //get tool list
    $sql = "SELECT id, name, logo, short_describe FROM {research_tools}";
    $res = pager_query($sql,30);
    
    while($row = db_fetch_object($res)){
        
        $count = "SELECT COUNT(*) FROM {research_project_tools_list} WHERE tool=%d";
        $count_res = db_result(db_query($count, $row->id));
        switch($count_res){
            case 0:
                $ratings = '';
                break;
            case 1:
                $ratings = '1 project is using this tool.';
                break;
            default :
                $ratings = "$count_res projects are using this tool.";
        }
        
        $rows[] = array(
            array(
                'data' => "<a href='".url('research/tools/view/'.$row->id)."'><img src='{$row->logo}' alt='{$row->name}' class='tool_logo_image'/><span class='tool-title'>{$row->name}</span></a>",
                'class' => 'tool-name',
            ),
            array(
                'data' => $row->short_describe,
                'class' => 'tool-describe',
            ),
            array(
                'data' => $ratings,
                'class' => 'tool-rating',
            ),            
        );
    }
    
    $content.= theme_table($header, $rows);
    $content.= theme('pager');
    
    return $content;
}

/**
 * Menu callback for research/tools/add
 */
function research_tools_add_callback(){
    return drupal_get_form('research_tool_form');  
}

/**
 * Menu callback for research/tools/view/%
 */
function research_tools_view_callback($tool_id){
    
    //get tool info
    $get_tool = "SELECT id, name, logo, introduction FROM {research_tools} WHERE id=%d";
    $tool = db_fetch_object(db_query($get_tool, $tool_id));
    
    if($tool){
        
        $breadcrumb = drupal_get_breadcrumb();
        $breadcrumb[] = l('Tools', 'research/tools/');
        drupal_set_breadcrumb($breadcrumb);
        
        //get projects count
        $count = "SELECT COUNT(*) FROM {research_project_tools_list} WHERE tool=%d";
        $count_res = db_result(db_query($count, $tool_id));
        $tool->projects = $count_res?$count_res:0;
        
        //get tool links
        $get_links = "SELECT title, url FROM {research_tool_links} WHERE tool=%d";
        $res = db_query($get_links, $tool_id);
        $links = array();
        while($link = db_fetch_object($res)){
            $links[] = $link;
        }
        $tool->links = $links;
        
        return theme('research_tool_view_page', $tool);
    }else{
        drupal_not_found();
    }
}


/**
 * Menu callback for research/tools/view/%/edit
 */
function research_tools_edit_callback($tool_id){
    
    //get tool info
    $get_tool = "SELECT id, name, logo, short_describe, introduction FROM {research_tools} WHERE id=%d";
    $tool = db_fetch_object(db_query($get_tool, $tool_id));
    if($tool){
        
        $breadcrumb = drupal_get_breadcrumb();
        $breadcrumb[] = l('Tools', 'research/tools/');
        $breadcrumb[] = l($tool->name, 'research/tools/view/'.$tool_id);
        drupal_set_breadcrumb($breadcrumb);
        
        return drupal_get_form('research_tool_form',$tool);
    }else{
        drupal_not_found();
    }
    
}

/**
 * Menu callback for research/tools/view/%/delete
 */
function research_tools_delete_callback(){
    $get_tool = "SELECT id, name, logo, short_describe, introduction FROM {research_tools} WHERE id=%d";
    $tool = db_fetch_object(db_query($get_tool, $tool_id));
    if($tool){
        return drupal_get_form('research_tool_form',$tool);
    }else{
        drupal_not_found();
    }
}

/************ research tool form **************/
function research_tool_form(&$form_state, $tool=null){
    
    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['id'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($tool->id)?$tool->id:0,
    );
    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Tool name'),
        '#description'      => t('The name of the research tool.'),
        '#required'         => TRUE,
        '#default_value'    => isset($tool->name)?$tool->name:'',
    );
    $form['logo'] = array(
        '#type'             => 'file',
        '#title'            => t('Tool logo'),
        '#description'      => t('Upload a image file as the logo of the tool. If no change, keep this empty'),
        //'#required'         => FALSE,
    );
    if(isset($tool->logo)){
        $form['logo-image'] = array(
            '#value'        => "<div><image src='{$tool->logo}'/></div>",
        );
    }
    $form['logo-path'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($tool->logo)?$tool->logo:'',
    );
    $form['short_describe'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Short description'),
        '#description'      => t('Same short phrases or sentences to describe this tool. E.g: Android application, Track locations.'),
        '#default_value'    => isset($tool->short_describe)?$tool->short_describe:'',
        '#required'         => TRUE,
    );
    $form['introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Introduction'),
        '#description'      => t('The introduction for the tool.'),
        '#default_value'    => isset($tool->introduction)?$tool->introduction:'',
        '#required'         => TRUE,
    );
    $form['submit'] = array(
        '#type'             => 'submit',
        '#value'            => t('Save'),
    );
    
    return $form;
}
function research_tool_form_submit($form, &$form_state){
    //get value from form
    $id = $form_state['values']['id'];
    $name = $form_state['values']['name'];
    $logo_path = $form_state['values']['logo-path'];
    $describe = $form_state['values']['short_describe'];
    $introduction = $form_state['values']['introduction'];
    
    //Upload image
    $validators = array(
        'file_validate_is_image' => array(),
    );
    $file = file_save_upload('logo', $validators);
    if($file!=0){
        $info = image_get_info($file->filepath);
        //create tools_logo folder if not existed
        $folder = "tools_logo";
        $path = file_create_path($folder);
        file_check_directory($path, FILE_CREATE_DIRECTORY);
        
        $destination = $folder.'/'.time().'.'. $info['extension'];
        if(file_copy($file, $destination, FILE_EXISTS_REPLACE)){
	    $logo_path = base_path().$file->filepath;
        }else{
            //keep default logo
            $logo_path = dbase_path().drupal_get_path('module','reTool').'/images/nologo.png';
        }
    }
    
    if($id>0){
        //update
        $update = 'UPDATE {research_tools} SET name="%s", logo="%s", short_describe="%s", introduction="%s", changed=NOW() WHERE id=%d';
        db_query($update, $name, $logo_path, $describe, $introduction, $id);
    }else{
        //insert new into database
        $sql = 'INSERT INTO {research_tools}(name, logo, short_describe, introduction, created, changed) VALUES("%s", "%s", "%s", "%s", NOW(), NOW())';
        db_query($sql, $name, $logo_path, $describe, $introduction);
        $id = mysql_insert_id();
    }
    
    
    drupal_goto('research/tools/view/'.$id);
}

