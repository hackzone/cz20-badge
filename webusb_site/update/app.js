var device;
var bytepayload;
var nextprog;
var lengthprog;
var checktimer;

let readLoop = () => {
    device.transferIn(3, 3).then(result => {
        console.log(result);
        readLoop();
  }, error => {
    console.log(error);
  });
};

function dfu_wipe(pageid) {
    device.controlTransferOut({
        requestType: 'class',
        recipient: 'interface',
        request: 0x23,
        value: pageid,
        index: 0x02});
}

function dfu_write(pageid, data) {
    console.log("Writing page")
    device.controlTransferOut({
        requestType: 'class',
        recipient: 'interface',
        request: 0x24,
        value: pageid,
        index: 0x02});
    device.transferOut(3, data);
}

function process_firmware(data) {
  console.log(data);
}

function check_status(response) {
  console.log(response);
  var data = new Uint32Array(response.data.buffer);
  var writepage = data[0];
  console.log(writepage);
  if(writepage != 0) {
    console.log("Still writing...");
  } else {
    if(lengthprog == nextprog) {
      console.log("Done programming :)");
      clearInterval(checktimer);
      return;
    }
    dfu_wipe(16+nextprog);
    dfu_write(16+nextprog, bytepayload.subarray(nextprog*1024, 1024*(nextprog+1)));
    nextprog = nextprog+1;
    
  }
}

function checkWrite() {
  device.controlTransferIn({
    requestType: 'class',
    recipient: 'interface',
    request: 0x28,
    value: 0x02,
    index: 0x02}, 4)
  .then((res) => {check_status(res)});
}

function prog_file() {
  console.log("Fetching file...")
  var oReq = new XMLHttpRequest();
  oReq.open("GET", "/update/ledblink.bin", true);
  oReq.responseType = "arraybuffer";

  oReq.onload = function (oEvent) {
    var arrayBuffer = oReq.response; // Note: not oReq.responseText
    console.log(oEvent);
    if (arrayBuffer) {
      var byteArray = new Uint8Array(arrayBuffer);
      var lenbinary = byteArray.byteLength;
      lenbinary = Math.ceil(lenbinary/1024)*1024;
      lengthprog = lenbinary/1024;
      console.log("Binary length: "+String(lenbinary));
      var progpayload = new ArrayBuffer(lenbinary);
      new Uint8Array(progpayload).set(byteArray);
      bytepayload = new Uint8Array(progpayload);
      console.log(bytepayload);
      dfu_wipe(16);
      dfu_write(16, bytepayload.subarray(0, 1024));
      nextprog = 1;
      checktimer=setInterval(checkWrite,200);
      
    }
  };
  oReq.send(null);
}

function connect() {
    navigator.usb.requestDevice({ filters: [{ vendorId: 0xcafe }] })
    .then(selectedDevice => {
    device = selectedDevice;
    return device.open(); // Begin a session.
    })
    .then(() => device.selectConfiguration(1)) // Select configuration #1 for the device.
    .then(() => device.claimInterface(2)) // Request exclusive control over interface #2.
    .then(() => device.controlTransferIn({
        requestType: 'class',
        recipient: 'interface',
        request: 0x22,
        value: 0x02,
        index: 0x02}, 8))
    .then((res) => {process_firmware(res); readLoop();}); // Waiting for 64 bytes of data from endpoint #5.
}