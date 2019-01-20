#pragma once

#ifndef HottModule_h__
#define HottModule_h__

#include "Arduino.h"
//#include "Sensor.h"
#include "ModuleDefines.h"
#include "Page.h"
#include "SEnsorValue.h"

class Page;

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
private:
	friend class Page;
protected:
	Page* currentPage;
	uint8_t serialBinMessage[178];
	uint8_t serialTxtMessage[173];
	struct HOTT_TEXTMODE_MSG  *hott_txt_msg = (struct HOTT_TEXTMODE_MSG *)serialTxtMessage;
	bool dummyMessage = false;
	void cleanTxtMessage();

	 
	
public:
	HottModule();
	virtual ~HottModule();
	virtual void init_BinMsg() = 0;
	virtual void initTxtMsg();
	virtual void createBinMessage() = 0;
	virtual void createTxtMessage(byte keyId) = 0;
	virtual int getBinMessageSize() = 0;
	uint8_t* getBinMessage();
	uint8_t* getTxtMessage();
	int getTxtMessageSize();
	void setCurrentPage(Page* _page);
	void setDummyMessage(bool onOff);
};

//GAM Module


class GamModule : public HottModule
{
private:
	friend class Page;
	struct HOTT_GAM_MSG       *hott_gam_msg = (struct HOTT_GAM_MSG *)&serialBinMessage[0];
	long timeLastMessageSend = 0;
	bool invAlert = false;

protected:
   	  
	void set_Alert(byte alarm);
	void set_alarminvers1(byte alarm);
	void set_alarminvers2(byte alarm);
	void setBinMsg_cellVotlage(uint8_t cell, float voltage);
	void setBinMsg_Battery1(float voltage);
	void setBinMsg_Battery2(float voltage);
	byte setBinMsg_temp(int temp);
	void setBinMsg_temperature1(int temp);
	void setBinMsg_temperature2(int temp);
	void setBinMsg_fuelPercent(byte fuelPercent);
	void setBinMsg_fuelMl(uint16_t fuelMl);
	void setBinMsg_rpm(uint16_t rpm);
	void setBinMsg_altitude(uint16_t altitude);
	void setBinMsg_climbrateL(float climbrateL);
	void setBinMsg_climbrate3s(uint16_t climbrate3s);
	void setBinMsg_current(float current);
	void setBinMsg_mainVoltage(uint16_t voltage);
	void setBinMsg_battCap(uint16_t capacitiy);
	void setBinMsg_speed(uint16_t speed);
	void setBinMsg_minCellVoltage(uint16_t voltage);
	void setBinMsg_minCellVoltageNumber(byte cell);
	void setBinMsg_rpm2(uint16_t rpm);
	void setBinMsg_pressure_in_bar(float pressure);
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
	void initSensors(bool _cellVoltage, bool _battery1, bool _battery2, bool _temp1, bool _temp2
		, bool _fuelPerc, bool _fuelMl, bool _rpm, bool _alt, bool _climbL, bool _climb3s
		, bool _current, bool _voltageMain, bool _batCap, bool _speed, bool _minVolCelNum, bool _rpm2
		, bool _pressure);
	virtual void createBinMessage() override;
	virtual int getBinMessageSize() override;
	virtual void createTxtMessage(byte keyId) override;
	virtual void init_BinMsg() override;
	SensorValue<float>*		cellVoltage;
	SensorValue<float>*		battery1;
	SensorValue<float>*		battery2;
	SensorValue<byte>*		temperature1;
	SensorValue<byte>*		temperature2;
	SensorValue<byte>*		fuelPercentage;
	SensorValue<uint16_t>*	fuelMl;
	SensorValue<uint16_t>*	rpm;
	SensorValue<uint16_t>*	altitude;
	SensorValue<float>*		climbrate_L;
	SensorValue<uint16_t>*	climbrate3s;
	SensorValue<float>*		current;
	SensorValue<float>*		voltageMain;
	SensorValue<uint16_t>*	batteryCapacity;
	SensorValue<uint16_t>*	speed;
	SensorValue<byte>*	minVoltageCellNumber;
	SensorValue<uint16_t>*	rpm2;
	SensorValue<float>*		pressure;
	//void set_cellVotlage(uint8_t cell, float voltage);
	//void set_Battery1(float voltage);
	//void set_Battery2(float voltage);
	//byte set_temp(int temp);
	//void set_temperature1(int temp);
	//void set_temperature2(int temp);
	//void set_fuelPercent(byte fuelPercent);
	//void set_fuelMl(uint16_t fuelMl);
	//void set_rpm(uint16_t rpm);
	//void set_altitude(uint16_t altitude);
	//void set_climbrateL(float climbrateL);
	//void set_climbrate3s(uint16_t climbrate3s);
	//void set_current(float current);
	////void set_mainVoltage(uint16_t voltage);
	//void set_battCap(uint16_t capacitiy);
	//void set_speed(uint16_t speed);
	//void set_minCellVoltage(uint16_t voltage);
	//void set_minCellVoltageNumber(byte cell);
	//void set_rpm2(uint16_t rpm);
	//void set_pressure_in_bar(float pressure);
};

#endif // HottModule_h__
