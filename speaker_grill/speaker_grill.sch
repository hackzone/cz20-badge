EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x09_Female J4
U 1 1 5ED3E614
P 7300 3750
F 0 "J4" H 7328 3726 50  0000 L CNN
F 1 "Conn_01x08_Female" H 7328 3635 50  0000 L CNN
F 2 "lcsc:FFC-SMD_8P-P1.00_FFC10028-08SBD224K6M" H 7300 3750 50  0001 C CNN
F 3 "~" H 7300 3750 50  0001 C CNN
	1    7300 3750
	1    0    0    -1  
$EndComp
Text GLabel 7000 4150 0    50   Input ~ 0
GND
Wire Wire Line
	7000 4150 7100 4150
Text GLabel 7000 3350 0    50   Input ~ 0
L-
Wire Wire Line
	7000 3350 7100 3350
Text GLabel 7000 3450 0    50   Input ~ 0
L+
Wire Wire Line
	7000 3450 7100 3450
Text GLabel 7000 3550 0    50   Input ~ 0
R-
Text GLabel 7000 3650 0    50   Input ~ 0
R+
Wire Wire Line
	7000 3550 7100 3550
Wire Wire Line
	7000 3650 7100 3650
Text GLabel 7000 3750 0    50   Input ~ 0
GND
Wire Wire Line
	7000 3750 7100 3750
Text GLabel 7000 3850 0    50   Input ~ 0
I2C_SCL
Wire Wire Line
	7000 3850 7100 3850
Text GLabel 7000 3950 0    50   Input ~ 0
I2C_SDA
Wire Wire Line
	7000 3950 7100 3950
Text GLabel 7000 4050 0    50   Input ~ 0
3V3
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5ED58EF4
P 5600 2450
F 0 "J2" H 5628 2426 50  0000 L CNN
F 1 "Conn_01x02_Female" H 5628 2335 50  0000 L CNN
F 2 "Connector_Molex:Molex_PicoBlade_53261-0271_1x02-1MP_P1.25mm_Horizontal" H 5600 2450 50  0001 C CNN
F 3 "https://www.molex.com/molex/products/part-detail/pcb_headers/0532610271" H 5600 2450 50  0001 C CNN
	1    5600 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J3
U 1 1 5ED594F6
P 5600 2750
F 0 "J3" H 5628 2726 50  0000 L CNN
F 1 "Conn_01x02_Female" H 5628 2635 50  0000 L CNN
F 2 "Connector_Molex:Molex_PicoBlade_53261-0271_1x02-1MP_P1.25mm_Horizontal" H 5600 2750 50  0001 C CNN
F 3 "https://www.molex.com/molex/products/part-detail/pcb_headers/0532610271" H 5600 2750 50  0001 C CNN
	1    5600 2750
	1    0    0    -1  
$EndComp
Text GLabel 5400 2450 0    50   Input ~ 0
L-
Text GLabel 5400 2550 0    50   Input ~ 0
L+
Text GLabel 5400 2750 0    50   Input ~ 0
R-
Text GLabel 5400 2850 0    50   Input ~ 0
R+
Wire Wire Line
	7000 4050 7100 4050
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J1
U 1 1 5ED7E479
P 5350 3650
F 0 "J1" H 5400 3967 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 5400 3876 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x03_P2.54mm_Vertical_SMD" H 5350 3650 50  0001 C CNN
F 3 "~" H 5350 3650 50  0001 C CNN
	1    5350 3650
	-1   0    0    -1  
$EndComp
NoConn ~ 5050 3750
NoConn ~ 5550 3750
Text GLabel 5050 3650 0    50   Input ~ 0
I2C_SCL
Text GLabel 5050 3550 0    50   Input ~ 0
GND
Text GLabel 5550 3550 2    50   Input ~ 0
3V3
Text GLabel 5550 3650 2    50   Input ~ 0
I2C_SDA
$EndSCHEMATC
