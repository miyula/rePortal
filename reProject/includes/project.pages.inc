<?php
/**
 * Menu callback for projects/new
 */
function projects_new_callback(){
    //get project form
    $content = drupal_get_form('create_new_project_form');
    return $content;
}
/**
 * Menu callback for projects/new/editpages/
 */
function projects_new_editpages_callback(){
    //get project form
    if(isset($_SESSION['new_project_id'])&&$_SESSION['new_project_id']>0){
        $content = drupal_get_form('first_edit_project_pages_form',$_SESSION['new_project_id']);
    }else{
        $content = '<h1>Session missing! This page is noly allow when first create project.</h1>';
    }
    
    return $content;
}
/**
 * Menu callback for projects/list
 */
function projects_list_callback(){
    
    $content = '';
    $rows = array();
    $header = array(
        array('data'=>'Project name'),
        array('data'=>'Last update'),
        array('data'=>'Founder'),
    );
    
    //get project list
    $sql = "SELECT id, creator, name, url, path, founder, introduction, created, changed FROM {research_projects} LEFT JOIN {research_projects_activity} ON project=id ORDER BY activity DESC";
    
    $res = pager_query($sql,30);
    while($row = db_fetch_object($res)){
        $rows[] = array(
            l($row->name,'project/'.$row->path),
            format_interval(time()-strtotime($row->changed)).' ago',
            $row->founder,
        );
    }
    
    $content.= theme_table($header, $rows);
    $content.= theme('pager');
    
    return $content;
    
}

/**
 * Menu callback for project/%
 */
function project_display_callback($path){
    $project = load_project($path);
    if($project){
        $homepage = load_homepage_node($project);
        $project->homepage = $homepage;
        
        //load tools list
        $sql = 'SELECT t.id id, name, logo, introduction FROM {research_project_tools_list} AS p, {research_tools} AS t '
              .'WHERE p.project=%d AND p.tool=t.id';
        $tools = array();
        $res = db_query($sql, $project->id);
        while($tool=db_fetch_object($res)){
            $tools[] = $tool;
        }
        $project->tools_list = $tools;
        return theme('project_home_page', $project);
    }
    drupal_not_found();
}

/***************** Create new project form********************/

/**
 * Form for create new project
 */
function create_new_project_form(){

    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Project name'),
        '#description'      => t('The name of the project.'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->name)?$project->name:'',
    );
    $form['founder'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Founder'),
        '#description'      => t('The name of founders'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->founder)?$project->founder:'',
    );
    $form['path'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Define a path'),
        '#description'      => t('Unique path to access the project. By a-z, A-Z, 0-9 and _'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->path)?$project->path:'',
    );
    $form['Introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Write a short introduction for your project'),
        '#description'      => t('A short description of the project'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->introduction)?$project->introduction:'',
    );
    $form['next_step'] = array(
        '#type'             => 'submit',
        '#value'            => t('Next'),
    );
    return $form;
}

/**
 * Handle validation for project form
 */
function create_new_project_form_validate($form, &$form_state){
    //$name = $form_state['values']['name'];
    $path = $form_state['values']['path'];
    //$introduction = $form_state['values']['introduction'];
    
    //validate if the path contain illegal characters
    if(!preg_match('/^([0-9]|[_]|[a-z]|[A_Z])*$/',$path)){
        form_set_error('path','The path contain illegal characters. Only letters, numbers and _ are allowed');
    }
    
    //validate if the path is unique
    $sql = "SELECT COUNT(*) FROM {research_projects} WHERE path='%s'";
    if(db_result(db_query($sql, $path))>0){
        form_set_error('path','Unlucky! The path has alreay been used by another project.');
    }
}

/**
 * Handle submit for project form
 */
function create_new_project_form_submit($form, &$form_state){
    
    //get current user
    global $user;
    if($user->uid==0){
        //not authentication user, redirect to login page
        drupal_goto('user');
    }
    
    $project = new stdClass;
    $project->creator       = $user->uid;
    $project->name          = $form_state['values']['name'];
    $project->founder       = $form_state['values']['founder'];
    $project->path          = $form_state['values']['path'];
    $project->introduction  = $form_state['values']['introduction'];
    
    if(create_new_project($project)){
        //succeed to create new project
        $_SESSION['new_project_id'] = $project->id;
        drupal_goto("projects/new/editpages");
    }else{
        drupal_set_message($message = 'Failed to create new project', $type = 'error');
        drupal_goto('user');
    }
}



/******************* Edit project form **********************/

/**
 * Form for edit research  project
 */
