/*
 Name:		HottOpenTelemetry.ino
 Created:	31.12.2018 12:01:37
 Author:	AC
*/


#ifndef UNIT_TEST

// #ifdef UNIT_TEST
//     #include "ArduinoFake.h"
// #else
#include "Arduino.h"
// #endif
// #ifndef UNIT_TEST
#include <Wire.h>
// #endif

#include <SoftwareSerial.h>
#include "HottModule.h"
#include "MessageSender.h"
#include "Page.h"


// Baudrate UART
#define SERIAL_COM_SPEED    19200

SoftwareSerial HottSerial(10, 11);
GamModule *Hott_GamModule;// = new GamModule();
HottMessage *MessageSender;// = new HottMessage(HottSerial);
Page* startPage;
Page* secondPage;



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	//Serial.println("Setup");
	HottSerial.begin(SERIAL_COM_SPEED);
	Hott_GamModule = new GamModule();
	Hott_GamModule->initSensors(true, true, true, true, true, true, true, true, true,
		true, true, true, true, true, true, true, true, true);
	//Hott_GamModule->initSensors(false, true, false, false, false, false, false, false, false, false, false, false, 
	//	false, false, false, false, false, false);
	Hott_GamModule->setDummyMessage(false);
	MessageSender = new HottMessage(&HottSerial);
	MessageSender->setGamModule(Hott_GamModule);
	startPage = new Page("Start Page", Hott_GamModule);
	secondPage = new Page("Second Page", Hott_GamModule);
	startPage->setNext(secondPage);
	secondPage->setPrevious(startPage);

	//Page startPage("Start Page", Hott_GamModule);
	Hott_GamModule->setCurrentPage(startPage);

	Hott_GamModule->cellVoltage->setValue(4.2);
	Hott_GamModule->battery1->setValue(8.4);
	Hott_GamModule->battery2->setValue(8.4);
	Hott_GamModule->temperature1->setValue(120);
	Hott_GamModule->temperature2->setValue(121);
	Hott_GamModule->fuelPercentage->setValue(80);
	Hott_GamModule->fuelMl->setValue(1200);
	Hott_GamModule->rpm->setValue(34000);
	Hott_GamModule->altitude->setValue(120);
	Hott_GamModule->climbrate_L->setValue(5.2);
	Hott_GamModule->climbrate3s->setValue(10.2);
	Hott_GamModule->current->setValue(23.4);
	Hott_GamModule->voltageMain->setValue(10.2);
	Hott_GamModule->batteryCapacity->setValue(2200);
	Hott_GamModule->speed->setValue(223);
	Hott_GamModule->minVoltageCellNumber->setValue(2);
	Hott_GamModule->rpm2->setValue(33000);
	Hott_GamModule->pressure->setValue(12.1);


	
}

// the loop function runs over and over again until power down or reset
void loop() {

	MessageSender->sendMessage();
	
}

#endif


