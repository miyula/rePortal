<?php
// $Id$
/**
 * Template to display project home page.
 *
 * Fields available:
 * $project, the project object
 */
?>
<div class="project-introduction-div"><?=$project->homepage->body;?></div>

<!-- start part of Research tools -->
<div id="research-tools-div">
<h3>Research tools</h3>
<div class="research-tool-blocks">
    <?php foreach($project->tools_list as $tool): ?>
    <div class="research-tool-block">
        <div class="research-tool-log-div">
            <a class="tab-window-link" href=""><img class="research-tool-logo" src="<?=$tool['logo']?>" alt="<?=$tool['name']?>"/></a>
        </div>
        <p class="research-tool-name-p"><a class="tab-window-link" href="<?=url("research/tools/{$tool['id']}")?>"><?=$tool['name']?></a></p>
        <ul>
            <li><a class="tab-window-link" href="<?=url("research/tools/{$tool['id']}")?>">Read more</a></li>
            <li><a class="tab-window-link" href="<?=$tool['download']?>">View data</a></li>
        </ul>
    </div>
    <?php endforeach; ?>
</div><!-- the end of class research-tool-blocks -->
</div>
<!-- end part of Research tools -->

<!-- footer navigation -->
<div class="footer-navigation">
    <p><a href="<?=url('project/'.$project->path.'/manage'); ?>">Manage</a> | <a href="<?=url('project/'.$project->path.'/help'); ?>">Help</a></p>
</div>