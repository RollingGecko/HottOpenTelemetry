#include "HottModule.h"

#include "math.h"

HottModule::HottModule()
{

}


HottModule::~HottModule()
{
}


uint8_t* HottModule::getBinMessage()
{
	return serialBinMessage;
}

void HottModule::setDummyMessage(bool onOff)
{
	dummyMessage = onOff;
}

void GamModule::set_alarminvers1(byte alarm)
{
	hott_gam_msg->alarm_invers1 = alarm;
}

void GamModule::set_alarminvers2(byte alarm)
{
	hott_gam_msg->alarm_invers2 = alarm;
}

void GamModule::init_BinMsg()
{
	memset(hott_gam_msg, 0, sizeof(struct HOTT_GAM_MSG));
	hott_gam_msg->start_byte = 0x7c;
	hott_gam_msg->gam_sensor_id = HOTT_TELEMETRY_GAM_SENSOR_ID;
	hott_gam_msg->sensor_id = 0xd0;
	hott_gam_msg->stop_byte = 0x7d;
}

void GamModule::set_cellVotlage(uint8_t cell, float voltage)
{
		hott_gam_msg->cell[cell] = byte(round(voltage / 0.02));
	}

void GamModule::set_Battery1(float voltage)
{
	hott_gam_msg->Battery1 = int(round(voltage / 0.1));
}

void GamModule::set_Battery2(float voltage)
{
	hott_gam_msg->Battery2 = int(round(voltage / 0.1));
}

byte GamModule::set_temp(int temp)
{
	int tempOut = 0;
	if (temp < -20)
		tempOut = 0;
	else if (temp > 234)
		tempOut = 235;
	else
		tempOut = temp + 20;
	return tempOut;
}

void GamModule::set_temperature1(int temp)
{
	hott_gam_msg->temperature1 = set_temp(temp);
}

void GamModule::set_temperature2(int temp)
{
	hott_gam_msg->temperature2 = set_temp(temp);
}
void GamModule::set_fuelPercent(byte fuelPercent)
{
	hott_gam_msg->fuel_percent = fuelPercent;
}

void GamModule::set_fuelMl(uint16_t fuelMl)
{
	hott_gam_msg->fuel_ml = fuelMl;
}

void GamModule::set_rpm(uint16_t rpm)
{
	hott_gam_msg->rpm = rpm / 10;
}

void GamModule::set_altitude(uint16_t altitude)
{
	hott_gam_msg->altitude = altitude + 500;
}

void GamModule::set_climbrateL(float climbrateL)
{
	hott_gam_msg->climbrate_L = long(climbrateL / 0.01) + 30000;
}

void GamModule::set_climbrate3s(uint16_t climbrate3s)
{
	hott_gam_msg->climbrate3s = climbrate3s + 120;
}

void GamModule::set_current(float current)
{
	hott_gam_msg->current = long(current / 0.1);
}

void GamModule::set_mainVoltage(uint16_t voltage)
{
	hott_gam_msg->main_voltage = long(voltage / 0.1);
}

void GamModule::set_battCap(uint16_t capacitiy)
{
	hott_gam_msg->batt_cap = capacitiy / 10;
}

void GamModule::set_speed(uint16_t speed)
{
	hott_gam_msg->speed = speed;
}

void GamModule::set_minCellVoltage(uint16_t voltage)
{
	hott_gam_msg->min_cell_volt = byte(round(voltage / 0.02));
}

void GamModule::set_minCellVoltageNumber(byte cell)
{
	hott_gam_msg->min_cell_volt_num = cell;
}

void GamModule::set_rpm2(uint16_t rpm)
{
	hott_gam_msg->rpm = rpm / 10;
}

void GamModule::set_pressure_in_bar(float pressure)
{
	hott_gam_msg->pressure_in_bar = byte(pressure / 0.1);
}

void GamModule::set_InvAlarm_allCellvoltage()
{
	
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x1;
}

void GamModule::set_InvAlarm_Bat1()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x2;
}

void GamModule::set_InvAlarm_Bat2()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x4;
}

void GamModule::set_InvAlarm_Temp1()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x8;
}

void GamModule::set_InvAlarm_Temp2()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x10;
}

void GamModule::set_InvAlarm_Fuel()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x20;
}

void GamModule::set_InvAlarm_Rpm()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x40;
}

void GamModule::set_InvAlarm_Altitude()
{
	hott_gam_msg->alarm_invers1 = hott_gam_msg->alarm_invers1 | 0x80;
}

void GamModule::set_InvAlarm_mainPowerCurrent()
{
	hott_gam_msg->alarm_invers2 = hott_gam_msg->alarm_invers2 | 0x1;
}

void GamModule::set_InvAlarm_mainPowerVoltage()
{
	hott_gam_msg->alarm_invers2 = hott_gam_msg->alarm_invers2 | 0x2;
}

void GamModule::set_InvAlarm_climb()
{
	hott_gam_msg->alarm_invers2 = hott_gam_msg->alarm_invers2 | 0x8;
}

void GamModule::set_InvAlarm_climb3()
{
	hott_gam_msg->alarm_invers2 = hott_gam_msg->alarm_invers2 | 0x10;
}

void GamModule::clear_InvAlarm()
{
	hott_gam_msg->alarm_invers1 = 0;
	hott_gam_msg->alarm_invers2 = 0;
}

GamModule::GamModule()
{
	init_BinMsg();
}

void GamModule::setAlarmHandler(HottMessage * handler)
{
	AlertHandler = handler;
}

void GamModule::createMessage()
{
	//init_msg();
	if (dummyMessage)
	{
		
		AlertHandler->set_Alarm(ALARM_OFF);
		if(millis()- timeLastMessageSend >=1000)
		{
			if (!invAlert) {
				set_InvAlarm_allCellvoltage();
				set_InvAlarm_Altitude();
				set_InvAlarm_Bat1();
				set_InvAlarm_Bat2();
				set_InvAlarm_Fuel();
				set_InvAlarm_Rpm();
				set_InvAlarm_Temp1();
				set_InvAlarm_Temp2();
				set_InvAlarm_mainPowerCurrent();
				set_InvAlarm_mainPowerVoltage();
				set_InvAlarm_climb();
				set_InvAlarm_climb3();
				invAlert = !invAlert;
			}
			else
			{
				clear_InvAlarm();
				invAlert = !invAlert;
			}
			timeLastMessageSend = millis();
		}

		for (int i = 0; i <= 5; i++)
		{
			set_cellVotlage(i, 4.2);
		}
		set_Battery1(8.4);
		set_Battery2(8.4);
		set_temperature1(50);
		set_temperature2(100);
		set_fuelPercent(80);
		set_fuelMl(1111);
		set_rpm2(30000);
		set_altitude(121);
		set_climbrateL(1.1);
		set_climbrate3s(200);
		set_current(45);
		set_mainVoltage(11.6);
		set_battCap(1210);
		set_speed(210);
		set_minCellVoltage(3.7);
		set_minCellVoltageNumber(3);
		set_pressure_in_bar(10);
	}
	else
	{
	}
}

int GamModule::getBinMessageSize()
{
	return sizeof(HOTT_GAM_MSG);
}

