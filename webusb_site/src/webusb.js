/**
 * Created by Tom on 5/27/2020.
 */
export let device;
let waiting_command = 1;
let command;
let size;
let received;
let payload;
let messageid_recv;
let cb_reply_read = [];
let cb_reply_dir = [];
const packetheadersize = 12;

let current_message_id = 0;
let requests = {};

let MAX_RETRIES = 3;

import * as $ from 'jquery';

//Function to create valid packet. Size is the payload size, command is the command id
export function buildpacket(size, command) {
    console.log(packetheadersize+size);
    current_message_id++;
    let arraybuffer = new ArrayBuffer(12+size);
    let buffer = new Uint8Array(arraybuffer);
    new DataView(arraybuffer).setUint16(0, command, true);
    new DataView(arraybuffer).setUint32(2, size, true);
    new DataView(arraybuffer).setUint32(8, current_message_id, true);
    buffer[6] = 0xDE;
    buffer[7] = 0xAD;
    return {buffer: buffer, message_id: current_message_id};
}

export function buildpacketWithFilename(size, command, filename) {
    let {buffer, message_id} = buildpacket(filename.length+1+size, command, message_id);
    for(let i = 0; i<filename.length; i++) {
        buffer[packetheadersize+i] = filename.charCodeAt(i);
    }
    buffer[packetheadersize+filename.length] = 0;
    return {buffer, message_id: message_id};
}

export function send_buffer(buffer, message_id, return_string=true) {
    let resolve, reject;
    let promise = new Promise((_resolve, _reject) => {resolve = _resolve; reject = _reject;});
    let request = {
        buffer, // For retransmit
        resolve: (data) => {
            if(return_string) {
                let textdecoder = new TextDecoder("ascii");
                resolve(textdecoder.decode(data));
            } else {
                resolve(data);
            }
        },
        reject: (reason, immediate_reject=false) => {
            if(!immediate_reject && request.retries <= MAX_RETRIES) {
                request.retries++;
                device.transferOut(3, buffer);
            } else {
                reject(reason);
            }
        },
        retries: 0 // Number of times this request has been retransmitted already
    };
    requests[message_id] = request;
    device.transferOut(3, buffer);
    return promise;
}

export function fetch_dir(dir_name, cb) {
    cb_reply_dir.push(cb);
    console.log('Fetching', dir_name);
    if(dir_name === undefined || dir_name === '') {
        dir_name = '/';
    }
    let {buffer, message_id} = buildpacket(dir_name.length+1, 4096);
    for(let i = 0; i<dir_name.length; i++) {
        buffer[packetheadersize+i] = dir_name.charCodeAt(i);
    }
    buffer[packetheadersize+dir_name.length] = 0;
    return send_buffer(buffer, message_id, false);
}

export function readfile(dir_name, callback, return_string=true) {
    let {buffer, message_id} = buildpacketWithFilename(0, 4097, dir_name);
    return send_buffer(buffer, message_id, return_string);
}

export function createfile(dir_name) {
    let {buffer, message_id} = buildpacketWithFilename(0, 4098, dir_name);
    return send_buffer(buffer, message_id);
}

export function delfile(dir_name) {
    let {buffer, message_id} = buildpacketWithFilename(0, 4099, dir_name);
    return send_buffer(buffer, message_id);
}

export function runfile(dir_name) {
    let {buffer, message_id} = buildpacketWithFilename(0, 0, dir_name);
    return send_buffer(buffer, message_id);
}

