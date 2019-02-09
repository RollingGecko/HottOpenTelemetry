/*
HottOpenTelemetry
*/


#include "MessageSender.h"
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#include <Stream.h>


//SoftwareSerial HottSerialPort(8,9); // RX, TX

static uint8_t _hott_serial_buffer[173];   //création d'une variable tampon pour stocker les struct

// pointer des structures vers le tampon "_hott_serial_buffer"
struct HOTT_TEXTMODE_MSG  *hott_txt_msg = (struct HOTT_TEXTMODE_MSG *)&_hott_serial_buffer[0];

// For communication
static uint8_t octet1 = 0;  // reception
static uint8_t octet2 = 0;  // reception
boolean mode_text = 0;

HottMessage::HottMessage():HottSerialPort(NULL), gamModule(NULL){
}

HottMessage::HottMessage(Stream *serial):HottSerialPort(serial), gamModule(NULL)
{
}

void HottMessage::setHottSerialPort(Stream *serial)
{
	HottSerialPort = serial;
}

void HottMessage::setGamModule(GamModule *module)
{
	gamModule = module;
}

void HottMessage::init(){
  
 // HottSerialPort.begin(SERIAL_COM_SPEED); // 19400 FOR GRAUPNER HOTT using SoftSerial lib.
}


void HottMessage::sendMessage(){ 
	
  // STARTING MAIN PROGRAM LIPOMETRE
  static byte page_settings = 1;//numéro de la page de settings à afficher
  
  if(HottSerialPort->available() >= 2) {
    
    uint8_t octet1 = HottSerialPort->read();
    switch (octet1) {
    case HOTT_BINARY_MODE_REQUEST_ID:
      { 
        uint8_t  octet2 = HottSerialPort->read();
        
        // Demande RX Module =	$80 $XX
        switch (octet2) {
        
          case HOTT_TELEMETRY_GPS_SENSOR_ID: //0x8A
          { 
              break;
          }
          
        case HOTT_TELEMETRY_VARIO_SENSOR_ID: //0x89
          {
          break;
          }
                 
        case HOTT_TELEMETRY_GAM_SENSOR_ID: //0x8D
          {
			gamModule->createBinMessage();
            send(gamModule->getBinMessage(), gamModule->getBinMessageSize());
            break;

          } //end case GAM*/
        } //end case octet 2
        break;
      } // end case HOTT_BINARY_MODE_REQUEST_ID
    
    
    // SATARTING TEXT MODE
    case HOTT_TEXT_MODE_REQUEST_ID:
      {
      //  mode_text = 1;
       uint8_t  octet3 = HottSerialPort->read();
		   
	   //Lower octet3 Bits representing Sensor
        byte id_sensor = (octet3 >> 4);
				
		//Higher octet3 Bits representing Key Information
        byte id_key = octet3 & 0x0f;

		gamModule->initTxtMsg();
		//Text mode General  Air Module
		if (id_sensor == HOTT_GAM_SENSOR_TEXT_ID) {
			
			//Serial.println("Inn");
			gamModule->createTxtMessage(id_key);
			send(gamModule->getTxtMessage(), gamModule->getTxtMessageSize() );
		    
		}
				
      
        break;
      }
    } // END BINARY CASE
  } // END SWITCH OCTET 1
}

char * HottMessage::_hott_invert_all_chars(char *str) {
  return _hott_invert_chars(str, 0);
}




void HottMessage::send(uint8_t * serialBuffer, int lenght)
{
	uint8_t sum = 0;
	delay(5);
	/*for (int i = 0; i < lenght - 1; i++) {
		Serial.print("Out: "); Serial.println(serialBuffer[i]);
	}*/
	for (int i = 0; i < lenght - 1; i++) {
		sum = sum + serialBuffer[i];
		HottSerialPort->write(serialBuffer[i]);
		delayMicroseconds(HOTTV4_TX_DELAY);
		//delay(TPS_EM);
		HottSerialPort->read(); // 
	}
	//Emision du checksum
	HottSerialPort->write(sum);
	//delay(TPS_EM);
	delayMicroseconds(HOTTV4_TX_DELAY);
	HottSerialPort->read(); // A cause du rebouclage TX <--> RX Lecture de l'octet émis 
}


void HottMessage::_hott_invert_ligne(int ligne) {
  if (ligne>= 0 && ligne<= 7)
    for(int i=0; i< 21; i++) {
      if (hott_txt_msg->text[ligne][i] == 0)   //inversion du caratère null (fin de string)
        hott_txt_msg->text[ligne][i] = (byte)(0x80 + 0x20);
      else
        hott_txt_msg->text[ligne][i] = (byte)(0x80 + (byte)hott_txt_msg->text[ligne][i]);
    }
}

char * HottMessage::_hott_invert_chars(char *str, int cnt) {
	if (str == 0) return str;
	int len = strlen(str);
	if ((len < cnt) && cnt > 0) len = cnt;
	for (int i = 0; i < len; i++) {
		str[i] = (byte)(0x80 + (byte)str[i]);
	}
	return str;
}

