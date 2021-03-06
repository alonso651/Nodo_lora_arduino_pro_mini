EESchema Schematic File Version 4
LIBS:pro_mini_lorawan_sensor-cache
EELAYER 29 0
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
L arduino-pro-mini:arduino-pro-mini U1
U 1 1 5D0D7C1E
P 3300 2450
F 0 "U1" H 3300 2450 55  0000 C CNN
F 1 " " H 3300 3338 55  0000 C CNN
F 2 "" H 3200 3300 55  0001 C CNN
F 3 "" H 3200 3300 55  0001 C CNN
	1    3300 2450
	1    0    0    -1  
$EndComp
$Comp
L RF_Module:RFM95W-868S2 U2
U 1 1 5D0D95FE
P 3350 4150
F 0 "U2" H 3350 4150 50  0000 C CNN
F 1 "RFM95W" H 3350 4740 50  0000 C CNN
F 2 "" H 50  5800 50  0001 C CNN
F 3 "https://www.hoperf.com/data/upload/portal/20181127/5bfcbea20e9ef.pdf" H 50  5800 50  0001 C CNN
	1    3350 4150
	-1   0    0    1   
$EndComp
Wire Wire Line
	3450 3550 3350 3550
Connection ~ 3250 3550
Wire Wire Line
	3250 3550 2850 3550
Connection ~ 3350 3550
Wire Wire Line
	3350 3550 3250 3550
Text Notes 2850 4850 0    50   ~ 0
Chip de comunicación LoRa
Text Notes 2900 1600 0    50   ~ 0
Controlador Pro-Mini
Wire Wire Line
	3650 2950 4000 2950
Wire Wire Line
	3650 3050 4000 3050
Wire Wire Line
	3650 3150 4000 3150
Wire Wire Line
	3650 3250 4000 3250
Wire Wire Line
	3650 2350 4000 2350
Wire Wire Line
	3650 2450 4000 2450
Wire Wire Line
	3650 2650 4000 2650
Wire Wire Line
	2950 3250 2700 3250
Wire Wire Line
	2950 3150 2700 3150
Wire Wire Line
	2950 1800 2700 1800
Wire Wire Line
	2950 1700 2700 1700
Wire Wire Line
	3850 4150 4150 4150
Wire Wire Line
	3850 4250 4150 4250
Wire Wire Line
	3850 4350 4150 4350
Wire Wire Line
	3850 4450 4150 4450
Wire Wire Line
	2850 4450 2500 4450
Wire Wire Line
	2850 3750 2500 3750
Wire Wire Line
	2850 3850 2500 3850
Wire Wire Line
	2850 3950 2500 3950
Wire Wire Line
	3350 4650 3600 4650
Text Label 4000 2350 0    50   ~ 0
DIO0
Text Label 4000 2450 0    50   ~ 0
DIO1
Text Label 4000 2650 0    50   ~ 0
DIO2
Text Label 4000 2950 0    50   ~ 0
NSS
Text Label 4000 3050 0    50   ~ 0
MOSI
Text Label 4000 3150 0    50   ~ 0
MISO
Text Label 4000 3250 0    50   ~ 0
SCK
Text Label 2700 1700 0    50   ~ 0
Vin
Text Label 2700 1800 0    50   ~ 0
3v3
Text Label 2700 3150 0    50   ~ 0
GND
Text Label 2700 3250 0    50   ~ 0
GND
Text Label 4150 4150 0    50   ~ 0
NSS
Text Label 4150 4250 0    50   ~ 0
MISO
Text Label 4150 4350 0    50   ~ 0
MOSI
Text Label 4150 4450 0    50   ~ 0
SCK
Text Label 2500 4450 0    50   ~ 0
ANT
Text Label 2500 3950 0    50   ~ 0
DIO2
Text Label 2500 3850 0    50   ~ 0
DIO1
Text Label 2500 3750 0    50   ~ 0
DIO0
Text Label 2850 3550 0    50   ~ 0
GND
Text Label 3600 4650 0    50   ~ 0
3v3
$Comp
L Connector:Conn_Coaxial J?
U 1 1 5D5F9F61
P 5350 1600
F 0 "J?" H 5450 1575 50  0000 L CNN
F 1 "Conector Antena" H 5450 1484 50  0000 L CNN
F 2 "" H 5350 1600 50  0001 C CNN
F 3 " ~" H 5350 1600 50  0001 C CNN
	1    5350 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 1600 4850 1600
Wire Wire Line
	5350 1800 4850 1800
Text Label 4850 1600 0    50   ~ 0
ANT
Text Label 4850 1800 0    50   ~ 0
GND
Wire Wire Line
	3650 2000 4000 2000
Wire Wire Line
	3650 1900 4000 1900
$EndSCHEMATC
