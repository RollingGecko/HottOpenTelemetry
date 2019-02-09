#include "HottModule.h"

#include "math.h"

void HottModule::cleanTxtMessage()
{
	for (byte *_hott_msg_ptr = hott_txt_msg->text[0]; _hott_msg_ptr < &hott_txt_msg->stop_byte; _hott_msg_ptr++) {
		if (*_hott_msg_ptr == 0)
			*_hott_msg_ptr = 0x20;
	}
}

void HottModule::setCurrentPage(Page * _page)
{
	currentPage = _page;
}

HottModule::HottModule()
{

}


HottModule::~HottModule()
{
}

void HottModule::initTxtMsg()
{
	hott_txt_msg->start_byte = 0x7b;
	hott_txt_msg->esc = 0;
	hott_txt_msg->warning_beeps = 0;
	memset((char *)&hott_txt_msg->text, 0x20, HOTT_TEXTMODE_MSG_TEXT_LEN);
	hott_txt_msg->stop_byte = 0x7d;
}


uint8_t* HottModule::getBinMessage()
{
	return serialBinMessage;
}

uint8_t * HottModule::getTxtMessage()
{
	return serialTxtMessage;
}

int HottModule::getTxtMessageSize()
{
	return sizeof(serialTxtMessage);
}

void HottModule::setDummyMessage(bool onOff)
{
	dummyMessage = onOff;
}

void GamModule::initSensors(bool _cellVoltage, bool _battery1, bool _battery2, bool _temp1, bool _temp2
	, bool _fuelPerc, bool _fuelMl, bool _rpm, bool _alt, bool _climbL, bool _climb3s
	, bool _current, bool _voltageMain, bool _batCap, bool _speed, bool _minVolCelNum, bool _rpm2
	, bool _pressure)
{
	if (_cellVoltage)
	{
		cellVoltage = new SensorValue<float>("cellV",this,ALARM_MINCELLVOLTAGE);
	}
	if (_battery1)
	{
		battery1 = new SensorValue<float>("Bat1", this, ALARM_MINSENSVOL1, ALARM_MAXSENSVOL1);
	}
	if (_battery2)
	{
		battery2 = new SensorValue<float>("Bat2", this, ALARM_MINSENSVOL2, ALARM_MAXSENSVOL2);
	}
	if (_temp1)
	{
		temperature1 = new SensorValue<byte>("Temp1", this, ALARM_MINTEMP1, ALARM_MAXTEMP1);
	}
	if (_temp2)
	{
		temperature2 = new SensorValue<byte>("Temp2", this, ALARM_MINTEMP2, ALARM_MAXTEMP2);
	}
	if (_fuelPerc)
	{
		fuelPercentage = new SensorValue<byte>("Fuel%",this);
	}
	if (_fuelMl)
	{
		fuelMl = new SensorValue<uint16_t>("Fuel", this, ALARM_FUELRESERVE);
	}
	if (_rpm)
	{
		rpm = new SensorValue<uint16_t>("RPM", this, ALARM_MINRPM, ALARM_MAXRPM);
	}
	if (_alt)
	{
		altitude = new SensorValue<uint16_t>("Alt", this, ALARM_MINALT, ALARM_MAXALTITUDE);
	}
	if (_climbL)
	{
		climbrate_L = new SensorValue<float>("ClimbL", this, ALARM_OFF, ALARM_CLIMBRATE1);
	}
	if (_climb3s)
	{
		climbrate3s = new SensorValue<uint16_t>("Climb3s", this, ALARM_OFF, ALARM_OFF);
	}
	if (_current)
	{
		current = new SensorValue<float>("Cur", this, ALARM_OFF, ALARM_MAXCURRENT);
	}
	if (_voltageMain)
	{
		voltageMain = new SensorValue<float>("MainVol", this, ALARM_MINPOWVOLTAGE, ALARM_MAXPPOWVOLTAGE);
	}
	if (_batCap)
	{
		batteryCapacity = new SensorValue<uint16_t>("Cap", this, ALARM_OFF, ALARM_MAXUSEDCAPACITY);
	}
	if (_speed)
	{
		speed = new SensorValue<uint16_t>("Speed", this, ALARM_MINSPEED, ALARM_MAXSPEED);
	}
	if (_minVolCelNum)
	{
		minVoltageCellNumber = new SensorValue<byte>("MinVolCel", this, ALARM_OFF, ALARM_OFF);
	}
	if (_rpm2)
	{
		rpm2 = new SensorValue<uint16_t>("RPM2", this, ALARM_MINRPM, ALARM_MAXRPM);
	}
	if (_pressure)
	{
		pressure = new SensorValue<float>("Pressure", this, ALARM_OFF, ALARM_OFF);
	}
}

