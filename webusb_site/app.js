var device;
var payload="";

function IsJsonString(str) {
try {
    JSON.parse(str);
} catch (e) {
    return false;
}
return true;
}

let readLoop = () => {
    let decoder = new TextDecoder("ascii");
      device.transferIn(3, 64).then(result => {
    console.log('Length: ' + result.data.byteLength);
    console.log('Value: ' + result.data.getUint8(0));
    payload = payload + decoder.decode(result.data);
    console.log(payload)
    if(IsJsonString(payload)) {
        console.log("Valid json found")
        $('#filebrowser').jstree(true).settings.core.data = JSON.parse(payload);
        $('#filebrowser').jstree(true).refresh();
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