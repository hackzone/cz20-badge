var device;
var queue = new Queue();
var waiting_command = 1;
var command;
var size;
var received;
var payload;

function handlePacket(id, data) {
    switch(id) {
        case 1:
            var textdecoder = new TextDecoder("ascii");
            json_data = textdecoder.decode(data);
            try {
                json_data = JSON.parse(json_data);
                $('#filebrowser').jstree(true).settings.core.data = json_data;
                $('#filebrowser').jstree(true).refresh();
            } catch (e) {
                console.log("Error in file directory json.");
            }
        break;
    }
}


let readLoop = () => {
    device.transferIn(3, 64).then(result => {
        if(waiting_command) {
            waiting_command = 0;
            command = result.data.getUint16(0, true);
            size = result.data.getUint32(2, true);
            var verif = result.data.getUint16(6);
            received = result.data.byteLength-8;
            console.log("Verif: "+verif);
            payload = new ArrayBuffer(size);
            console.log(new Uint8Array(result.data.buffer)[0]);
            new Uint8Array(payload, 0, size).set(new Uint8Array(result.data.buffer, 8));
        } else {
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
    .then(() => {readLoop()}); // Waiting for 64 bytes of data from endpoint #5.
}

function refreshFiles() {
    var buffer = new Uint8Array(8);
    buffer[0] = 0x01;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0xDE;
    buffer[7] = 0xAD;
    //queue.enqueue(buffer);
    console.log("Sending command...");
    device.transferOut(3, buffer);
}