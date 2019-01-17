/*
 Name:		HottOpenTelemetry.ino
 Created:	31.12.2018 12:01:37
 Author:	AC
*/
#include <Arduino.h>
#include <Wire.h>
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
	Hott_GamModule->setDummyMessage(true);
	MessageSender = new HottMessage(&HottSerial);
	MessageSender->setGamModule(Hott_GamModule);
	startPage = new Page("Start Page", Hott_GamModule);
	secondPage = new Page("Second Page", Hott_GamModule);
	startPage->setNext(secondPage);
	secondPage->setPrevious(startPage);

	//Page startPage("Start Page", Hott_GamModule);
	Hott_GamModule->setCurrentPage(startPage);

	
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Serial.println("loop");
	MessageSender->sendMessage();
	
}
