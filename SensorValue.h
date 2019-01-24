#pragma once
#include "Value.h"
#include "SensorAlarm.h"
#include "ModuleDefines.h"
#include "HottModule.h"

template <class T>
class SensorValue : public Value<T> {
private:

public:
	SensorValue();
	SensorValue(const char* _name, HottModule* _moduleObject, uint8_t _alarmTypeMin = ALARM_OFF, uint8_t _alarmTypeMax = ALARM_OFF);
	~SensorValue();
		SensorAlarm<T>* minAlarm;
	SensorAlarm<T>* maxAlarm;

};

//template <class T>
//SensorValue<T>::SensorValue()
//{
//
//}

template <class T>
SensorValue<T>::SensorValue(const char* _name, HottModule* _moduleObject, uint8_t _alarmTypeMin, uint8_t _alarmTypeMax):Value<T>(_name)
{
	/*const char* namePostfixMin = "MinAl";
	const char* namePostfixMax = "MaxAl";
	int sizeName = strlen(_name)+ strlen(namePostfixMin);
	
	char dumpMin[sizeName];
	
	strcpy(dumpMin, _name);
	strcat(dumpMin, namePostfixMin);*/
	
	//Serial.println(dumpMin);
	minAlarm = new SensorAlarm<T>("MinAlarm",_moduleObject, _alarmTypeMin);
	//char dumpMax[sizeName];
	//Serial.println(sizeName);
	//strcpy(dumpMax, _name);
	//strcat(dumpMax, namePostfixMax);
	//Serial.println(dumpMax);
	
	maxAlarm = new SensorAlarm<T>("MaxAlarm",_moduleObject, _alarmTypeMax, true); 
	
}

template <class T>
SensorValue<T>::~SensorValue()
{

}

