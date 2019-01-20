#pragma once
#include "Value.h"
#include "SensorAlarm.h"
#include "ModuleDefines.h"

template <class T>
class SensorValue : public Value<T> {
private:

public:
	SensorValue();
	SensorValue(const char* _name, byte _alarmTypeMin = ALARM_OFF, byte _alarmTypeMax = ALARM_OFF);
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
SensorValue<T>::SensorValue(const char* _name, byte _alarmTypeMin, byte _alarmTypeMax):Value<T>(_name)
{
	/*const char* namePostfixMin = "MinAl";
	const char* namePostfixMax = "MaxAl";
	int sizeName = strlen(_name)+ strlen(namePostfixMin);
	
	char dumpMin[sizeName];
	
	strcpy(dumpMin, _name);
	strcat(dumpMin, namePostfixMin);*/
	
	//Serial.println(dumpMin);
	minAlarm = new SensorAlarm<T>("MinAlarm", _alarmTypeMin);
	//char dumpMax[sizeName];
	//Serial.println(sizeName);
	//strcpy(dumpMax, _name);
	//strcat(dumpMax, namePostfixMax);
	//Serial.println(dumpMax);
	
	maxAlarm = new SensorAlarm<T>("MaxAlarm", _alarmTypeMax); 
	
}

template <class T>
SensorValue<T>::~SensorValue()
{

}