export function duplicatefile(source, destination) {
    let {buffer, message_id} = buildpacket(source.length+1+destination.length+1, 4100);
    for(let i = 0; i<source.length; i++) {
        buffer[packetheadersize+i] = source.charCodeAt(i);
    }
    buffer[packetheadersize+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[packetheadersize+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[packetheadersize+source.length+1+destination.length] = 0;
    //queue.enqueue(buffer);
    return send_buffer(buffer, message_id);
}

export function movefile(source, destination) {
    let {buffer, message_id} = buildpacket(source.length+1+destination.length+1, 4101);
    for(let i = 0; i<source.length; i++) {
        buffer[packetheadersize+i] = source.charCodeAt(i);
    }
    buffer[packetheadersize+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[packetheadersize+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[packetheadersize+source.length+1+destination.length] = 0;

    return send_buffer(buffer, message_id);
}

export function copyfile(source, destination) {
    let {buffer, message_id} = buildpacket(source.length+1+destination.length+1, 4100);
    for(let i = 0; i<source.length; i++) {
        buffer[packetheadersize+i] = source.charCodeAt(i);
    }
    buffer[packetheadersize+source.length] = 0;

    for(let i = 0; i<destination.length; i++) {
        buffer[packetheadersize+source.length+1+i] = destination.charCodeAt(i);
    }
    buffer[packetheadersize+source.length+1+destination.length] = 0;

    return send_buffer(buffer, message_id);
}

export function savetextfile(filename, contents) {
    console.log(filename);
    let {buffer, message_id} = buildpacketWithFilename(contents.length, 4098, filename);
    for(let i = 0; i<contents.length; i++) {
        buffer[packetheadersize+filename.length+1+i] = contents.charCodeAt(i);
    }

    return send_buffer(buffer, message_id);
}

export function savefile(filename, contents) {
    let {buffer, message_id} = buildpacketWithFilename(contents.byteLength, 4098, filename);
    let uint8 = new Uint8Array(buffer);
    uint8.set(new Uint8Array(contents), packetheadersize+filename.length+1);
    buffer = uint8.buffer;
    return send_buffer(buffer, message_id);
}

export function createfolder(folder) {
    let {buffer, message_id} = buildpacketWithFilename(0, 4102, folder);
    return send_buffer(buffer, message_id);
}

export function handlePacket(message_type, message_id, data) {
    let textdecoder = undefined;
    let file_contents = undefined;
    // let dir_structure = undefined;
    // let data_structure = undefined;
    // let parent_path = undefined;

    // case 4096:
    //     textdecoder = new TextDecoder("ascii");
    //     dir_structure = textdecoder.decode(data).split('\n');
    //     console.log(dir_structure);
    //     data_structure = [];
    //     parent_path = dir_structure[0] === '/' ? '': dir_structure[0];
    //     for(let i = 1; i < dir_structure.length; i++) {
    //         let is_dir = dir_structure[i].charAt(0) === "d";
    //         let child = {};
    //         child["text"] = dir_structure[i].substr(1);
    //         child["full_path"] = parent_path + '/' + child["text"];
    //         if(is_dir) {
    //             if(dir_structure[i] === "dflash") {
    //                 child["icon"] = "fas fa-microchip";
    //             } else if(dir_structure[i] === "dsdcard") {
    //                 child["icon"] = "fas fa-sd-card";
    //             } else {
    //                 child["icon"] = "far fa-folder";
    //             }
    //             child["is_dir"] = true;
    //             child["dragDisabled"] = true;
    //         } else {
    //             child["icon"] = "far fa-file";
    //         }
    //         child["opened"] = false;
    //         child["disabled"] = false;
    //         child["selected"] = false;
    //         if(is_dir) {
    //             child["children"] = [{text:'Click parent to refresh', icon: 'none', isDummy: true}];
    //         }
    //         data_structure.push(child);
    //     }
    //     if(cb_reply_dir[0]) {
    //         cb_reply_dir[0].call(this, data_structure);
    //     }
    //     cb_reply_dir.shift();
    //     break;
    // case 4097:
    //     textdecoder = new TextDecoder("ascii");
    //     file_contents = textdecoder.decode(data);
    //     console.log(data);
    //     if(cb_reply_read[0]) {
    //         cb_reply_read[0].call(this, file_contents);
    //     }
    //     cb_reply_read.shift();
    //     break;

    if (message_type === 1 && message_id === 0) {
        textdecoder = new TextDecoder("ascii");
        file_contents = textdecoder.decode(data);
        file_contents = file_contents.substring(0, 2);
        if (file_contents === "to") {
            for(let key in requests) {
                let request = requests[key];
                request.reject('Timeout');
            }
        } else if (file_contents === "te") {
            for(let key in requests) {
                let request = requests[key];
                request.reject('Timeout');
            }
        }
        return;
    }

    if(message_id in requests) {
        let request = requests[message_id];
        if(data.byteLength === 3) {
            let textdecoder = new TextDecoder("ascii");
            let text_content = textdecoder.decode(data.slice(0, 2));
            if(text_content === 'er') {
                request.reject('Unspecified error');
            } else {
                request.resolve(data, true);
            }
        } else {
            request.resolve(data);
        }
    }
}


let readLoop = () => {
    device.transferIn(3, 64).then(result => {
        if(waiting_command) {
            waiting_command = 0;
            command = result.data.getUint16(0, true);
            size = result.data.getUint32(2, true);
            messageid_recv = result.data.getUint32(8, true);
            console.log("Trans: "+size);
            console.log("Packetsize: "+result.data.byteLength);
            let verif = result.data.getUint16(6);            
            received = result.data.byteLength-packetheadersize;
            console.log("Verif: "+verif);
            payload = new ArrayBuffer(size);
            new Uint8Array(payload, 0, size).set(new Uint8Array(result.data.buffer, packetheadersize, Math.min(size, received)));
        } else {
            console.log(received + " " + size + " " + result.data.byteLength);
            new Uint8Array(payload, received, size-received).set(new Uint8Array(result.data.buffer, 0, Math.min(size-received, result.data.byteLength)));
            received = received + Math.min(size-received, result.data.byteLength);
            console.log("rec: "+received+"/"+size);
        }
        if(received === size) {
            console.log("transfer complete");
            waiting_command = 1;
            handlePacket(command, messageid_recv, payload);
        }
        readLoop();
    }, error => {
        this.onReceiveError(error);
    });
};

let connect_resolves = [];
function connect_check() {
    if(device !== undefined && device.opened) {
        for(let resolve of connect_resolves) {
            resolve();
        }
        connect_resolves = [];
    }
}
setInterval(connect_check, 500);

export function on_connect() {
    return new Promise((resolve) => connect_resolves.push(resolve));
}

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
        });
}