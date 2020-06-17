br_len = 11.3;
wall_thickness = 1.5;
pcb_thickness = 1.75;
notch_thickness = 0.4;
notch_offset_x = 0.4;
notch_offset_y = 0.1;

difference() {
    union() {
        cube([br_len, pcb_thickness+2*wall_thickness, 8]);
        translate([br_len, 0, 0])
        rotate([0, 0, 45])   
        cube([br_len, pcb_thickness+2*wall_thickness, 8]);     
    }
    translate([0, wall_thickness, 1])
    cube([10, pcb_thickness, 8]);
    

    translate([br_len, 0, 0])
    rotate([0, 0, 45])

    translate([br_len-10, wall_thickness, 1])
    cube([10, pcb_thickness, 8]);
}

//// Base plate
//hull() {
//    cube([br_len, 6.6, 3]);
//    
//    translate([br_len, 0, 0])
//    rotate([0, 0, 45])
//    cube([br_len, 6.6, 3]);
//}

// Notches
translate([1+notch_offset_x,0,8-wall_thickness-2+notch_offset_y])
cube([6, wall_thickness+notch_thickness, 2]);

translate([br_len, 0, 0])
rotate([0, 0, 45])
translate([br_len-7-notch_offset_x, 0, 8-wall_thickness-2+notch_offset_y])
cube([6, wall_thickness+notch_thickness, 2]);