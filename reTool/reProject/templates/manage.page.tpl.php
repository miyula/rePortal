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
    <div class="sub-title-div"><span class="sub-title-span"><?=$project->subtitle; ?></span><?=$project->title_tabs; ?></div>
    <?=$project->right_part; ?>
</div>
<div style="clear:both"></div>
</div>