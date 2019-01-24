/*
 Name:		TestSensorValue.ino
 Created:	20.01.2019 10:45:07
 Author:	AC
*/

#include "Arduino.h"
#include "SensorValue.h"
#include "HottModule.h"

// the setup function runs once when you press reset or power the board
SensorValue<int>* sensor;
GamModule* hottmodule;

void setup() {
	Serial.begin(115200);
	sensor = new SensorValue<int>("Sens1",hottmodule);
	hottmodule = new GamModule();
	hottmodule->initSensors(false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false);
	/*Serial.print("getMinAlarm: "); Serial.println(sensor->minAlarm->getName());
	Serial.print("getMaxAlarm: "); Serial.println(sensor->maxAlarm->getName());*/


}

// the loop function runs over and over again until power down or reset
void loop() {

  
}
