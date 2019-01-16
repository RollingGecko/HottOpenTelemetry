#pragma once

#ifndef ModuleDefines_h__
#define ModuleDefines_h__

#define HOTT_TELEMETRY_GEA_SENSOR_ID    0x8E // Electric Air Module ID
#define HOTT_BINARY_MODE_REQUEST_ID		0x80
#define HOTT_TEXT_MODE_REQUEST_ID       0x7f
#define HOTT_TELEMETRY_GAM_SENSOR_ID    0x8d

//Alerts

#define ALARM_OFF				0x00//0x00  00  0  No alarm
#define	ALARM_MINSPEED			0x01 //01  A  MinSpeed
#define	ALARM_SINKRATE2			0x02 //02  B  Sink Rate 3s 
#define	ALARM_SINKRATE1			0x03 //03  C  Sink Rate 1s
#define	ALARM_MAXDISTANCE		0x04 //04  D  Maximum distance
#define	ALARM_WEAKTELEMETRY		0x05 //05  E  Weak telemetry
#define	ALARM_MINTEMP1			0x06 //06  F  Min.Sensor 1 temp.F
#define	ALARM_MINTEMP2			0x07 //07  G  Min.Sensor 2 temp.G
#define	ALARM_MAXTEMP1			0x08 //08  H  Max.Sensor 1 temp.H
#define	ALARM_MAXTEMP2			0x09 //09  I  Max.Sensor 2 temp.I
#define	ALARM_MAXSENSVOL1		0xA  //10  J  Max.Sens. 1 voltage J
#define	ALARM_MAXSENSVOL2		0xB  //11  K  Max.Sens. 2 voltage K
#define	ALARM_MAXSPEED			0xC  //12  L  Max. speed
#define	ALARM_CLIMBRATE2		0xD  //13  M  Climb rate 2 M
#define	ALARM_CLIMBRATE1		0xE  //14  N  Climb rate 1 N
#define	ALARM_MINALT			0xF  //15  O  Min.Altitude O
#define	ALARM_MINPOWVOLTAGE		0x10 //16  P  Min.Power Voltage P    // We use this one for Battery Warning
#define	ALARM_MINCELLVOLTAGE	0x11 //17  Q  Min.Cell voltage Q
#define	ALARM_MINSENSVOL1		0x12 //18  R  Min.Sens. 1 voltage R
#define	ALARM_MINSENSVOL2		0x13 //19  S  Min.Sens. 2 voltage S
#define	ALARM_MINRPM			0x14 //20  T  Minimum RPM T
#define	ALARM_FUELRESERVE		0x15 //21  U  Fuel reserve
#define	ALARM_MAXUSEDCAPACITY	0x16 //22  V  Max.used capacity V
#define	ALARM_MAXCURRENT		0x17 //23  W  Max.Current W
#define	ALARM_MAXPPOWVOLTAGE	0x18 //24  X  Max.Power Voltage X
#define	ALARM_MAXRPM			0x19 //25  Y  Maximum RPM Y
#define	ALARM_MAXALTITUDE		0x1A //26  Z  Max.Altitude Z


// Radio keys
#define HOTT_KEY_RIGHT  14	//0xE	ENTER
#define HOTT_KEY_DOWN   11	//0xB	DEC
#define HOTT_KEY_UP     13	//0xD	INC
#define HOTT_KEY_SET    9	//0x9	INC+DEC
#define HOTT_KEY_LEFT   7	//0x7	ESC
#endif // ModuleDefines_h__