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
        <?php foreach($project->navigation as $n): ?>
            <li><a href="<?=url($n['url'])?>"><?=$n['title']?></a></li>
        <?php endforeach; ?>
    </ul>
</div>
<div class="right-content-div">
    <h1><?=$project->subtitle; ?></h1>
    <?=$project->right_part; ?>
</div>
<div style="clear:both"></div>
</div>