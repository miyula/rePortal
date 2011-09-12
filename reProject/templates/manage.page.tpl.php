<?php
// $Id$
/**
 * Template to display project manage page.
 *
 * Fields available:
 * $project, the project object
 */
?>
<div class="project-manage-div">
<div class="left-navigation-div">
    <ul>
        <li><a href="<?=url('project/'.$project->path)?>">Home page</a></li>
        <li><a href="<?=url('project/'.$project->path.'/manage/tools')?>">Manage tools</a></li>
        <li><a href="<?=url('project/'.$project->path.'/manage/contents')?>">Manage content</a></li>
        <li><a href="<?=url('project/'.$project->path.'/manage/persons')?>">Manage persons</a></li>
        <li>Send message</li>
        <li>Settings</li>
    </ul>
</div>
<div class="right-content-div">
    <h1><?=$project->subtitle; ?></h1>
    <?=$project->right_part; ?>
</div>
<div style="clear:both"></div>
</div>