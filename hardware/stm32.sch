EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
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
L MCU_ST_STM32F1:STM32F103C8Tx U7
U 1 1 5E3BAF1D
P 5200 3700
F 0 "U7" H 5150 2111 50  0000 C CNN
F 1 "STM32F103C8Tx" H 5150 2020 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 4600 2300 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00161566.pdf" H 5200 3700 50  0001 C CNN
F 4 "C8734" H 5200 3700 50  0001 C CNN "LCSC"
	1    5200 3700
	1    0    0    -1  
$EndComp
Text GLabel 5000 5550 3    50   UnSpc ~ 0
GND
Text GLabel 5000 2050 1    50   UnSpc ~ 0
3V3
Wire Wire Line
	5000 2050 5000 2200
Wire Wire Line
	5000 2200 5100 2200
Connection ~ 5000 2200
Connection ~ 5100 2200
Wire Wire Line
	5100 2200 5200 2200
Connection ~ 5200 2200
Wire Wire Line
	5200 2200 5300 2200
Connection ~ 5300 2200
Wire Wire Line
	5300 2200 5400 2200
Wire Wire Line
	5000 5550 5000 5200
Wire Wire Line
	5300 5200 5200 5200
Connection ~ 5000 5200
Connection ~ 5100 5200
Wire Wire Line
	5100 5200 5000 5200
Connection ~ 5200 5200
Wire Wire Line
	5200 5200 5100 5200
Text GLabel 6250 2150 1    50   UnSpc ~ 0
3V3
$Comp
L Device:C C?
U 1 1 5E3D7D7C
P 6250 2400
AR Path="/5DC753C3/5E3D7D7C" Ref="C?"  Part="1" 
AR Path="/5E3D7D7C" Ref="C?"  Part="1" 
AR Path="/5E3BA64A/5E3D7D7C" Ref="C19"  Part="1" 
F 0 "C19" H 6365 2446 50  0000 L CNN
F 1 "100nF" H 6365 2355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6288 2250 50  0001 C CNN
F 3 "~" H 6250 2400 50  0001 C CNN
F 4 "C14663" H 6250 2400 50  0001 C CNN "LCSC"
	1    6250 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E3D85DB
P 6700 2400
AR Path="/5DC753C3/5E3D85DB" Ref="C?"  Part="1" 
AR Path="/5E3D85DB" Ref="C?"  Part="1" 
AR Path="/5E3BA64A/5E3D85DB" Ref="C20"  Part="1" 
F 0 "C20" H 6815 2446 50  0000 L CNN
F 1 "100nF" H 6815 2355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6738 2250 50  0001 C CNN
F 3 "~" H 6700 2400 50  0001 C CNN
F 4 "C14663" H 6700 2400 50  0001 C CNN "LCSC"
	1    6700 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E3D8A4A
P 7150 2400
AR Path="/5DC753C3/5E3D8A4A" Ref="C?"  Part="1" 
AR Path="/5E3D8A4A" Ref="C?"  Part="1" 
AR Path="/5E3BA64A/5E3D8A4A" Ref="C21"  Part="1" 
F 0 "C21" H 7265 2446 50  0000 L CNN
F 1 "100nF" H 7265 2355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7188 2250 50  0001 C CNN
F 3 "~" H 7150 2400 50  0001 C CNN
F 4 "C14663" H 7150 2400 50  0001 C CNN "LCSC"
	1    7150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 2150 6250 2250
Wire Wire Line
	7150 2250 6700 2250
Connection ~ 6250 2250
Connection ~ 6700 2250
Wire Wire Line
	6700 2250 6250 2250
Text GLabel 6250 2650 3    50   UnSpc ~ 0
GND
Wire Wire Line
	6250 2650 6250 2550
Wire Wire Line
	7150 2550 6700 2550
Connection ~ 6250 2550
Connection ~ 6700 2550
Wire Wire Line
	6700 2550 6250 2550
$Comp
L Device:R R?
U 1 1 5E3DFB2E
P 4200 2250
AR Path="/5DC753C3/5E3DFB2E" Ref="R?"  Part="1" 
AR Path="/5E3DFB2E" Ref="R?"  Part="1" 
AR Path="/5E3BA64A/5E3DFB2E" Ref="R31"  Part="1" 
F 0 "R31" V 3993 2250 50  0000 C CNN
F 1 "10k" V 4084 2250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4130 2250 50  0001 C CNN
F 3 "~" H 4200 2250 50  0001 C CNN
F 4 "C25804" H 4200 2250 50  0001 C CNN "LCSC"
	1    4200 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2400 4200 2400
Text GLabel 4200 2000 1    50   UnSpc ~ 0
3V3
Wire Wire Line
	4200 2000 4200 2100
