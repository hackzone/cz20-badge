br_len = 11.3;
pcb_thickness = 1.65;

difference() {
    union() {
        cube([br_len, pcb_thickness+2*1.5, 8]);
        translate([br_len, 0, 0])
        rotate([0, 0, 45])   
        cube([br_len, pcb_thickness+2*1.5, 8]);     
    }
    translate([0, 1.5, 1])
    cube([10, pcb_thickness, 8]);
    

    translate([br_len, 0, 0])
    rotate([0, 0, 45])

    translate([br_len-10, 1.5, 1])
    cube([10, pcb_thickness, 8]);
}

//hull() {
//    cube([br_len, 6.6, 3]);
//    
//    translate([br_len, 0, 0])
//    rotate([0, 0, 45])
//    cube([br_len, 6.6, 3]);
//}

translate([1,0,8-1.5-2])
cube([6, pcb_thickness, 2]);

translate([br_len, 0, 0])
rotate([0, 0, 45])
translate([br_len-7, 0, 8-1.5-2])
cube([6, pcb_thickness, 2]);