function edit_research_project_form(&$form_state, $project=null){
    $form['id'] = array(
        '#type'             => 'hidden',
        '#value'            => isset($project->id)?$project->id:0,
    );
    $form['founder'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Founder'),
        '#description'      => t('The name of founders'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->founder)?$project->founder:'',
    );
    $form['name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Project name'),
        '#description'      => t('The name of the project.'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->name)?$project->name:'',
    );
    $form['path'] = array(
        '#type'             => 'textfield',
        '#title'            => t('The path to access project'),
        '#description'      => t('Unique path to access the project. By a-z, A-Z, 0-9 and _'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->path)?$project->path:'',
    );
    $form['Introduction'] = array(
        '#type'             => 'textarea',
        '#title'            => t('Introduction'),
        '#description'      => t('A short description of the project'),
        '#required'         => TRUE,
        '#default_value'    => isset($project->introduction)?$project->introduction:'',
    );
    return $form;
}

/**
 * Handle validation for project form
 */
function edit_research_project_form_validate($form, &$form_state){
    //$name = $form_state['values']['name'];
    $id = $form_state['values']['id'];
    $path = $form_state['values']['path'];
    //$introduction = $form_state['values']['introduction'];
    
    //validate if the path contain illegal characters
    if(!preg_match('/^([0-9]|[_]|[a-z]|[A_Z])*$/',$path)){
        form_set_error('path','The path contain illegal characters. Only letters, numbers and _ are allowed');
    }
    
    //validate if the path is not used by other project
    $sql = "SELECT id FROM {research_projects} WHERE path='%s'";
    $result = db_result(db_query($sql, $path, $id));
    if($result && $result<>$id){
        form_set_error('path','Unlucky! The path has alreay been used by another project.');
    }
}

/**
 * Handle submit for project form
 */
function edit_research_project_form_submit($form, &$form_state){
    
}

/**
 * Form for first edit project pages
 */
function first_edit_project_pages_form(&$form_state, $pid=0){
    
    $sql = 'SELECT node FROM {research_projects_contents} WHERE project=%d AND type="%s"';
    
    //get homepage
    $homepage_id = db_result(db_query($sql, (int)$pid, 'homepage'));
    if($homepage_id){
        $homepage = node_load($homepage_id);
    }
    
    //get helppage
    $helppage_id = db_result(db_query($sql, (int)$pid, 'helppage'));
    if($helppage_id){
        $helppage = node_load($helppage_id);
    }
    
    $form['project_id'] = array(
        '#type'         => 'hidden',
        '#value'        =>  ''.$pid, 
    );
    $form['homepage'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Home page content'),
        '#description'  => t('This is a public page. Please not put any confidential informatoin here!'),
        '#required'     => TRUE,
        '#default_value'      => $homepage->body,
    );
    $form['homepage_nid'] = array(
        '#type'         => 'hidden',
        '#value'        =>  ''.$homepage_id, 
    );
    $form['helppage'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Help page content'),
        '#description'  => t('This is a public page. Please not put any confidential informatoin here!'),
        '#required'     => TRUE,
        '#default_value'  => $helppage->body,
    );
    $form['helppage_nid'] = array(
        '#type'         => 'hidden',
        '#value'        =>  ''.$helppage_id, 
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => t('Save'),
    );
    
    return $form;
}

/**
 * Handle submit for first edit project pages
 */
function first_edit_project_pages_form_submit($form, &$form_state){
    //die(print($form_state));
    
    $pid = $form_state['values']['project_id'];
    $homepage_body = $form_state['values']['homepage'];
    $homepage_nid = $form_state['values']['homepage_nid'];
    $helppage_body = $form_state['values']['helppage'];
    $helppage_nid = $form_state['values']['helppage_nid'];
    
    
    //save homepage
    $homepage = node_load($homepage_nid);
    $homepage->body = $homepage_body;
    node_save($homepage);
    
    //save helppage
    $helppage = node_load($helppage_nid);
    $helppage->body = $helppage_body;
    node_save($helppage);
    
    //get project path
    drupal_goto(get_project_url($pid));
}

/**
 * Handle operation to create a new project().
 */
function create_new_project($project){
    //step.1 save project info
    if(_research_projects_save($project)){
        
        //log operation
        log_operation($project->id, 'information', "Create new project", url('project/create'), REPROJECT_MODULE_NAME);
        
        //update project activity
        update_project_activity($project->id, 1);
        
        //step.2 create default page for project
        
        global $user;
        
        //create defautl home page
        $node = new stdClass;
        $node->title = "Home page";
        $node->body = "Please input home page information here for your project. The home page is a public page that everyone can see. Please don't put any confidential informatoin here!";
        $node->uid = $user->uid;
        $node->type = 'projectcontent';
        node_save($node);
        project_add_content($project->id, $node->nid, 'homepage', FALSE);
        
        //create defautl help page
        $node = new stdClass;
        $node->title = "Help page";
        $node->body = "Please input help infor here. The help page is to guide people who are interested in how to participate or contact project. The help page is a public page that everyone can see. Please don't put any confidential informatoin here!";
        $node->uid = $user->uid;
        $node->type = 'projectcontent';
        node_save($node);
        project_add_content($project->id, $node->nid, 'helppage', FALSE);
        
        
        return TRUE;
        
        
    }else{
        drupal_set_message($message = 'Failed to create new project', $type = 'error');
        return FALSE;
    }
    
}