$Comp
L Connector:TestPoint TP2
U 1 1 5E3E2E0D
P 3750 2350
F 0 "TP2" H 3808 2468 50  0000 L CNN
F 1 "Nrst" H 3808 2377 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3950 2350 50  0001 C CNN
F 3 "~" H 3950 2350 50  0001 C CNN
	1    3750 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2350 3750 2400
Wire Wire Line
	3750 2400 4200 2400
Connection ~ 4200 2400
Text GLabel 3250 2750 3    50   UnSpc ~ 0
GND
Wire Wire Line
	4100 2750 4500 2750
Wire Wire Line
	4500 2750 4500 2800
Wire Wire Line
	4500 2900 4500 2950
Wire Wire Line
	4500 2950 4100 2950
$Comp
L Device:C C?
U 1 1 5E3EFF6B
P 3800 2700
AR Path="/5DC753C3/5E3EFF6B" Ref="C?"  Part="1" 
AR Path="/5E3EFF6B" Ref="C?"  Part="1" 
AR Path="/5E3BA64A/5E3EFF6B" Ref="C1"  Part="1" 
F 0 "C1" H 3915 2746 50  0000 L CNN
F 1 "20pF" H 3915 2655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3838 2550 50  0001 C CNN
F 3 "~" H 3800 2700 50  0001 C CNN
F 4 "C1648" H 3800 2700 50  0001 C CNN "LCSC"
	1    3800 2700
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E3F1F93
P 3800 3000
AR Path="/5DC753C3/5E3F1F93" Ref="C?"  Part="1" 
AR Path="/5E3F1F93" Ref="C?"  Part="1" 
AR Path="/5E3BA64A/5E3F1F93" Ref="C18"  Part="1" 
F 0 "C18" H 3915 3046 50  0000 L CNN
F 1 "20pF" H 3915 2955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3838 2850 50  0001 C CNN
F 3 "~" H 3800 3000 50  0001 C CNN
F 4 "C1648" H 3800 3000 50  0001 C CNN "LCSC"
	1    3800 3000
	0    1    1    0   
$EndComp
Text GLabel 3550 3000 3    50   UnSpc ~ 0
GND
Wire Wire Line
	3650 3000 3550 3000
Wire Wire Line
	3650 2700 3550 2700
Wire Wire Line
	3550 2700 3550 2850
$Comp
L Device:R R?
U 1 1 5E3F742A
P 3650 3350
AR Path="/5DC753C3/5E3F742A" Ref="R?"  Part="1" 
AR Path="/5E3F742A" Ref="R?"  Part="1" 
AR Path="/5E3BA64A/5E3F742A" Ref="R25"  Part="1" 
F 0 "R25" V 3550 3700 50  0000 C CNN
F 1 "10k" V 3534 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3580 3350 50  0001 C CNN
F 3 "~" H 3650 3350 50  0001 C CNN
F 4 "C25804" H 3650 3350 50  0001 C CNN "LCSC"
	1    3650 3350
	0    1    -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E3F7F45
P 3650 3450
AR Path="/5DC753C3/5E3F7F45" Ref="R?"  Part="1" 
AR Path="/5E3F7F45" Ref="R?"  Part="1" 
AR Path="/5E3BA64A/5E3F7F45" Ref="R27"  Part="1" 
F 0 "R27" V 3550 3800 50  0000 C CNN
F 1 "10k" V 3534 3450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3580 3450 50  0001 C CNN
F 3 "~" H 3650 3450 50  0001 C CNN
F 4 "C25804" H 3650 3450 50  0001 C CNN "LCSC"
	1    3650 3450
	0    1    1    0   
$EndComp
Text GLabel 3250 3400 0    50   UnSpc ~ 0
3V3
Wire Wire Line
	4500 3100 3800 3100
Wire Wire Line
	3800 3100 3800 3350
Wire Wire Line
	3900 3200 4500 3200
Wire Wire Line
	3250 3100 3800 3100
Connection ~ 3800 3100
Wire Wire Line
	3250 3200 3900 3200
Connection ~ 3900 3200
Text HLabel 6500 4700 2    50   Input ~ 0
D+
Text HLabel 6500 4600 2    50   Input ~ 0
D-
Wire Wire Line
	6500 4600 5800 4600
Wire Wire Line
	5800 4700 6400 4700
$Comp
L Device:R R?
U 1 1 5E4109CF
P 6400 4400
AR Path="/5DC753C3/5E4109CF" Ref="R?"  Part="1" 
AR Path="/5E4109CF" Ref="R?"  Part="1" 
AR Path="/5E3BA64A/5E4109CF" Ref="R32"  Part="1" 
F 0 "R32" V 6193 4400 50  0000 C CNN
F 1 "10k" V 6284 4400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6330 4400 50  0001 C CNN
F 3 "~" H 6400 4400 50  0001 C CNN
F 4 "C25804" H 6400 4400 50  0001 C CNN "LCSC"
	1    6400 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4550 6400 4700
