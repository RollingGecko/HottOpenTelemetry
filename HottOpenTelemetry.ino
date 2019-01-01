/*
 Name:		HottOpenTelemetry.ino
 Created:	31.12.2018 12:01:37
 Author:	AC
*/

#include "HottModule.h"
#include "MessageSender.h"
#include <SoftwareSerial.h>

// Baudrate UART
#define SERIAL_COM_SPEED    19200

SoftwareSerial HottSerial(10, 11);
GamModule Hott_GamModule;
HottMessage MessageSender(HottSerial);

// the setup function runs once when you press reset or power the board
void setup() {

	
	HottSerial.begin(SERIAL_COM_SPEED);
	MessageSender.setGamModule(Hott_GamModule);
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	MessageSender.sendMessage();
	
}
