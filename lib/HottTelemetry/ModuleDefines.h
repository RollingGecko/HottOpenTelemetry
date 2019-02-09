#pragma once

#ifndef ModuleDefines_h__
#define ModuleDefines_h__

#define HOTT_TELEMETRY_GEA_SENSOR_ID    0x8E // Electric Air Module ID
#define HOTT_BINARY_MODE_REQUEST_ID		0x80
#define HOTT_TEXT_MODE_REQUEST_ID       0x7f
#define HOTT_TELEMETRY_GAM_SENSOR_ID    0x8d

//Alerts

#define ALARM_OFF				0x00//0x00  00  0  No alarm
#define	ALARM_MINSPEED			0x01 //01  A  MinSpeed
#define	ALARM_SINKRATE2			0x02 //02  B  Sink Rate 3s 
#define	ALARM_SINKRATE1			0x03 //03  C  Sink Rate 1s
#define	ALARM_MAXDISTANCE		0x04 //04  D  Maximum distance
#define	ALARM_WEAKTELEMETRY		0x05 //05  E  Weak telemetry
#define	ALARM_MINTEMP1			0x06 //06  F  Min.Sensor 1 temp.F
#define	ALARM_MINTEMP2			0x07 //07  G  Min.Sensor 2 temp.G
#define	ALARM_MAXTEMP1			0x08 //08  H  Max.Sensor 1 temp.H
#define	ALARM_MAXTEMP2			0x09 //09  I  Max.Sensor 2 temp.I
#define	ALARM_MAXSENSVOL1		0xA  //10  J  Max.Sens. 1 voltage J
#define	ALARM_MAXSENSVOL2		0xB  //11  K  Max.Sens. 2 voltage K
#define	ALARM_MAXSPEED			0xC  //12  L  Max. speed
#define	ALARM_CLIMBRATE2		0xD  //13  M  Climb rate 2 M
#define	ALARM_CLIMBRATE1		0xE  //14  N  Climb rate 1 N
#define	ALARM_MINALT			0xF  //15  O  Min.Altitude O
#define	ALARM_MINPOWVOLTAGE		0x10 //16  P  Min.Power Voltage P    // We use this one for Battery Warning
#define	ALARM_MINCELLVOLTAGE	0x11 //17  Q  Min.Cell voltage Q
#define	ALARM_MINSENSVOL1		0x12 //18  R  Min.Sens. 1 voltage R
#define	ALARM_MINSENSVOL2		0x13 //19  S  Min.Sens. 2 voltage S
#define	ALARM_MINRPM			0x14 //20  T  Minimum RPM T
#define	ALARM_FUELRESERVE		0x15 //21  U  Fuel reserve
#define	ALARM_MAXUSEDCAPACITY	0x16 //22  V  Max.used capacity V
#define	ALARM_MAXCURRENT		0x17 //23  W  Max.Current W
#define	ALARM_MAXPPOWVOLTAGE	0x18 //24  X  Max.Power Voltage X
#define	ALARM_MAXRPM			0x19 //25  Y  Maximum RPM Y
#define	ALARM_MAXALTITUDE		0x1A //26  Z  Max.Altitude Z


// Radio keys
#define HOTT_KEY_RIGHT  14	//0xE	ENTER
#define HOTT_KEY_DOWN   11	//0xB	DEC
#define HOTT_KEY_UP     13	//0xD	INC
#define HOTT_KEY_SET    9	//0x9	INC+DEC
#define HOTT_KEY_LEFT   7	//0x7	ESC


