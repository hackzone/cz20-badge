/**
 * Created by Tom on 5/27/2020.
 */

import {
    getnodepath, copyfile, createfile, createfolder,
    delfile, fetch_dir, isfile, movefile, runfile, savefile,
    savetextfile, readfile
} from './webusb';

import * as $ from 'jquery';

export let editor_handle = undefined;
export let editor_filename = "";

export function trash_ui() {
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    console.log(node);
    let file = getnodepath(node);
    console.log(file);
    if(!isfile(node)) return;
    if(confirm("Delete: " + file + "?")) {
        delfile(file)
    }
}

export function sync_ui() {
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) {
        $('#filebrowser').jstree(true).refresh_node(node["id"])
    }
}

export function rename_ui() {
    let filename = prompt("Please enter new filename", "");
    if (filename != null) {
        let node = $("#filebrowser").jstree("get_selected",true)[0];
        let source = getnodepath(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        } else {
            return;
        }
        let file = getnodepath(node) + "/" + filename;
        movefile(source, file);
    }
}

export function save_ui() {
    let contents = editor_handle.getValue();
    savetextfile(editor_filename, contents);
}

export function startapp_ui() {
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    console.log(node);
    let file = getnodepath(node);
    console.log(file);
    if(!isfile(node)) return;
    runfile(file);
}

export function mkdir_ui() {
    let folder_name = prompt("Please enter foldername", "New folder");
    if (folder_name != null) {
        let node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        let folder = getnodepath(node) + "/" + folder_name;
        console.log(folder);
        createfolder(folder);
    }
}

export function mkfile_ui() {
    let filename = prompt("Please enter filename", "New file.py");
    if (filename != null) {
        let node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        let file = getnodepath(node) + "/" + filename;
        console.log(file);
        createfile(file);
    }
}

export function upload_ui() {
    $('#file-input').trigger('click');
}


let filetocopy = "";
let filetocut = "";

export function copy_ui() {
    let filetocut = "";
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) return;

    let file = getnodepath(node);
    filetocopy = file;
    document.getElementById("paste_btn").disabled = false;
}

export function cut_ui() {
    filetocopy = "";
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) return;

    let file = getnodepath(node);
    filetocut = file;
    document.getElementById("paste_btn").disabled = false;
}

export function paste_ui() {
    document.getElementById("paste_btn").disabled = true;
    let node = $("#filebrowser").jstree("get_selected",true)[0];
    if(isfile(node)) {
        node = $('#filebrowser').jstree(true).get_node(node["parent"]);
    }
    let folder = getnodepath(node) + "/";
    console.log(folder);
    if(filetocut !== "") {
        movefile(filetocut, folder);
    } else if(filetocopy !== "") {
        copyfile(filetocopy, folder);
    }
}

export function readSingleFile(e) {
    let file = e.target.files[0];
    if (!file) {
        return;
    }
    console.log(file);
    let reader = new FileReader();
    reader.onload = function(e) {
        let node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        let filedir = getnodepath(node) + "/" + file["name"];
        console.log(filedir);
        let arrayBuffer = reader.result;
        console.log(arrayBuffer);
        savefile(filedir, arrayBuffer);
    }
    reader.readAsArrayBuffer(file);
}

export function treechanged(e, data) {
    console.log("tree event");
    console.log(data);
    if(data["action"] == "select_node") {
        let obj = data["node"];
        if(obj["icon"] != "far fa-file") return; //Exit if not a file
        if(obj["parents"]) { //We have parent, time to construct dir structure
            let dir_name = "/" + obj["text"];
            for(const par_id of obj["parents"]) {
                if(par_id != "#") {
                    let node = $('#filebrowser').jstree(true).get_node(par_id);
                    let folder_name = node["text"];
                    dir_name = "/" + folder_name + dir_name;
                }
            }
            console.log(dir_name);
            editor_filename = dir_name;
            readfile(editor_filename);
        }
    }
}