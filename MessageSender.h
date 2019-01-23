#pragma once

/*
HottOpenTelemetry
 */

#ifndef MessageSender_h__
#define MessageSender_h__


#include "Arduino.h"
//#include <EEPROM.h>
#include "HottModule.h"

// Radio keys
#define HOTT_KEY_RIGHT  0xE	//ENTER
#define HOTT_KEY_DOWN   0xB	//DEC
#define HOTT_KEY_UP     0xD	//INC
#define HOTT_KEY_SET    0x9 //SET
#define HOTT_KEY_LEFT   0x7	//ESC

// Hott protocol v4 delay
#define HOTTV4_TX_DELAY 650

#define HOTT_BINARY_MODE_REQUEST_ID		0x80
#define HOTT_TEXT_MODE_REQUEST_ID       0x7F
//Binary Mode Requests 
#define HOTT_TELEMETRY_GEA_SENSOR_ID    0x8E // Electric Air Module ID
#define HOTT_TELEMETRY_GAM_SENSOR_ID    0x8D
#define HOTT_TELEMETRY_GPS_SENSOR_ID    0x8A
#define HOTT_TELEMETRY_VARIO_SENSOR_ID  0x89

#define HOTT_GAM_SENSOR_TEXT_ID			0xD
#define HOTT_EAM_SENSOR_TEXT_ID			0xE
#define HOTT_VARIO_SENSOR_TEXT_ID		0x9	
#define HOTT_GPS_SENSOR_TEXT_ID			0xA 

//Graupner #33601 Vario Module






// TEXT MODE STRUCTURE
//struct HOTT_TEXTMODE_MSG {
//  char start_byte;			//#01 Starting constant value == 0x7b
//  char esc;				//#02 Escape (higher-ranking menu in text mode or Text mode leave)
//						//0x00 to stay normal
//						//0x01 to exit
//						//I will send 2 times, so the ESCAPE works really well, so two data frames with 0x01 in char 2
//  char warning_beeps;			//#03 1=A 2=B ...
//  char text[8][21];			//#04...#171 168 ASCII text to display to
//						// Bit 7 = 1 -> Inverse character display
//						// Display 21x8
//  char stop_byte;		        //#172 constant value 0x7d
//  char parity;				//#173 Checksum / parity
//};


class HottMessage {

private:

	Stream *HottSerialPort;
	GamModule *gamModule;

	  //void send_text_msg();
	  char * _hott_invert_all_chars(char *str);
	  char * _hott_invert_chars(char *str, int cnt);
	  void send(int* serialBuffer, int lenght);
	  void _hott_invert_ligne(int ligne);


public:
  HottMessage();
  HottMessage(Stream *serial);
  void setHottSerialPort(Stream *serial);
  void setGamModule(GamModule *module);
  void init();
  void sendMessage();
//  void debug();
  
};





#endif // MessageSender_h__