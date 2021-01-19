/**
 * Created by Tom on 5/27/2020.
 */
export let device = {opened: false};
let stdout_callback, port, reader, writer;

let MAX_RETRIES = 3;

import * as $ from 'jquery';
import * as JSZip from 'jszip';
import {encode} from 'uint8-to-base64';

class PyShellTransformer {
    constructor() {
      this.container = '';
    }
  
    transform(chunk, controller) {
        if (stdout_callback !== undefined) {
            stdout_callback(chunk);
        }
        this.container += chunk;
        const results = this.container.split('>>> ');
        this.container = results.pop();
        results.forEach(result => controller.enqueue(result));
    }
  
    flush(controller) {
      controller.enqueue(this.container);
    }
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

function strip_flash(path) {
    return path.startsWith('/flash') ? path.slice('/flash'.length) : path;
}

async function transceive(data, add_newlines=true) {
    if(typeof(data) != 'string') {
        console.debug('Can\'t call transceive on non-text data');
        return;
    }
    if (add_newlines && !data.endsWith('\r\n')) {
        data = data.trimEnd() + '\r\n';
    }
    await writer.write(data);
    let {value, done} = await reader.read();

    if (value.indexOf(data) == 0) {
        // Strip echoed command from answer
        value = value.slice(data.length); // Includes the newline characters after the echoed command
    }
    return value;
}

async function reset() {
    // Toggle badge reset pin
    await port.setSignals({ requestToSend: true, dataTerminalReady: false });
    await sleep(100);
    await port.setSignals({ requestToSend: false, dataTerminalReady: false });
    
    // Wait for the firmware to start
    await sleep(1000);
    
    // Enter the serial menu's Python Shell option
    await transceive('\r\n');
}

export async function fetch_dir(dir_name) {
    dir_name = strip_flash(dir_name);
    console.log('Fetching', dir_name);
    if (dir_name === undefined || dir_name === '') {
        dir_name = '/';
    }
    let answer = await transceive(`from upysh import ls; ls('${dir_name}')`);
    let lines = answer.trimEnd().split('\n');
    let result = [dir_name !== '' ? dir_name : '/'];

    for (let line of lines) {
        line = line.trim();
        if (line === '') {
            continue;
        }
        let is_dir = line.startsWith('<dir>');
        result.push((is_dir ? 'd' : 'f') + line.split(' ')[1]);
    }

    return result.join('\n');
}

export async function readfile(file_name, return_string=true) {
    file_name = strip_flash(file_name);
    let contents = await transceive(`from upysh import cat; cat('${file_name}')`);
    return contents.replaceAll('\r\n', '\n')
}

export async function createfile(file_name) {
    file_name = strip_flash(file_name);
    return await transceive(`from upysh import newfile; newfile('${file_name}')`);
}

export async function deldir(dir_name) {
    dir_name = strip_flash(dir_name);
    let dir = await fetch_dir(dir_name)
    let dirlist = dir.split('\n');
    dirlist.unshift();
    console.log(dirlist);
    for(let i = 1; i < dirlist.length; i++) {
        let item = dirlist[i];
        if(item.charAt(0) == 'd') {
            await deldir(dir_name + "/" + item.substr(1));
        } else {
            await delfile(dir_name + "/" + item.substr(1));
        }
    }
    await delfile(dir_name);
}

export async function downloaddir(dir_name, zip=undefined) {
    if(zip === undefined) {
        zip = new JSZip();
    }

    dir_name = strip_flash(dir_name);

    let dir = await fetch_dir(dir_name)
    let dirlist = dir.split('\n');
    dirlist.unshift();
    console.log(dirlist);
    for(let i = 1; i < dirlist.length; i++) {
        let item = dirlist[i];
        if(item.charAt(0) == 'd') {
            await downloaddir(dir_name + "/" + item.substr(1), zip.folder(item.substr(1)));
        } else {
            let data = await readfile(dir_name + "/" + item.substr(1));
            zip.file(item.substr(1), data);
        }
    }
    return zip;
}

export function delfile(file_name) {
    file_name = strip_flash(file_name);
    return transceive(`from upysh import rm; rm('${file_name}')`);
}

export function runfile(file_path) {
    file_path = strip_flash(file_path);
    if(file_path.startsWith('/flash')) {
        file_path = file_path.slice('/flash'.length);
    }
    return transceive(`__import__('${file_path}')`);
}

export function duplicatefile(source, destination) {
    source = strip_flash(source);
    destination = strip_flash(destination);
    console.error('duplicatefile is not yet implemented');
}

export function movefile(source, destination) {
    source = strip_flash(source);
    destination = strip_flash(destination);
    return transceive(`from upysh import mv; mv('${source}', '${destination}')`);
}

export function copyfile(source, destination) {
    source = strip_flash(source);
    destination = strip_flash(destination);
    return transceive(`from upysh import cp; cp('${source}', '${destination}')`);
}

export function savetextfile(filename, contents) {
    filename = strip_flash(filename);
    let escaped = contents.replaceAll('\r', '\\r').replaceAll('\n', '\\n').replaceAll("'", "\\'");
    return transceive(`\x05f=open('${filename}', 'wt')\r\nf.write('${escaped}')\r\nf.close()\x04`, false);
}

export function savefile(filename, contents) {
    filename = strip_flash(filename);
    let data = new Uint8Array(contents);
    let base64 = encode(data);
    return transceive(`\x05import binascii\r\nf=open('${filename}', 'wb')\r\nf.write(binascii.a2b_base64('${base64}'))\r\nf.close()\x04`, false);
}

export function createfolder(folder) {
    folder = strip_flash(folder);
    return transceive(`from upysh import mkdir; mkdir('${folder}')`);
}

export function registerstdout(func) {
    stdout_callback = func;
}

export function writetostdin(input) {
    return transceive(input, false);
}

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


export async function connect() {
    port = await navigator.serial.requestPort();
    await port.open({baudRate: 115200});

    reader = port.readable.pipeThrough(new TextDecoderStream()).pipeThrough(new TransformStream(new PyShellTransformer())).getReader();
    
    let encoder = new TextEncoderStream();
    let outputDone = encoder.readable.pipeTo(port.writable);
    let outputStream = encoder.writable;
    writer = outputStream.getWriter();

    window.reader = reader;

    await reset();
    device.opened = true;
}

window.transceive = transceive;