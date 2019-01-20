#pragma once
#include "Value.h"
#include "ModuleDefines.h"

template <class T>
class SensorAlarm : public Value<T> {
private:
	bool alarmTone;
	bool maxAlarm; //true, else it is a minAlarm
	byte alarmType;

public:
	SensorAlarm();
	SensorAlarm(const char* _name, byte _alarmType = ALARM_OFF, bool _maxAlarm = false);
	~SensorAlarm();
	void toggleAlarmTone();
	void loadAlarm();
	void saveAlarm();
	bool checkAlarm(T _sensorValue);
};

template <class T>
SensorAlarm<T>::SensorAlarm()
{

}

template <class T>
SensorAlarm<T>::SensorAlarm(const char* _name, byte _alarmType, bool _maxAlarm):Value<T>(_name)
{
	alarmType = _alarmType;
	maxAlarm = _maxAlarm;
}

template<class T>
inline SensorAlarm<T>::~SensorAlarm()
{
}

template<class T>
inline void SensorAlarm<T>::toggleAlarmTone()
{
	alarmTone = !alarmTone;
}

