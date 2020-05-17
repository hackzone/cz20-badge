//Init ace editor
var editor = ace.edit("editor");
    editor.setTheme("ace/theme/monokai");
    editor.session.setMode("ace/mode/python");
var editor_filename = "";

var device;
var queue = new Queue();
var waiting_command = 1;
var command;
var size;
var received;
var payload;
var cb_reply;

//Function to create valid packet. Size is the payload size, command is the command id
function buildpacket(size, command) {
    var arraybuffer = new ArrayBuffer(8+size);
    var buffer = new Uint8Array(arraybuffer);
    new DataView(arraybuffer).setUint16(0, command, true);
    new DataView(arraybuffer).setUint32(2, size, true);
    buffer[6] = 0xDE;
    buffer[7] = 0xAD;
    return buffer;
}

function buildpacketWithFilename(size, command, filename) {
    buffer = buildpacket(filename.length+1+size, command);
    for(var i = 0; i<filename.length; i++) {
        buffer[8+i] = filename.charCodeAt(i);
    }
    buffer[8+filename.length] = 0;
    return buffer;
}

function getnodepath(obj) {
    var dir_name = "/";
    if(obj["parent"]) { //We have parent, time to construct dir structure
        dir_name = "/" + obj["text"];
        for(const par_id of obj["parents"]) {
            console.log(par_id);
            if(par_id != "#") {
                var node = $('#filebrowser').jstree(true).get_node(par_id);
                console.log(node);
                var folder_name = node["text"];
                dir_name = "/" + folder_name + dir_name;
            }
        }
    }
    return dir_name;
}

function isfile(obj) {
    return obj['icon'] == "far fa-file";
}

function fetch_dir(obj, cb) {
    cb_reply = cb;
    console.log(obj);
    var dir_name = "/";
    if(obj["parent"]) { //We have parent, time to construct dir structure
        dir_name = "/" + obj["text"];
        for(const par_id of obj["parents"]) {
            console.log(par_id);
            if(par_id != "#") {
                var node = $('#filebrowser').jstree(true).get_node(par_id);
                console.log(node);
                var folder_name = node["text"];
                dir_name = "/" + folder_name + dir_name;
            }
        }
        console.log(dir_name);
    }
    buffer = buildpacket(dir_name.length+1, 4096);
    for(var i = 0; i<dir_name.length; i++) {
        buffer[8+i] = dir_name.charCodeAt(i);
    }
    buffer[8+dir_name.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);

}

