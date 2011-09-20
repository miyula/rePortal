<?php

/**
 * Menu callback for project/%/manage/persons
 */
function project_manage_persons_list_callback($path){
    $project = prepare_project_manage_page($path);
    
    if ($_POST) {
        $name = $_POST['name'];
        $email = $_POST['email'];
        $phone = $_POST['phone'];
        $notes = $_POST['notes'];
        $insert = 'INSERT INTO {research_projects_participants_cards}(project, name, email, phone, notes, created, changed) VALUES(%d, "%s","%s", "%s","%s", NOW(), NOW())';
        db_query($insert, $project->id, $name, $email, $phone, $notes);
    }
    
    
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
        '<input type="text" name="name" size=15/>',
        '<input type="text" name="email" size=15/>',
        '<input type="text" name="phone" size=15/>',
        '<textarea name="notes" rows="4" cols="30"></textarea>',
        '<input type="submit" value="Add" />',
    );
        
    $sql2 = "SELECT id, name, email, phone, notes FROM {research_projects_participants_cards} WHERE project={$project->id} ORDER BY created DESC";
    $res = pager_query($sql2, 30);
    while($row=db_fetch_object($res)){
        $rows[] = array(
            '<div class="participant-card-name-div">'.$row->name.'</div>',
            array(
                'data'  =>'<div class="participant-card-email-div">Email: '.$row->email.'</div>'
                         .'<div class="participant-card-phone-div">Phone: '.$row->phone.'</div>'
                         .'<div class="participant-card-notes-div">Notes: '.$row->notes.'</div>',
                'colspan' => 3,
            ),
            '<p class="operation-p"><a class="delete_link" href="'.url("project/$path/manage/persons/participant/remove/{$row->id}").'">Remove</a></p>'
                .'<p class="operation-p"><a class="edit_link" href="'.url("project/$path/manage/persons/participant/edit/{$row->id}").'">Edit</a></p>'
                .'<p class="operation-p"><a class="tags_link window_tab" href="" target="_blank">Tags</a></p>',
        );
    }
        
    $content.= '<form action="" method="POST"><div class="project-manage-page-table-div">'.theme_table($header, $rows).'</div></form>';
    $content.= theme('pager');
    $project->right_part = $content;    
    return theme('project_manage_page',$project);
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