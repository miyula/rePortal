<?php
// $Id$
/**
 * Template to display project home page.
 *
 * Fields available:
 * $project, the project object
 */
?>
<div class="project-introduction-div"><?=add_translation_widget($project->homepage->body);?></div>

<!-- start part of Research tools -->
<div id="research-tools-div">
<h3>Research tools</h3>
<div class="research-tool-blocks">
    <?php foreach($project->tools_list as $tool): ?>
    <div class="research-tool-block">
        <div class="research-tool-log-div">
            <img class="research-tool-logo" src="<?=$tool->logo?>" alt="<?=$tool->name?>"/>
        </div>
        <p class="research-tool-name-p"><a class="tab-window-link" href="<?=url("research/tools/view/{$tool->id}")?>" target='_blank'><?=$tool->name?></a></p>
        <ul>
            <li><a class="tab-window-link list_link" href="<?=url("research/tools/view/{$tool->id}")?>" target='_blank'>Read more</a></li>
            <?php foreach($tool->links as $link): ?>
                <li><a class="tab-window-link list_link" href="<?=$link->url;?>" target='_blank'><?=$link->title;?></a></li>
            <?php endforeach; ?>
        </ul>
    </div>
    <?php endforeach; ?>
</div><!-- the end of class research-tool-blocks -->
</div>
<!-- end part of Research tools -->

<!-- footer navigation -->
<div class="footer-navigation">
    <p><a href="<?=url('project/'.$project->path.'/manage'); ?>">Manage</a> | <a href="<?=url('project/'.$project->path.'/content/Help_page'); ?>">Help</a></p>
</div>