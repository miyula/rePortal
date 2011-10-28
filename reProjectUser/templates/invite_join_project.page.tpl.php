<?php
// $Id$
/**
 * Template for invite page div
 *
 * Fields available:
 * $person, object
 * $invite_message, string
 */
?>
<div class="invite-dialog-div">
    <p class="title">Hello<?=isset($person->username)?' '.$person->username:'';?>,</p>
    <p class="message"><?=$invite_message;?></p>
    <div class='invite-forms'>
    <?php if(isset($person->exiting_user_join_form) ): ?>
        <?=$person->exiting_user_join_form; ?>
    <?php endif; ?>
    <?php if(isset($person->login_to_join_form) ): ?>
        <div class="invite-user-form-div">
            <h3>Already have an account?</h3>
            <?=$person->login_to_join_form; ?>
        </div>
    <?php endif; ?>
     <?php if(isset($person->register_to_join_form) ): ?>
        <div class='middle-bar'></div>
        <div class="invite-user-form-div">
            <h3>Are you a new user?</h3>
            <?=$person->register_to_join_form; ?>
        </div>
    <?php endif; ?>
    <div style='clear:both'></div>
    </div>
</div>