Connection ~ 6400 4700
Wire Wire Line
	6400 4700 6500 4700
Text GLabel 6400 4150 1    50   UnSpc ~ 0
3V3
Wire Wire Line
	6400 4150 6400 4250
Text HLabel 4400 4100 0    50   Input ~ 0
i2c_SCL
Wire Wire Line
	4400 4100 4500 4100
Text HLabel 5900 3700 2    50   Output ~ 0
TX
Wire Wire Line
	5900 3700 5800 3700
Text HLabel 5900 3800 2    50   Input ~ 0
RX
Wire Wire Line
	5900 3800 5800 3800
Text HLabel 5900 4400 2    50   Output ~ 0
TX2
Wire Wire Line
	5900 4400 5800 4400
Text HLabel 5900 4500 2    50   Input ~ 0
RX2
Wire Wire Line
	5900 4500 5800 4500
Text HLabel 5900 3500 2    50   Input ~ 0
ROW1
Text HLabel 5900 3900 2    50   Input ~ 0
COL1
Wire Wire Line
	5900 3900 5800 3900
Text HLabel 5900 4000 2    50   Input ~ 0
COL2
Text HLabel 5900 4100 2    50   Input ~ 0
COL3
Text HLabel 5900 4200 2    50   Input ~ 0
COL4
Wire Wire Line
	5900 4000 5800 4000
Wire Wire Line
	5900 4100 5800 4100
Wire Wire Line
	5900 4200 5800 4200
Wire Wire Line
	5900 3500 5800 3500
Text HLabel 5900 3600 2    50   Input ~ 0
ROW2
Wire Wire Line
	5900 3600 5800 3600
Text HLabel 5900 4300 2    50   Input ~ 0
ROW3
Wire Wire Line
	5900 4300 5800 4300
Text HLabel 5900 5000 2    50   Input ~ 0
ROW4
Wire Wire Line
	5900 5000 5800 5000
$Comp
L Device:Crystal_GND24_Small Y1
U 1 1 5E3ED918
P 4100 2850
F 0 "Y1" V 4054 2994 50  0000 L CNN
F 1 "8MHz" V 4145 2994 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 4100 2850 50  0001 C CNN
F 3 "~" H 4100 2850 50  0001 C CNN
F 4 "C280087" H 4100 2850 50  0001 C CNN "LCSC"
	1    4100 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 2700 4100 2700
Wire Wire Line
	4100 2700 4100 2750
Connection ~ 4100 2750
Wire Wire Line
	4100 3000 4100 2950
Wire Wire Line
	3950 3000 4100 3000
Connection ~ 4100 2950
Connection ~ 3550 2850
Wire Wire Line
	3550 2850 3550 3000
Wire Wire Line
	3550 2850 4000 2850
Connection ~ 4000 2850
Wire Wire Line
	4000 2850 4200 2850
Connection ~ 4200 2850
Wire Wire Line
	4200 2850 4250 2850
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 5E568FFD
P 6700 4900
F 0 "J6" H 6780 4942 50  0000 L CNN
F 1 "PROG" H 6780 4851 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical_SMD_Pin1Left" H 6700 4900 50  0001 C CNN
F 3 "~" H 6700 4900 50  0001 C CNN
	1    6700 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4800 6500 4800
Wire Wire Line
	6500 4900 5800 4900
Text GLabel 6400 5100 3    50   UnSpc ~ 0
GND
Wire Wire Line
	6500 5000 6400 5000
Wire Wire Line
	6400 5000 6400 5100
Wire Wire Line
	3250 2750 3250 2600
Wire Wire Line
	3250 2600 4500 2600
Wire Wire Line
	3250 3400 3500 3400
Wire Wire Line
	3800 3450 3900 3450
Wire Wire Line
	3900 3200 3900 3450
Wire Wire Line
	3500 3400 3500 3350
Wire Wire Line
	3500 3400 3500 3450
Connection ~ 3500 3400
NoConn ~ 4500 3300
Text HLabel 3250 3200 0    50   Input ~ 0
G0_ESP
Text HLabel 3250 3100 0    50   Input ~ 0
EN_ESP
Wire Wire Line
	4400 4200 4500 4200
Text HLabel 4400 4200 0    50   Input ~ 0
i2c_SDA
Text HLabel 4250 4900 0    50   Output ~ 0
OE
Text HLabel 4250 4800 0    50   Output ~ 0
CLK
Text HLabel 4250 4700 0    50   Output ~ 0
LE
Wire Wire Line
	4250 5000 4500 5000
Text HLabel 4250 5000 0    50   Output ~ 0
SDI
Wire Wire Line
	4250 4800 4500 4800
Wire Wire Line
	4500 4700 4250 4700
Wire Wire Line
	4250 4900 4500 4900
$EndSCHEMATC
