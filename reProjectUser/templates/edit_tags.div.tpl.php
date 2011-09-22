<?php
// $Id$
/**
 * Template to display edit tags div
 *
 * Fields available:
 * $tags
 * $project
 */
?>
<div id='edit-tags-div' title='Edit tags'>
    <div id='participants-tags-div'>
        <div class='participant-tag used-tag'>Male</div><div class='participant-tag'>Male</div><div class='participant-tag'>Male</div>
        <div style='clear:both'></div>
    </div>
    <hr style='width: 98%; border:0; height: 2px; background-color:#777;'/>
    <div id='new-tags-div' class='form-item'>
        <form action='' method='POST'>
            <input type="hidden" value="" name="project-input" id="edit-project-input" />
            <p>New tag: <input type='text' name='new-tag' id="new-tag-input" size='20' class="form-text" /><input type='button' value='Add' class="form-submit" /></p>
        </form>
    </div>
    <div id='all-tags-div'>
        <div class='participant-tag'>Male</div><div class='participant-tag'>Male</div><div class='participant-tag'>Male</div>
        <div style='clear:both;'></div>
    </div>
</div>