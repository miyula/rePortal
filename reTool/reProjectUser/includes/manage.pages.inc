<?php

/**
 * Menu callback for project/%/manage/persons
 */
function project_manage_persons_list_callback($path){
    $project = prepare_project_manage_page($path);    
    
    drupal_add_css(drupal_get_path('module','reProjectUser').'/css/user_manage.css');
    
    //add jQuery ui
    load_jquery_ui();
    drupal_add_js(drupal_get_path('module','reProjectUser').'/js/edit_tags_window.js');
    
    //add ajax link Drupal.settings.reProjectUser.base_path
    drupal_add_js(array('reProjectUser'=>array('base_path'=>url('project/'.$path.'/manage/persons/'))),'setting');
    //get right part info
    
    //get researchers
    $content.= '<h2>Researchers list</h2>';
    $content.= '<p><a href="'.url('project/'.$path.'/manage/persons/researcher/add').'" class="add_link">Add researcher</a></p>';
    $rows = array();
    $header = array(
        //array('data'=>'Photo'),
        array('data'=>'Name'),
        array('data'=>'Email'),
    );
        
    $sql = "SELECT name, mail FROM {research_projects_persons_list} AS p, {users} AS u WHERE p.project=%d AND p.user=u.uid AND p.role='researcher'";
    $res = db_query($sql, $project->id);
    while($row=db_fetch_object($res)){
        $rows[] = array(
            $row->name,
            $row->mail,
        );
    }
    $content.= theme_table($header, $rows);
    
    $content.= '<p>&nbsp;</p>';    
    //get normal user
    $content.= '<h2>Participants list</h2>';
        
    $rows = array();
    $header = array(
        array('data'=>'Name'),
        array('data'=>'Email'),
        array('data'=>'Phone'),
        array('data'=>'Notes'),
        array('data'=>'Operations'),
    );
    
    $rows[] = array(
        array(
            'data'  =>   '<div id="new_participant_info_card_form_div">'.drupal_get_form('new_participant_info_card_form', $project).'</div>',
            'colspan' => 5,
        ),
    );
        
    $sql2 = "SELECT id, name, email, phone, tags, notes FROM {research_projects_participants_cards} WHERE project={$project->id} ORDER BY created DESC";
    $res = pager_query($sql2, 30);
    while($row=db_fetch_object($res)){
        $tags = get_tags_as_array($row->tags);
        $row->tags_array = $tags;
        $rows[] = array(
            '<div class="participant-card-name-div">'.$row->name.'</div>',
            array(
                'data'  => theme('participants_card_div', $row, $project->path),
                'colspan' => 3,
            ),
            '<p class="operation-p"><a class="delete_link" href="'.url("project/$path/manage/persons/participant/remove/{$row->id}").'">Remove</a></p>'
                .'<p class="operation-p"><a class="edit_link" href="'.url("project/$path/manage/persons/participant/edit/{$row->id}").'">Edit</a></p>'
                .'<p class="operation-p"><a class="tags_link window_tab" href="javascript:onclick_edit_tags('.$row->id.');" target="_blank">Tags</a></p>',
        );
    }
    $content.= theme('participants_tags_div');
    $content.= '<div class="project-manage-page-table-div">'.theme_table($header, $rows).'</div>';
    $content.= theme('pager');
    $project->right_part = $content;    
    return theme('project_manage_page',$project);
}


/**
 * Menu callback for project/%/manage/persons/filter/%
 */
function project_manage_persons_filter_callback($path, $tag){
    $project = prepare_project_manage_page($path);    
    
    drupal_add_css(drupal_get_path('module','reProjectUser').'/css/user_manage.css');
    
    //add jQuery ui
    load_jquery_ui();
    drupal_add_js(drupal_get_path('module','reProjectUser').'/js/edit_tags_window.js');
    
    //add ajax link Drupal.settings.reProjectUser.base_path
    drupal_add_js(array('reProjectUser'=>array('base_path'=>url('project/'.$path.'/manage/persons/'))),'setting');
    //get right part info
    
    //get researchers
    $content.= '<h2>Researchers list</h2>';
    $content.= '<p><a href="'.url('project/'.$path.'/manage/persons/researcher/add').'" class="add_link">Add researcher</a></p>';
    $rows = array();
    $header = array(
        //array('data'=>'Photo'),
        array('data'=>'Name'),
        array('data'=>'Email'),
    );
        
    $sql = "SELECT name, mail FROM {research_projects_persons_list} AS p, {users} AS u WHERE p.project=%d AND p.user=u.uid AND p.role='researcher'";
    $res = db_query($sql, $project->id);
    while($row=db_fetch_object($res)){
        $rows[] = array(
            $row->name,
            $row->mail,
        );
    }
    $content.= theme_table($header, $rows);
    
    $content.= '<p>&nbsp;</p>';    
    //get normal user
    $content.= '<h2>Participants list</h2>';
    $content.= '<p><a href="'.url("project/$path/manage/persons").'" class="reload_link">Reload all</a></p>';   
    $rows = array();
    $header = array(
        array('data'=>'Name'),
        array('data'=>'Email'),
        array('data'=>'Phone'),
        array('data'=>'Notes'),
        array('data'=>'Operations'),
    );
    
    $rows[] = array(
        array(
            'data'  =>   '<div id="new_participant_info_card_form_div">'.drupal_get_form('new_participant_info_card_form', $project).'</div>',
            'colspan' => 5,
        ),
    );
        
    $sql2 = "SELECT id, name, email, phone, tags, notes FROM {research_projects_participants_cards} WHERE project=%d AND tags LIKE '%;$tag;%' ORDER BY created DESC";
    $res = db_query($sql2, $project->id, $tag);
    while($row=db_fetch_object($res)){
        $tags = get_tags_as_array($row->tags);
        $row->tags_array = $tags;
        $rows[] = array(
            '<div class="participant-card-name-div">'.$row->name.'</div>',
            array(
                'data'  => theme('participants_card_div', $row, $project->path, $tag),
                'colspan' => 3,
            ),
            '<p class="operation-p"><a class="delete_link" href="'.url("project/$path/manage/persons/participant/remove/{$row->id}").'">Remove</a></p>'
                .'<p class="operation-p"><a class="edit_link" href="'.url("project/$path/manage/persons/participant/edit/{$row->id}").'">Edit</a></p>'
                .'<p class="operation-p"><a class="tags_link window_tab" href="javascript:onclick_edit_tags('.$row->id.');" target="_blank">Tags</a></p>',
        );
    }
    $content.= theme('participants_tags_div');
    $content.= '<div class="project-manage-page-table-div">'.theme_table($header, $rows).'</div>';
    $project->right_part = $content;    
    return theme('project_manage_page',$project);
}

