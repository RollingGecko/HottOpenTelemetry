#pragma once

// #include "Module.h"
#include"HottModule_Interface.h"
#ifndef UNIT_TEST
	#include "ModuleDefines.h"
	// #include "Module.h"
#else
	// #include "HottModule_Mock.h"
	//#define HOTTMODULE_MOCKED
	#include <stdint.h>
	#define ALARM_OFF	0x00
#endif
#include "Value.h"

template <class T>
class SensorAlarm : public Value<T> {
private:
	bool alarmTone;
	bool maxAlarm; //true, else it is a minAlarm
	uint8_t alarmType;
	
	HottModule_Interface* moduleObject;

public:
	bool triggerAlarm();
	//SensorAlarm();
	/**
	 * @brief Construct a new Sensor Alarm object
	 * 
	 * @param _name 
	 * @param _moduleObject 
	 * @param _alarmType: defines alarm type in RC Transmitter
	 * @param _maxAlarm: if TRUE it is max Alarm, else it is a min Alarm (default)
	 */
	SensorAlarm(const char* _name,HottModule_Interface* _moduleObject ,uint8_t _alarmType = ALARM_OFF, bool _maxAlarm = false);
	~SensorAlarm();
	/**
	 * @brief Toggle Alarm Tone ON/OFF
	 * 
	 */
	void toggleAlarmTone();
	/**
	 * @brief Load Alarm from EEprom
	 * 
	 */
	void loadAlarm();
	/**
	 * @brief Save Alarm to EEprom
	 * 
	 */
	void saveAlarm();
	bool checkAlarm(T _sensorValue);
	bool getAlarmToneSetting();
};

template <class T>
bool SensorAlarm<T>::triggerAlarm()
{
	//moduleObject->set_Alert(alarmType);
	return true;
}

// template <class T>
// SensorAlarm<T>::SensorAlarm()
// {

// }

template <class T>
SensorAlarm<T>::SensorAlarm(const char* _name,HottModule_Interface* _moduleObject,uint8_t _alarmType, bool _maxAlarm):Value<T>(_name)
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

template<class T>
bool SensorAlarm<T>::getAlarmToneSetting()
{
	return alarmTone;
}

template<class T>
void SensorAlarm<T>::loadAlarm(){
	//ToDo
};
template<class T>
void SensorAlarm<T>::saveAlarm(){
	//ToDo
};
template<class T>
bool SensorAlarm<T>::checkAlarm(T _sensorValue){
	return true;//ToDo
};



