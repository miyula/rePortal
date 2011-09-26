<?php
// $Id$
/**
 * Template to display participant info
 *
 * Fields available:
 * $row
 */
?>
<div class="participant-card-email-div">
    <span class="title-span">Email</span><?=$row->email;?>
</div>
<div class="participant-card-phone-div">
    <span class="title-span">Phone</span><?=$row->phone;?>
</div>
<div class="participant-card-tags-div">
    <div class="title-div">Tags</div>
    <?php foreach($row->tags_array as $tag): ?>
    <div class='participant-tag used-tag'><?=$tag; ?></div>
    <?php endforeach; ?>
    <div style="clear:both"></div>
</div>
<div class="participant-card-notes-div">
    <div class="title-div">Notes</div>
    <div class="notes-content-div"><?=nl2br($row->notes);?></div>
    <div style="clear:both"></div>
</div>