/** New participant info card form **/
function new_participant_info_card_form($form_state, $project){
    $form['project'] = array(
        '#type'         => 'hidden',
        '#value'        => $project->id,
    );
    $form['name'] = array(
        '#type'         => 'textfield',
        '#size'         => 15,
    );
    $form['email'] = array(
        '#type'         => 'textfield',
        '#size'         => 15,
    );
    $form['phone'] = array(
        '#type'         => 'textfield',
        '#size'         => 15,
    );
    $form['notes'] = array(
        '#type'         => 'textarea',
        '#cols'         => 15,
        '#rows'         => 3,
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Add'),
    );
    return $form;
}
function new_participant_info_card_form_validate($form, &$form_state){
    $name = $form_state['values']['name'];
    if(empty($name)){
        form_set_error('name','The name is required.');
    }
    
    $email = $form_state['values']['email'];
    if(!empty($email) && !valid_email_address($email)){
        form_set_error('email','Invalid email address.');
    }
    
    $phone = $form_state['values']['phone'];
    if(!empty($phone) && !valid_phone_number($phone)){
        form_set_error('phone','Invalid phone number.');
    }
    
    if(empty($email) && empty($phone)){
        form_set_error('email', 'The email and phone should not all be empty!');
        form_set_error('phone', 'The email and phone should not all be empty!');
    }
    
}
function new_participant_info_card_form_submit($form, &$form_state){
    $project = $form_state['values']['project'];
    $name = $form_state['values']['name'];
    $email = $form_state['values']['email'];
    $phone = $form_state['values']['phone'];
    $notes = filter_filter('process', 0, -1, $form_state['values']['notes']);
    $insert = 'INSERT INTO {research_projects_participants_cards}(project, name, email, phone, notes, created, changed) VALUES(%d, "%s","%s", "%s","%s", NOW(), NOW())';
    db_query($insert, $project, $name, $email, $phone, $notes);
}




////////////////// NOT USED OLD CODE //////////////////////
/**
 * Menu callback for project/%/manage/persons/normal/add
 */
function project_manage_persons_normal_add_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        $project->right_part = drupal_get_form('project_invite_participants_form',$project);    
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}

/** Invite new participants form **/
function project_invite_participants_form(&$form_state, $project){
    $form['pid'] = array(
        '#type' => 'hidden',
        '#value' => $project->id,
    );
    $form['invitation-list'] = array(
        '#type' => 'textarea',
        '#title' => t('Invitation List'),
        '#description' => t('Please split each item by ",". e.g. 123@gmail.com, avc@tkk.fi'),
        '#required' => TRUE,
    );
    $form['subjet-title'] = array(
        '#type' => 'textfield',
        '#title' => t('Mail subject'),
        '#default_value' => 'Invitation from '.$project->name, 
    );
    $invite_body = "<p>{$project->name} invites you to join their project.  Click the link to accept the term of use:</p>"
                  ."<p>http://".$_SERVER['HTTP_HOST'].url("project/{$project->path}/termofuse")."</p>"
                  ."<p>If clicking the link doesn't work, please copy and paste it into your browser.</p>"
                  ."<p>Please don't reply this mail.</p>"
                  ."<p>Best Regards,</p>"
                  ."<p>Yours,</p>"
                  ."<p>{$project->name} Team</P>";

    $form['message'] = array(
        '#type' => 'textarea',
        '#title' => t('Message body'),
        '#description' => t('This message will be sent to all persons on above invitation list'),
        '#required' => TRUE,
        '#default_value' => $invite_body,
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Invite'),
    );
    return $form;
}
function project_invite_participants_form_submit($form, &$form_state){
    $mail_list = $form_state['values']['invitation-list'];
    $title = $form_state['values']['subjet-title'];
    $body = $form_state['values']['message'];
    
    message_send_http_mail($mail_list,$title,$body);
}