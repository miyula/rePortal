$(document).ready(function(){
    $('#edit-tags-div').dialog({
        autoOpen: false,
        modal: true,
        width: 400
    });
});

function onclick_edit_tags(id){
    $('#edit-tags-div').dialog('open');
    alert(Drupal.settings.reProjectUser.base_path);
}