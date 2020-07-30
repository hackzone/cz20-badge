var device;
var bytepayload;
var nextprog;
var lengthprog;
var checktimer;
var application;
var verifactive = 0;

function concatTypedArrays(a, b) { // a, b TypedArray of same type
  var c = new (a.constructor)(a.length + b.length);
  c.set(a, 0);
  c.set(b, a.length);
  return c;
}

function compareTypedArrays(a, b) {
  if(a.length != b.length) return false;
  for(var i=0; i<a.length; i++) {
    if(a[i] != b[i]) return false
  }
  return true;
}

function prog_success() {
  window.alert("Program success");
  dfu_setversion(1);
  dfu_restart();
}

function prog_failed() {
  window.alert("Programming failed");
}

let readLoop = () => {
    device.transferIn(3, 64).then(result => {
        console.log(result);
        if(verifactive) {
          application = concatTypedArrays(application, new Uint8Array(result.data.buffer));        
          if((application.byteLength % 1024) == 0) {
            if(nextprog == lengthprog) {
              verifactive = 0;
              if(compareTypedArrays(application, bytepayload)) {
                prog_success();
              } else {
                prog_failed();
              }
            } else {
              dfu_read(16+nextprog);
              nextprog = nextprog+1;
            }
          }
          

        }
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

function dfu_read(pageid) {
  console.log("Reading page: "+String(pageid))
  device.controlTransferOut({
    requestType: 'class',
    recipient: 'interface',
    request: 0x25,
    value: pageid,
    index: 0x02});
}

function dfu_setversion(version) {
  device.controlTransferOut({
    requestType: 'class',
    recipient: 'interface',
    request: 0x27,
    value: version,
    index: 0x02});
}

function dfu_restart() {
  device.controlTransferOut({
    requestType: 'class',
    recipient: 'interface',
    request: 0x26,
    value: 0,
    index: 0x02});
}

function process_firmware(data) {
  console.log(data);
}

function verify_flash() {
  application = new Uint8Array(0);
  nextprog = 1;
  verifactive = 1;
  dfu_read(16);
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
      window.alert("Done writing :)");
      clearInterval(checktimer);
      verify_flash();      
      return;
    }
    dfu_wipe(16+nextprog);
    dfu_write(16+nextprog, bytepayload.subarray(nextprog*1024, 1024*(nextprog+1)));
    nextprog = nextprog+1;
    
  }
}

function prog_version(version_number) {
  device.controlTransferOut({
    requestType: 'class',
    recipient: 'interface',
    request: 0x27,
    value: version_number,
    index: 0x02});
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
  oReq.open("GET", "firmware-STM32.bin", true);
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
