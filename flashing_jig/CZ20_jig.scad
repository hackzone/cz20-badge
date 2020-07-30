// Note: X and Y are swapped in this design
// due to OpenSCAD not supporting negative object dimensions.

$fn=50;
offset = [15, 25, 0];

testpoints = [
    [28.95, 106.9, 0],  // VCC
    [109.3, 120.2, 0],  // 5V
    [118.8, 116.3, 0],  // GND
    [45.32, 105.92, 0], // SWCLK
    [53.15, 113.56, 0],  // SWDIO
    [77.5, 53, 0], // R+
    [77.5, 49.5, 0], // R-
    [81, 49.5, 0], // L+
    [81, 53, 0], // L-
    [50.4, 69.325, 0], // ESP TX
    [49.05, 72.6, 0], // ESP RX
    [53.594, 72.898, 0], // ESP FLASH
    [29.1, 87.6, 0], // ESP ENABLE
];

align_pins = [
    [27.5, 27.5, 0],
    [122.5, 27.5, 0],
    [27.5, 122.5, 0],
    [122.5, 122.5, 0]
];

probe_inner_dia = 1.4;
probe_outer_dia = 3.0;
probe_height = 11;
align_pin_inner_dia = 3.0;
align_pin_outer_dia = 5.0;
align_pin_height = 15.5; // 1mm shorter than the 16.5mm (uncompressed) probes
base = [110, 100, 1.0];

difference() {
    union() {   
        // base
        cube(base);
        
        // probe pin hulls
        for(testpoint=testpoints) {
            translate(testpoint - offset)
            cylinder(d1=probe_outer_dia*2, d2=probe_outer_dia, h=probe_height);
        }
        
        // Alignment pins
        for(align_pin=align_pins) {
            translate(align_pin - offset)
            cylinder(d=align_pin_outer_dia, h=align_pin_height);
            
            translate(align_pin - offset + [0, 0, align_pin_height])
            cylinder(d1=align_pin_inner_dia, d2=align_pin_inner_dia/2, h=2);
        }
    }
    
    // probe pin center holes
    for(testpoint=testpoints) {
        translate(testpoint - offset + [0, 0, 0.2])
        cylinder(d=probe_inner_dia, h=probe_height);
    }
}