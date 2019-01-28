#pragma once

#ifndef SensorAlarm_h__
#define SensorAlarm_h__


#include "Value.h"
#include "ModuleDefines.h"
#include "HottModule.h"

class HottModule;

template <class T>
class SensorAlarm : public Value<T> {
private:
	bool alarmTone;
	bool maxAlarm; //true, else it is a minAlarm
	uint8_t alarmType;
	void triggerAlarm();
	HottModule* moduleObject;

public:
	SensorAlarm();
	SensorAlarm(const char* _name,HottModule* _moduleObject ,uint8_t _alarmType = ALARM_OFF, bool _maxAlarm = false);
	~SensorAlarm();
	void toggleAlarmTone();
	void loadAlarm();
	void saveAlarm();
	bool checkAlarm(T _sensorValue);
};

template <class T>
void SensorAlarm<T>::triggerAlarm()
{
//	HottModule->set_Alert(alarmType); //ToDo Build error
}

template <class T>
SensorAlarm<T>::SensorAlarm()
{

}

template <class T>
SensorAlarm<T>::SensorAlarm(const char* _name,HottModule* _moduleObject ,uint8_t _alarmType, bool _maxAlarm):Value<T>(_name)
{
	alarmType = _alarmType;
	maxAlarm = _maxAlarm;
	moduleObject = _moduleObject;
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

#endif // SensorAlarm_h__
