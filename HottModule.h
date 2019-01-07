#pragma once

#include "Arduino.h"
#include "Sensor.h"
#include "ModuleDefines.h"

struct HOTT_TEXTMODE_MSG {
	byte start_byte;			//#01 Starting constant value == 0x7b
	byte esc;				//#02 Escape (higher-ranking menu in text mode or Text mode leave)
						  //0x00 to stay normal
						  //0x01 to exit
						  //I will send 2 times, so the ESCAPE works really well, so two data frames with 0x01 in byte 2
	byte warning_beeps;			//#03 1=A 2=B ...
	byte text[8][21];			//#04...#171 168 ASCII text to display to
						  // Bit 7 = 1 -> Inverse character display
						  // Display 21x8
	byte stop_byte;		        //#172 constant value 0x7d
	byte parity;				//#173 Checksum / parity
};

// String for text mode
#define HOTT_TEXTMODE_MSG_TEXT_LEN		168

class HottModule
{
protected:
	uint8_t serialBinMessage[178];
	uint8_t serialTxtMessage[173];
	struct HOTT_TEXTMODE_MSG  *hott_txt_msg = (struct HOTT_TEXTMODE_MSG *)&serialTxtMessage[0];
	bool dummyMessage = false;
	void cleanTxtMessage();
public:
	HottModule();
	virtual ~HottModule();
	virtual void init_BinMsg() = 0;
	virtual void initTxtMsg();
	virtual void createBinMessage() = 0;
	virtual void createTxtMessage() = 0;
	virtual int getBinMessageSize() = 0;
	uint8_t* getBinMessage();
	uint8_t* getTxtMessage();
	int getTxtMessageSize();

	void setDummyMessage(bool onOff);
};

//GAM Module

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

class GamModule : public HottModule
{
private:
	struct HOTT_GAM_MSG       *hott_gam_msg = (struct HOTT_GAM_MSG *)&serialBinMessage[0];
	long timeLastMessageSend = 0;
	bool invAlert = false;

protected:
	void set_Alert(byte alarm);
	void set_alarminvers1(byte alarm);
	void set_alarminvers2(byte alarm);
	void set_cellVotlage(uint8_t cell, float voltage);
	void set_Battery1(float voltage);
	void set_Battery2(float voltage);
	byte set_temp(int temp);
	void set_temperature1(int temp);
	void set_temperature2(int temp);
	void set_fuelPercent(byte fuelPercent);
	void set_fuelMl(uint16_t fuelMl);
	void set_rpm(uint16_t rpm);
	void set_altitude(uint16_t altitude);
	void set_climbrateL(float climbrateL);
	void set_climbrate3s(uint16_t climbrate3s);
	void set_current(float current);
	void set_mainVoltage(uint16_t voltage);
	void set_battCap(uint16_t capacitiy);
	void set_speed(uint16_t speed);
	void set_minCellVoltage(uint16_t voltage);
	void set_minCellVoltageNumber(byte cell);
	void set_rpm2(uint16_t rpm);
	void set_pressure_in_bar(float pressure);
	void set_InvAlarm_allCellvoltage();
	void set_InvAlarm_Bat1();
	void set_InvAlarm_Bat2();
	void set_InvAlarm_Temp1();
	void set_InvAlarm_Temp2();
	void set_InvAlarm_Fuel();
	void set_InvAlarm_Rpm();
	void set_InvAlarm_Altitude();
	void set_InvAlarm_mainPowerCurrent();
	void set_InvAlarm_mainPowerVoltage();
	void set_InvAlarm_climb();
	void set_InvAlarm_climb3();
	void clear_InvAlarm();
	

public:
	GamModule();
	virtual void createBinMessage() override;
	virtual int getBinMessageSize() override;
	virtual void createTxtMessage() override;
	virtual void init_BinMsg() override;
};