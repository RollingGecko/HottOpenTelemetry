#pragma once
#include "Value.h"
#include "HottModule_Interface.h"
#include "SensorAlarm.h"
#include "ModuleDefines.h"
//#include "Module.h"

template <class T>
class SensorAlarm;

class HottModule_Interface;

template <class T>
class SensorValue : public Value<T> {
private:

public:
	SensorValue();
	SensorValue(const char* _name, HottModule_Interface* _moduleObject, byte _alarmTypeMin = ALARM_OFF, byte _alarmTypeMax = ALARM_OFF);
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
SensorValue<T>::SensorValue(const char* _name, HottModule_Interface* _moduleObject, byte _alarmTypeMin, byte _alarmTypeMax):Value<T>(_name)
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