struct HOTT_GAM_MSG {
	byte start_byte;                      //#01 start byte constant value 0x7c
	byte gam_sensor_id;             	//#02 EAM sensort id. constat value 0x8d=GENRAL AIR MODULE
	byte warning_beeps;                   //#03 1=A 2=B ... 0x1a=Z  0 = no alarm									  
	byte sensor_id;             	        //#04 constant value 0xd0
	byte alarm_invers1;                   //#05 alarm bitmask. Value is displayed inverted
										  //Bit#  Alarm field
										  // 0    all cell voltage
										  // 1    Battery 1
										  // 2    Battery 2
										  // 3    Temperature 1
										  // 4    Temperature 2
										  // 5    Fuel
										  // 6    rpm
										  // 7    Altitude
	byte alarm_invers2;                   //#06 alarm bitmask. Value is displayed inverted
										  //Bit#  Alarm Field
										  // 0    main power current
										  // 1    main power voltage
										  // 2    Altitude
										  // 3    m/s                            
										  // 4    m/3s
										  // 5    Current
										  // 6    Main Voltage
										  // 7    "ON" sign/text msg active
	byte cell[6];				//#7 Volt Cell 1 (in 2 mV increments, 210 == 4.20 V)
								//#8 Volt Cell 2 (in 2 mV increments, 210 == 4.20 V)
								//#9 Volt Cell 3 (in 2 mV increments, 210 == 4.20 V)
								//#10 Volt Cell 4 (in 2 mV increments, 210 == 4.20 V)
								//#11 Volt Cell 5 (in 2 mV increments, 210 == 4.20 V)
								//#12 Volt Cell 6 (in 2 mV increments, 210 == 4.20 V)
	uint16_t  Battery1;                   //#13 LSB battery 1 voltage LSB value. 0.1V steps. 50 = 5.5V only pos. voltages
										  //#14 MSB 
	uint16_t  Battery2;                   //#15 LSB battery 2 voltage LSB value. 0.1V steps. 50 = 5.5V only pos. voltages
										  //#16 MSB
	byte temperature1;                    //#17 Temperature 1. Offset of 20. a value of 20 = 0°C
	byte temperature2;                    //#18 Temperature 2. Offset of 20. a value of 20 = 0°C
	byte fuel_percent;                    //#19 Fuel capacity in %. Values 0--100
										  //graphical display ranges: 0-100% with new firmwares of the radios MX12/MX20/...
	uint16_t fuel_ml;                     //#20 LSB Fuel in ml scale. Full = 65535!
										  //#21 MSB
	uint16_t rpm;                         //#22 RPM in 10 RPM steps. 300 = 3000rpm
										  //#23 MSB
	uint16_t altitude;                    //#24 altitude in meters. offset of 500, 500 = 0m
										  //#25 MSB
	uint16_t climbrate_L;                 //#26 climb rate in 0.01m/s. Value of 30000 = 0.00 m/s
										  //#27 MSB
	byte climbrate3s;                     //#28 climb rate in m/3sec. Value of 120 = 0m/3sec
	uint16_t current;                     //#29 current in 0.1A steps 100 == 10,0A
										  //#30 MSB current display only goes up to 99.9 A (continuous)
	uint16_t main_voltage;            	//#31 LSB Main power voltage using 0.1V steps 100 == 10,0V
										//#32 MSB (Appears in GAM display right as alternate display.)
	uint16_t batt_cap;                    //#33 LSB used battery capacity in 10mAh steps
										  //#34 MSB 
	uint16_t speed;                       //#35 LSB (air?) speed in km/h(?) we are using ground speed here per default
										  //#36 MSB speed
	byte min_cell_volt;                   //#37 minimum cell voltage in 2mV steps. 124 = 2,48V
	byte min_cell_volt_num;               //#38 number of the cell with the lowest voltage
	uint16_t rpm2;                        //#39 LSB 2nd RPM in 10 RPM steps. 100 == 1000rpm
										  //#40 MSB
	byte general_error_number;      	//#41 General Error Number (Voice Error == 12) TODO: more documentation
	byte pressure_in_bar;                 //#42 High pressure up to 16bar. 0,1bar scale. 20 == 2.0bar
	byte version;                         //#43 version number (Bytes 35 .43 new but not yet in the record in the display!)
	byte stop_byte;                       //#44 stop byte 0x7D
	byte parity;                          //#45 CHECKSUM CRC/Parity (calculated dynamicaly)
};

struct gam_values {
	uint16_t cell[6];					// Volt Cell 1-6 [div 10]
	uint16_t  Battery1;					//battery 1 voltage [div 10]
	uint16_t Battery2;					//battery 2 voltage [div 10]
	uint8_t temperature1;
	uint8_t temperature2;
	byte fuel_percent;               										  //graphical display ranges: 0-100% with new firmwares of the radios MX12/MX20/...
	uint16_t fuel_ml;
	uint16_t rpm;
	uint16_t altitude;
	uint16_t climbrate_L;
	uint16_t climbrate3s;                     //climb rate in m/3sec.
	uint16_t current;                     //#29 current in 0.1A steps 100 == 10,0A
	uint16_t main_voltage;            	//[div 10]
	uint16_t batt_cap;                    //used battery capacity in 10mAh steps
	uint16_t speed;                       //speed in km/h
	uint16_t min_cell_volt;               //[div 10]
	byte min_cell_volt_num;               //number of the cell with the lowest voltage
	uint16_t rpm2;                        //RPM in 10 RPM steps. 100 == 1000rpm
	byte general_error_number;      	//#41 General Error Number (Voice Error == 12) TODO: more documentation
	uint16_t pressure_in_bar;                 //[div 10]
	byte version;                         //#43 version number (Bytes 35 .43 new but not yet in the record in the display!)
};
#endif // ModuleDefines_h__