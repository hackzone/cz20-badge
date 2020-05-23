br_len = 12;

difference() {
    union() {
cube([br_len, 6.6, 10]);
     translate([br_len, 0, 0])
rotate([0, 0, 45])   
     cube([br_len, 6.6, 10]);   
        
    }
        translate([0, 2.5, 3])

    cube([10, 1.6, 8]);
    

translate([br_len, 0, 0])
rotate([0, 0, 45])

    translate([br_len-10, 2.5, 3])
    cube([40, 1.6, 40]);
}

hull() {
    cube([br_len, 6.6, 3]);
    translate([br_len, 0, 0])
rotate([0, 0, 45])
    cube([br_len, 6.6, 3]);
}

translate([1,0,10-1.5-2])
cube([6, 2.7, 2]);

translate([br_len, 0, 0])
rotate([0, 0, 45])
translate([br_len-7, 0, 10-1.5-2])
cube([6, 2.7, 2]);