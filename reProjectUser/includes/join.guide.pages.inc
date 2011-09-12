<?php

/**
 * Menu callback for project/%/termofuse/%
 */
function project_termofuse_callback($path, $inviteCode){
    //get project
    load_project($path);
}