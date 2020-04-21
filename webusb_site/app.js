//Init ace editor
var editor = ace.edit("editor");
    editor.setTheme("ace/theme/monokai");
    editor.session.setMode("ace/mode/python");

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
    buffer = buildpacket(dir_name.length+1, 4097);
    for(var i = 0; i<dir_name.length; i++) {
        buffer[8+i] = dir_name.charCodeAt(i);
    }
    buffer[8+dir_name.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function createfile(dir_name) {
    buffer = buildpacket(dir_name.length+1, 4098);
    for(var i = 0; i<dir_name.length; i++) {
        buffer[8+i] = dir_name.charCodeAt(i);
    }
    buffer[8+dir_name.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

function delfile(dir_name) {
    buffer = buildpacket(dir_name.length+1, 4099);
    for(var i = 0; i<dir_name.length; i++) {
        buffer[8+i] = dir_name.charCodeAt(i);
    }
    buffer[8+dir_name.length] = 0;
    //queue.enqueue(buffer);
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
                console.log(par_id);
                if(par_id != "#") {
                    var node = $('#filebrowser').jstree(true).get_node(par_id);
                    console.log(node);
                    var folder_name = node["text"];
                    dir_name = "/" + folder_name + dir_name;
                }
            }
            console.log(dir_name);
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
                    child["icon"] = "far fa-folder";
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

//Init jstree
$('#filebrowser').on('changed.jstree', treechanged).jstree({
    'core' : {
        "multiple" : false,
        'data' : fetch_dir
    }
});