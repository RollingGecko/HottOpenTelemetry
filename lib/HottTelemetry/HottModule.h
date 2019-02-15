#pragma once

#ifndef HottModule_h__
#define HottModule_h__

#ifdef UNIT_TEST
	#include "Mock_Arduino.h"
#else
    #include "Arduino.h"
#endif //UNIT_TEST

//#include "Sensor.h"
#include "HottModule_Interface.h"
// #include"HottModule.h"
#include "ModuleDefines.h"
#include "Page.h"
#include "SensorValue.h"
#include "SensorAlarm.h"

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

class HottModule : public HottModule_Interface
// class HottModule : public Module
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
	virtual void set_Alert(byte alarm) = 0;

	 
	
public:
	HottModule();
	virtual ~HottModule();
	virtual void init_BinMsg() = 0;
	virtual void initTxtMsg();
	virtual void createBinMessage() = 0;
	virtual void createTxtMessage(byte keyId) = 0;
	virtual int getBinMessageSize() = 0;
	virtual uint8_t* getBinMessage();
	virtual uint8_t* getTxtMessage();
	virtual int getTxtMessageSize();
	virtual void setCurrentPage(Page* _page);
	virtual void setDummyMessage(bool onOff);
};

//GAM Module


class GamModule : public HottModule
{
private:
	friend class Page;
	struct HOTT_GAM_MSG       *hott_gam_msg = (struct HOTT_GAM_MSG *)&serialBinMessage[0];
	long timeLastMessageSend = 0;
	bool invAlert = false;
	
	SensorValue<float>*		cellVoltage; //ToDo: Bring an array of values in this object 
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
	SensorValue<byte>*		minVoltageCellNumber;
	SensorValue<uint16_t>*	rpm2;
	SensorValue<float>*		pressure;


protected:
   	  
	
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
	
	virtual void createBinMessage() override;
	virtual int getBinMessageSize() override;
	virtual void createTxtMessage(byte keyId) override;
	virtual void init_BinMsg() override;
	virtual void set_cellVoltageSensor(SensorValue<float>* _cellVoltage);
	virtual void set_battery1Sensor(SensorValue<float>* _battery1);
	virtual void set_battery2Sensor(SensorValue<float>* _battery2);
	virtual void set_temperature1Sensor(SensorValue<byte>* _temperature1);
	virtual void set_temperature2Sensor(SensorValue<byte>* _temperature2);
	virtual void set_fuelPercentageSensor(SensorValue<byte>* _fuelPercentage);
	virtual void set_fuelMlSensor(SensorValue<uint16_t>* _fuelML);
	virtual void set_rpmSensor(SensorValue<uint16_t>* _rpm);
	virtual void set_altitudeSensor(SensorValue<uint16_t>* _altitude);
	virtual void set_climbrateLSensor(SensorValue<float>* _climbrateL);
	virtual void set_climbrate3sSensor(SensorValue<uint16_t>* _climbrate3s);
	virtual void set_currentSensor(SensorValue<float>* _current);
	virtual void set_voltageMainSensor(SensorValue<float>* _voltageMain);
	virtual void set_batteryCapacitySensor(SensorValue<uint16_t>* _capacity);
	virtual void set_speedSensor(SensorValue<uint16_t>* _speed);
	virtual void set_MinVoltageCellNumberSensor(SensorValue<byte>* _cellNumber);
	virtual void set_rpm2Sensor(SensorValue<uint16_t>* _rpm2);
	virtual void set_pressureSensor(SensorValue<float>* _pressure);
	void set_Alert(uint8_t alarm);
	
};
#endif // HottModule_h__
