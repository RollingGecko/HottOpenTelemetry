#pragma once
#include "Value.h"
#include "ModuleDefines.h"

template <class T>
class SensorAlarm : public Value<T> {
private:
	bool AlarmTone;
	byte alarmType;

public:
	SensorAlarm();
	SensorAlarm(const char* _name, byte _alarmType = ALARM_OFF);
	~SensorAlarm();
	void changeAlarmTone();
	void loadAlarm();
	void saveAlarm();
	bool checkAlarm(T _sensorValue);
};

template <class T>
SensorAlarm<T>::SensorAlarm()
{

}

template <class T>
SensorAlarm<T>::SensorAlarm(const char* _name, byte _alarmType):Value<T>(_name)
{
	alarmType = _alarmType;
}

template<class T>
inline SensorAlarm<T>::~SensorAlarm()
{
}

