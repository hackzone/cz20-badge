/**
 * Created by Tom on 5/27/2020.
 */
export let device;
let waiting_command = 1;
let command;
let size;
let received;
let payload;
let cb_reply = (obj, datastructure) => {};

import * as $ from 'jquery';

//Function to create valid packet. Size is the payload size, command is the command id
export function buildpacket(size, command) {
    let arraybuffer = new ArrayBuffer(8+size);
    let buffer = new Uint8Array(arraybuffer);
    new DataView(arraybuffer).setUint16(0, command, true);
    new DataView(arraybuffer).setUint32(2, size, true);
    buffer[6] = 0xDE;
    buffer[7] = 0xAD;
    return buffer;
}

export function buildpacketWithFilename(size, command, filename) {
    let buffer = buildpacket(filename.length+1+size, command);
    for(let i = 0; i<filename.length; i++) {
        buffer[8+i] = filename.charCodeAt(i);
    }
    buffer[8+filename.length] = 0;
    return buffer;
}

export function getnodepath(obj) {
    let dir_name = "/";
    if(obj["parent"]) { //We have parent, time to construct dir structure
        dir_name = "/" + obj["text"];
        for(const par_id of obj["parents"]) {
            console.log(par_id);
            if(par_id != "#") {
                let node = $('#filebrowser').jstree(true).get_node(par_id);
                console.log(node);
                let folder_name = node["text"];
                dir_name = "/" + folder_name + dir_name;
            }
        }
    }
    return dir_name;
}

export function isfile(obj) {
    return obj['icon'] === "far fa-file";
}

export function fetch_dir(dir_name, cb) {
    cb_reply = cb;
    console.log('Fetching', dir_name);
    if(dir_name === undefined || dir_name === '') {
        dir_name = '/';
    }
    let buffer = buildpacket(dir_name.length+1, 4096);
    for(let i = 0; i<dir_name.length; i++) {
        buffer[8+i] = dir_name.charCodeAt(i);
    }
    buffer[8+dir_name.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function readfile(dir_name, callback) {
    let buffer = buildpacketWithFilename(0, 4097, dir_name);
    console.log("Sending command...");
    cb_reply = callback;
    device.transferOut(3, buffer);
}

export function createfile(dir_name) {
    let buffer = buildpacketWithFilename(0, 4098, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function delfile(dir_name) {
    let buffer = buildpacketWithFilename(0, 4099, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function runfile(dir_name) {
    let buffer = buildpacketWithFilename(0, 0, dir_name);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function duplicatefile(source, destination) {
    let buffer = buildpacket(source.length+1+destination.length+1, 4100);
    for(let i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function movefile(source, destination) {
    let buffer = buildpacket(source.length+1+destination.length+1, 4101);
    for(let i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function copyfile(source, destination) {
    let buffer = buildpacket(source.length+1+destination.length+1, 4100);
    for(let i = 0; i<source.length; i++) {
        buffer[8+i] = source.charCodeAt(i);
    }
    buffer[8+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[8+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[8+source.length+1+destination.length] = 0;

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function savetextfile(filename, contents) {
    console.log(filename)
    let buffer = buildpacketWithFilename(contents.length, 4098, filename);
    for(let i = 0; i<contents.length; i++) {
        buffer[8+filename.length+1+i] = contents.charCodeAt(i);
    }

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function savefile(filename, contents) {
    let buffer = buildpacketWithFilename(contents.byteLength, 4098, filename);
    new Uint8Array(buffer, 8+filename.length+1, contents.byteLength).set(new Uint8Array(contents.buffer));

    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function createfolder(folder) {
    let buffer = buildpacketWithFilename(0, 4102, folder);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}

export function handlePacket(id, data) {
    let textdecoder = undefined;
    let file_contents = undefined;
    let dir_structure = undefined;
    let data_structure = undefined;
    let parent_path = undefined;

    switch(id) {
        case 4096:
            textdecoder = new TextDecoder("ascii");
            dir_structure = textdecoder.decode(data).split('\n');
            console.log(dir_structure);
            data_structure = [];
            parent_path = dir_structure[0] === '/' ? '': dir_structure[0];
            for(let i = 1; i < dir_structure.length; i++) {
                let is_dir = dir_structure[i].charAt(0) === "d";
                let child = {};
                child["text"] = dir_structure[i].substr(1);
                child["full_path"] = parent_path + '/' + child["text"];
                if(is_dir) {
                    if(dir_structure[i] === "dflash") {
                        child["icon"] = "fas fa-microchip";
                    } else if(dir_structure[i] === "dsdcard") {
                        child["icon"] = "fas fa-sd-card";
                    } else {
                        child["icon"] = "far fa-folder";
                    }
                    child["is_dir"] = true;
                } else {
                    child["icon"] = "far fa-file";
                }
                child["opened"] = false;
                child["disabled"] = false;
                child["selected"] = false;
                if(is_dir) {
                    child["children"] = [{text:'Click parent to refresh', icon: 'none'}];
                }
                data_structure.push(child);
            }
            cb_reply.call(this, data_structure);
            break;
        case 4097:
            textdecoder = new TextDecoder("ascii");
            file_contents = textdecoder.decode(data);
            console.log(file_contents);
            cb_reply.call(this, file_contents);
            break;
        default :
            textdecoder = new TextDecoder("ascii");
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
            let verif = result.data.getUint16(6);
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

export function connect() {
    waiting_command = 1;
    command = 0;
    size = 0;
    received = 0;
    payload = 0;
    return navigator.usb.requestDevice({ filters: [{ vendorId: 0xcafe }] })
        .then(selectedDevice => {
            device = selectedDevice;
            window.dev = selectedDevice;
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
        .then(() => {
            readLoop();
            // $('#filebrowser').jstree(true).refresh();
        }); // Waiting for 64 bytes of data from endpoint #5.
}