function readfile(dir_name) {
    buffer = buildpacketWithFilename(0, 4097, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function createfile(dir_name) {
    buffer = buildpacketWithFilename(0, 4098, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function delfile(dir_name) {
    buffer = buildpacketWithFilename(0, 4099, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function runfile(dir_name) {
    buffer = buildpacketWithFilename(0, 0, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}


function duplicatefile(source, destination) {
    buffer = buildpacket(source.length+1+destination.length+1, 4100);
    for(var i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;
    
    for(var i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function movefile(source, destination) {
    buffer = buildpacket(source.length+1+destination.length+1, 4101);
    for(var i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;
    
    for(var i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function copyfile(source, destination) {
    buffer = buildpacket(source.length+1+destination.length+1, 4100);
    for(var i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;
    
    for(var i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function savetextfile(filename, contents) {
    console.log(filename)
    buffer = buildpacketWithFilename(contents.length, 4098, filename);    
    for(var i = 0; i<contents.length; i++) {
        buffer[8+filename.length+1+i] = contents.charCodeAt(i);
    }

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function savefile(filename, contents) {
    buffer = buildpacketWithFilename(contents.byteLength, 4098, filename);
    new Uint8Array(buffer, 8+filename.length+1, contents.byteLength).set(new Uint8Array(contents.buffer));    

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function createfolder(folder) {
    buffer = buildpacketWithFilename(0, 4102, folder);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function treechanged(e, data) {
    console.log("tree event");
    console.log(data);
    if(data["action"] == "select_node") {
        obj = data["node"];
        if(obj["icon"] != "far fa-file") return; //Exit if not a file
        if(obj["parents"]) { //We have parent, time to construct dir structure
            dir_name = "/" + obj["text"];
            for(const par_id of obj["parents"]) {
                if(par_id != "#") {
                    var node = $('#filebrowser').jstree(true).get_node(par_id);
                    var folder_name = node["text"];
                    dir_name = "/" + folder_name + dir_name;
                }
            }
            console.log(dir_name);
            editor_filename = dir_name;
            readfile(dir_name);
        }
    }
}

function handlePacket(id, data) {
    switch(id) {
        case 4096:
            var textdecoder = new TextDecoder("ascii");
            dir_structure = textdecoder.decode(data).split('\n');
            console.log(dir_structure);
            var data_structure = [];
            for(var i = 1; i < dir_structure.length; i++) {
                var type = dir_structure[i].charAt(0) == "d";
                var child = {};
                child["text"] = dir_structure[i].substr(1);
                if(type) {
                    if(dir_structure[i] == "dflash") {
                        child["icon"] = "fas fa-microchip";
                    } else if(dir_structure[i] == "dsdcard") {
                        child["icon"] = "fas fa-sd-card";
                    } else {
                        child["icon"] = "far fa-folder";
                    }
                } else {
                    child["icon"] = "far fa-file";
                }
                child["state"] = {"opened":false, "disable":false, "selected":false};
                if(type) {
                    child["children"] = true;
                }
                data_structure.push(child);
            }
            cb_reply.call(this, data_structure);
            // try {
            //     json_data = JSON.parse(json_data);
            //     $('#filebrowser').jstree(true).settings.core.data = json_data;
            //     $('#filebrowser').jstree(true).refresh();
            // } catch (e) {
            //     console.log("Error in file directory json.");
            // }
        break;
        case 4097:
            var textdecoder = new TextDecoder("ascii");
            file_contents = textdecoder.decode(data);
            console.log(file_contents);
            editor.session.setValue(file_contents);
        break;
        default :
            var textdecoder = new TextDecoder("ascii");
            file_contents = textdecoder.decode(data);
            console.log(file_contents);
        break;
    }
}


let readLoop = () => {
    device.transferIn(3, 64).then(result => {
        if(waiting_command) {
            waiting_command = 0;
            command = result.data.getUint16(0, true);
            size = result.data.getUint32(2, true);
            console.log("Trans: "+size);
            var verif = result.data.getUint16(6);
            received = result.data.byteLength-8;
            console.log("Verif: "+verif);
            payload = new ArrayBuffer(size);
            new Uint8Array(payload, 0, size).set(new Uint8Array(result.data.buffer, 8, Math.min(size, received)));
        } else {
            console.log(received + " " + size + " " + result.data.byteLength);
            new Uint8Array(payload, received, size-received).set(new Uint8Array(result.data.buffer));
            received = received + result.data.byteLength;
            console.log("rec: "+received+"/"+size);
        }
        if(received == size) {
            console.log("transfer complete");
            waiting_command = 1;
            handlePacket(command, payload);
        }
        readLoop();
  }, error => {
    this.onReceiveError(error);
  });
};

function connect() {
    waiting_command = 1;
    command = 0;
    size = 0;
    received = 0;
    payload = 0;
    navigator.usb.requestDevice({ filters: [{ vendorId: 0xcafe }] })
    .then(selectedDevice => {
    device = selectedDevice;
    return device.open(); // Begin a session.
    })
    .then(() => device.selectConfiguration(1)) // Select configuration #1 for the device.
    .then(() => device.claimInterface(2)) // Request exclusive control over interface #2.
    .then(() => device.controlTransferOut({
    requestType: 'class',
    recipient: 'interface',
    request: 0x22,
    value: 0x02,
    index: 0x02})) // Ready to receive data
    .then(() => {readLoop(); $('#filebrowser').jstree(true).refresh()}); // Waiting for 64 bytes of data from endpoint #5.
}

function trash_ui() {
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    console.log(node);
    file = getnodepath(node);
    console.log(file);
    if(!isfile(node)) return;
    if(confirm("Delete: " + file + "?")) {
        delfile(file)
    }
}

function sync_ui() {
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) {
        $('#filebrowser').jstree(true).refresh_node(node["id"])
    }
}

function rename_ui() {
    var filename = prompt("Please enter new filename", "");
    if (filename != null) {
        var node = $("#filebrowser").jstree("get_selected",true)[0];
        var source = getnodepath(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        } else {
            return;
        }
        file = getnodepath(node) + "/" + filename;
        movefile(source, file);
    }
}

function save_ui() {
    contents = editor.getValue();
    savetextfile(editor_filename, contents);
}

function startapp_ui() {
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    console.log(node);
    file = getnodepath(node);
    console.log(file);
    if(!isfile(node)) return;
    runfile(file);
}

function mkdir_ui() {
    var folder_name = prompt("Please enter foldername", "New folder");
    if (folder_name != null) {
        var node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        folder = getnodepath(node) + "/" + folder_name;
        console.log(folder);
        createfolder(folder);
    }   
}

function mkfile_ui() {
    var filename = prompt("Please enter filename", "New file.py");
    if (filename != null) {
        var node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        file = getnodepath(node) + "/" + filename;
        console.log(file);
        createfile(file);
    }   
}

function upload_ui() {
    $('#file-input').trigger('click');
}


var filetocopy = "";
var filetocut = "";

function copy_ui() {
    filetocut = "";
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) return;
    
    file = getnodepath(node);
    filetocopy = file;
    document.getElementById("paste_btn").disabled = false;    
}

function cut_ui() {
    filetocopy = "";
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    if(!isfile(node)) return;
    
    file = getnodepath(node);
    filetocut = file;
    document.getElementById("paste_btn").disabled = false;    
}

function paste_ui() {
    document.getElementById("paste_btn").disabled = true;
    var node = $("#filebrowser").jstree("get_selected",true)[0];
    if(isfile(node)) {
        node = $('#filebrowser').jstree(true).get_node(node["parent"]);
    }
    folder = getnodepath(node) + "/";
    console.log(folder);
    if(filetocut != "") {
        movefile(filetocut, folder);
    } else if(filetocopy != "") {
        copyfile(filetocopy, folder);
    }
}

function readSingleFile(e) {
    var file = e.target.files[0];
    if (!file) {
      return;
    }
    console.log(file);
    var reader = new FileReader();
    reader.onload = function(e) {
        var node = $("#filebrowser").jstree("get_selected",true)[0];
        console.log(node);
        if(isfile(node)) {
            node = $('#filebrowser').jstree(true).get_node(node["parent"]);
        }
        filedir = getnodepath(node) + "/" + file["name"];
        console.log(filedir);
        var arrayBuffer = reader.result;
        console.log(arrayBuffer);
        savefile(filedir, arrayBuffer);
    }
    reader.readAsArrayBuffer(file);
  }

document.getElementById('file-input')
  .addEventListener('change', readSingleFile, false);

//Init jstree
$('#filebrowser')
.on('changed.jstree', treechanged)
.jstree({
    "core" : {
        "multiple" : false,
        "data" : fetch_dir,
        "check_callback" : true
    }
});