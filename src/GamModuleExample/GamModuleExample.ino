/*
 Name:		HottOpenTelemetry.ino
 Created:	31.12.2018 12:01:37
 Author:	AC
*/


#include "Arduino.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include "HottModule.h"
#include "MessageSender.h"
#include "Page.h"
#include "SensorValue.h"


// Baudrate UART
#define SERIAL_COM_SPEED    19200

SoftwareSerial HottSerial(10, 11);
GamModule *Hott_GamModule;// = new GamModule();
HottMessage *MessageSender;// = new HottMessage(HottSerial);

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

Page* startPage;
Page* secondPage;



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	//Serial.println("Setup");
	HottSerial.begin(SERIAL_COM_SPEED);
	Hott_GamModule = new GamModule();

	cellVoltage				= new SensorValue<float>("cellV",Hott_GamModule,ALARM_MINCELLVOLTAGE);
	battery1 				= new SensorValue<float>("Bat1", Hott_GamModule, ALARM_MINSENSVOL1, ALARM_MAXSENSVOL1);
	battery2 				= new SensorValue<float>("Bat2", Hott_GamModule, ALARM_MINSENSVOL2, ALARM_MAXSENSVOL2);
	temperature1			= new SensorValue<byte>("Temp1", Hott_GamModule, ALARM_MINTEMP1, ALARM_MAXTEMP1);
	temperature2 			= new SensorValue<byte>("Temp2", Hott_GamModule, ALARM_MINTEMP2, ALARM_MAXTEMP2);
	fuelPercentage 			= new SensorValue<byte>("Fuel%",Hott_GamModule);
	fuelMl 					= new SensorValue<uint16_t>("Fuel", Hott_GamModule, ALARM_FUELRESERVE);
	rpm 					= new SensorValue<uint16_t>("RPM", Hott_GamModule, ALARM_MINRPM, ALARM_MAXRPM);
	altitude 				= new SensorValue<uint16_t>("Alt", Hott_GamModule, ALARM_MINALT, ALARM_MAXALTITUDE);
	climbrate_L 			= new SensorValue<float>("ClimbL", Hott_GamModule, ALARM_OFF, ALARM_CLIMBRATE1);
	climbrate3s 			= new SensorValue<uint16_t>("Climb3s", Hott_GamModule, ALARM_OFF, ALARM_OFF);
	current 				= new SensorValue<float>("Cur", Hott_GamModule, ALARM_OFF, ALARM_MAXCURRENT);
	voltageMain 			= new SensorValue<float>("MainVol", Hott_GamModule, ALARM_MINPOWVOLTAGE, ALARM_MAXPPOWVOLTAGE);
	batteryCapacity 		= new SensorValue<uint16_t>("Cap", Hott_GamModule, ALARM_OFF, ALARM_MAXUSEDCAPACITY);
	speed 					= new SensorValue<uint16_t>("Speed", Hott_GamModule, ALARM_MINSPEED, ALARM_MAXSPEED);
	minVoltageCellNumber	= new SensorValue<byte>("MinVolCel", Hott_GamModule, ALARM_OFF, ALARM_OFF);
	rpm2 					= new SensorValue<uint16_t>("RPM2", Hott_GamModule, ALARM_MINRPM, ALARM_MAXRPM);
	pressure 				= new SensorValue<float>("Pressure", Hott_GamModule, ALARM_OFF, ALARM_OFF);

	Hott_GamModule->setDummyMessage(false);
	MessageSender = new HottMessage(&HottSerial);
	MessageSender->setGamModule(Hott_GamModule);
	startPage = new Page("Start Page", Hott_GamModule);
	secondPage = new Page("Second Page", Hott_GamModule);
	startPage->setNext(secondPage);
	secondPage->setPrevious(startPage);

	//Page startPage("Start Page", Hott_GamModule);
	Hott_GamModule->setCurrentPage(startPage);
	Hott_GamModule->set_cellVoltageSensor(cellVoltage);
	Hott_GamModule->set_battery1Sensor(battery1);
	Hott_GamModule->set_battery2Sensor(battery2);
	Hott_GamModule->set_temperature1Sensor(temperature1);
	Hott_GamModule->set_temperature2Sensor(temperature2);
	Hott_GamModule->set_fuelPercentageSensor(fuelPercentage);
	Hott_GamModule->set_fuelMlSensor(fuelMl);
	Hott_GamModule->set_rpmSensor(rpm);
	Hott_GamModule->set_altitudeSensor(altitude);
	Hott_GamModule->set_climbrateLSensor(climbrate_L);
	Hott_GamModule->set_climbrate3sSensor(climbrate3s);
	Hott_GamModule->set_currentSensor(current);
	Hott_GamModule->set_voltageMainSensor(voltageMain);
	Hott_GamModule->set_batteryCapacitySensor(batteryCapacity);
	Hott_GamModule->set_speedSensor(speed);
	Hott_GamModule->set_MinVoltageCellNumberSensor(minVoltageCellNumber);
	Hott_GamModule->set_rpm2Sensor(rpm2);
	Hott_GamModule->set_pressureSensor(pressure);


	cellVoltage->setValue(4.2);
	battery1->setValue(8.4);
	battery2->setValue(8.4);
	temperature1->setValue(120);
	temperature2->setValue(121);
	fuelPercentage->setValue(80);
	fuelMl->setValue(1200);
	rpm->setValue(34000);
	altitude->setValue(120);
	climbrate_L->setValue(5.2);
	climbrate3s->setValue(10.2);
	current->setValue(23.4);
	voltageMain->setValue(10.2);
	batteryCapacity->setValue(2200);
	speed->setValue(223);
	minVoltageCellNumber->setValue(2);
	rpm2->setValue(33000);
	pressure->setValue(12.1);


	
}

// the loop function runs over and over again until power down or reset
void loop() {

	MessageSender->sendMessage();
	
}