void GamModule::set_Alert(byte alarm)
{
	hott_gam_msg->warning_beeps = alarm;
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

void GamModule::setBinMsg_cellVotlage(uint8_t cell, float voltage)
{
		hott_gam_msg->cell[cell] = byte(round(voltage / 0.02));
	}

void GamModule::setBinMsg_Battery1(float voltage)
{
	hott_gam_msg->Battery1 = int(round(voltage / 0.1));
}

void GamModule::setBinMsg_Battery2(float voltage)
{
	hott_gam_msg->Battery2 = int(round(voltage / 0.1));
}

byte GamModule::setBinMsg_temp(int temp)
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

void GamModule::setBinMsg_temperature1(int temp)
{
	hott_gam_msg->temperature1 = setBinMsg_temp(temp);
}

void GamModule::setBinMsg_temperature2(int temp)
{
	hott_gam_msg->temperature2 = setBinMsg_temp(temp);
}
void GamModule::setBinMsg_fuelPercent(byte fuelPercent)
{
	hott_gam_msg->fuel_percent = fuelPercent;
}

void GamModule::setBinMsg_fuelMl(uint16_t fuelMl)
{
	hott_gam_msg->fuel_ml = fuelMl;
}

void GamModule::setBinMsg_rpm(uint16_t rpm)
{
	hott_gam_msg->rpm = rpm / 10;
}

void GamModule::setBinMsg_altitude(uint16_t altitude)
{
	hott_gam_msg->altitude = altitude + 500;
}

void GamModule::setBinMsg_climbrateL(float climbrateL)
{
	hott_gam_msg->climbrate_L = long(climbrateL / 0.01) + 30000;
}

void GamModule::setBinMsg_climbrate3s(uint16_t climbrate3s)
{
	hott_gam_msg->climbrate3s = climbrate3s + 120;
}

void GamModule::setBinMsg_current(float current)
{
	hott_gam_msg->current = long(current / 0.1);
}

void GamModule::setBinMsg_mainVoltage(uint16_t voltage)
{
	hott_gam_msg->main_voltage = long(voltage / 0.1);
}

void GamModule::setBinMsg_battCap(uint16_t capacitiy)
{
	hott_gam_msg->batt_cap = capacitiy / 10;
}

void GamModule::setBinMsg_speed(uint16_t speed)
{
	hott_gam_msg->speed = speed;
}

void GamModule::setBinMsg_minCellVoltage(uint16_t voltage)
{
	hott_gam_msg->min_cell_volt = byte(round(voltage / 0.02));
}

void GamModule::setBinMsg_minCellVoltageNumber(byte cell)
{
	hott_gam_msg->min_cell_volt_num = cell;
}

void GamModule::setBinMsg_rpm2(uint16_t rpm)
{
	hott_gam_msg->rpm = rpm / 10;
}

void GamModule::setBinMsg_pressure_in_bar(float pressure)
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

void GamModule::createBinMessage()
{
	//init_msg();
	if (dummyMessage)
	{
		
		set_Alert(ALARM_OFF);
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
			setBinMsg_cellVotlage(i, 4.2);
		}
		setBinMsg_Battery1(4);
		setBinMsg_Battery2(8.4);
		setBinMsg_temperature1(50);
		setBinMsg_temperature2(100);
		setBinMsg_fuelPercent(80);
		setBinMsg_fuelMl(1111);
		setBinMsg_rpm2(30000);
		setBinMsg_altitude(121);
		setBinMsg_climbrateL(1.1);
		setBinMsg_climbrate3s(200);
		setBinMsg_current(45);
		setBinMsg_mainVoltage(11.6);
		setBinMsg_battCap(1210);
		setBinMsg_speed(100);
		setBinMsg_minCellVoltage(3.7);
		setBinMsg_minCellVoltageNumber(3);
		setBinMsg_pressure_in_bar(10);
	}
	else
	{
		if (battery1 != nullptr)
		{
			setBinMsg_Battery1(battery1->getValue());
		}
		if (battery2 != nullptr)
		{
			setBinMsg_Battery2(battery2->getValue());
		}
		if (temperature1 != nullptr)
		{
			setBinMsg_temperature1(temperature1->getValue());
		}
		if (temperature2 != nullptr)
		{
			setBinMsg_temperature2(temperature2->getValue());
		}
		if (fuelPercentage != nullptr)
		{
			setBinMsg_fuelPercent(fuelPercentage->getValue());
		}
		if (fuelMl != nullptr)
		{
			setBinMsg_fuelMl(fuelMl-> getValue());
		}
		if (rpm2 != nullptr)
		{
			setBinMsg_rpm2(rpm2->getValue());
		}
		if (rpm != nullptr)
		{
			setBinMsg_rpm(rpm->getValue());
		}
		if (altitude != nullptr)
		{
			setBinMsg_altitude(altitude->getValue());
		}
		if (climbrate_L != nullptr)
		{
			setBinMsg_climbrateL(climbrate_L->getValue());
		}
		if (climbrate3s != nullptr)
		{
			setBinMsg_climbrate3s(climbrate3s->getValue());
		}
		if (current != nullptr)
		{
			setBinMsg_current(current->getValue());
		}
		if (voltageMain!=nullptr)
		{
			setBinMsg_mainVoltage(voltageMain->getValue());
		}
		if (batteryCapacity!= nullptr)
		{
			setBinMsg_battCap(batteryCapacity->getValue());
		}
		if (speed!=nullptr)
		{
			setBinMsg_speed(speed->getValue());
		}

			setBinMsg_minCellVoltage(3.7);

		if (minVoltageCellNumber!=nullptr)
		{
			setBinMsg_minCellVoltageNumber(minVoltageCellNumber->getValue());
		}
		if (pressure!=nullptr)
		{
			setBinMsg_pressure_in_bar(pressure->getValue());
		}
		
	}
}


void GamModule::createTxtMessage(byte keyId)
{
	/*int number = 12;
	float number_float = 123445.1;*/

	currentPage->callPage(keyId);
	cleanTxtMessage();
}


int GamModule::getBinMessageSize()
{
	return sizeof(HOTT_GAM_MSG);
}

