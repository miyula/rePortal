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

/**
 * Menu callback for project/%/manage/tools
 */
function project_manage_tools_list_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        
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
                "<a href='' class='delete_link'>Remove</a>",
            );
        }
        $content.= theme_table($header, $rows);
        $content.= theme('pager');
        $project->subtitle = 'Tools management';
        $project->right_part = $content;    
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}

/**
 * Menu callback for project/%/manage/tools/selete
 */
function project_manage_tools_selete_list_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        
        //get right part info
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
                "<a href='".url("project/$path/manage/tools/selete/{$row->id}")."' class='add_link'>Add to project</a>",
            );
        }
        $content.= theme_table($header, $rows);
        $content.= theme('pager');
        $project->subtitle = 'Tools management';
        $project->right_part = $content;    
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}

/**
 * Menu callback for project/%/manage/tools/selete/%
 */
function project_manage_tools_selete_new_callback($path, $id){
    $project = load_project($path);
    if($project){
        project_bind_tool($project, $id);
        drupal_goto('project/'.$path.'/manage/tools');
    }
    drupal_not_found();
}

/**
 * Menu callback for project/%/manage/persons
 */
function project_manage_persons_list_callback($path){
    $project = load_project($path);
    if($project){
        drupal_add_css(drupal_get_path('module','reProject').'/css/manage.css');
        
        //get right part info
        
        //get researchers
        $content.= '<h2>Researchers list</h2>';
        $content.= '<p><a href="'.url('project/'.$path.'/manage/persons/researcher/add').'" class="add_link">Add new tool</a></p>';
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
        
        //get normal user
        $content.= '<h2>Participants list</h2>';
        $content.= '<p><a href="'.url('project/'.$path.'/manage/persons/normal/add').'" class="add_link">Invite new</a></p>';
        $rows = array();
        $header = array(
            //array('data'=>'Photo'),
            array('data'=>'Name'),
            array('data'=>'Email'),
        );
        
        $sql = "SELECT name, mail FROM {research_projects_persons_list} AS p, {users} AS u WHERE p.project=%d AND p.user=u.uid AND p.role='normal'";
        $res = pager_query($sql, 30, $project->id);
        while($row=db_fetch_object($res)){
            $rows[] = array(
                $row->name,
                $row->mail,
            );
        }
        
        $content.= theme_table($header, $rows);
        $content.= theme('pager');
        $project->right_part = $content;    
        return theme('project_manage_page',$project);
    }
    drupal_not_found();
}

/**
 * Menu callback for project/%/manage/persons/researcher/add
 */
function project_manage_persons_researcher_add_callback($path){
    $project = load_project($path);
    if($project){
        return drupal_get_form('project_invite_research_form', $project->id);
    }
    drupal_not_found();
}

/** Invite researcher form **/
function project_invite_research_form(&$form_state, $pid){
    $form['pid'] = array(
        '#type' => 'hidden',
        '#value' => $pid,
    );
    $form['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email address'),
        '#description' => t('The email account of the researcher'),
        '#required' => TRUE,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
    );
    
    return $form;
}
function project_invite_research_form_submit($form, &$form_state){
    $pid = $form_state['values']['pid'];
    $email = $form_state['values']['email'];
    
    //find user
    $find = "SELECT uid FROM {users} WHERE mail='%s'";
    $uid = db_result(db_query($find, $email));
    
    if($uid){
        add_user_to_project($pid, $uid, PROJECT_RESEARCHER);
    }
    drupal_goto(get_project_url($pid).'/manage/persons');
}

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