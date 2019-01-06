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
struct HOTT_GAM_MSG       *hott_gam_msg = (struct HOTT_GAM_MSG *)&_hott_serial_buffer[0];
struct HOTT_TEXTMODE_MSG  *hott_txt_msg = (struct HOTT_TEXTMODE_MSG *)&_hott_serial_buffer[0];
struct HOTT_GPS_MSG       *hott_gps_msg = (struct HOTT_GPS_MSG *)&_hott_serial_buffer[0];
struct HOTT_VARIO_MSG     *hott_vario_msg = (struct HOTT_VARIO_MSG *)&_hott_serial_buffer[0];

static uint16_t currentTime = 0 ;
static uint16_t previousTime = 0 ;

///// ALARM /////
// For lipo
int alarm_on_off_batt1 = 1;       // 0=FALSE/Disable 1=TRUE/Enable   // Enable alarm by default for safety
int alarm_on_off_gps1 = 1;        // 0=FALSE/Disable 1=TRUE/Enable   // Enable alarm by default for safety
int alarm_on_off_pressure1 = 1;   // 0=FALSE/Disable 1=TRUE/Enable   // Enable alarm by default for safety
int init_on_off_pressure1 = 0;    // 0=FALSE/Disable 1=TRUE/Enable   // Disable init by default

// for pressure sensor
int calibration_ok = 0;           // 0=FALSE/Disable 1=TRUE/Enable   // Disable calibration by default
int start_calibration = 0;        // 0=FALSE/Disable 1=TRUE/Enable   // Disable calibration by default
char* alarm_on_off_batt[11];      // For Radio OSD
char* alarm_on_off_pressure[11];  // For Radio OSD
char* alarm_on_off_gps[11];  // For Radio OSD
char* init_on_off_pressure[11];   // For Radio OSD
char* calibration_ok_osd[10];       // For Radio OSD
static uint16_t alarm_lipo = 360; // Min volt for alarm in mV
static uint16_t alarm_pressure = 100; // 100km/h


// Timer for alarm GENERAL MODULE
// for refresh time
int alarm_interval = 15000; // in ms
static unsigned long lastTime=0;  // in ms
unsigned long time;      // in ms

// Timer for alarm GPS MODULE
// for refresh time
int alarm_interval_gps = 10000; // in ms
static unsigned long lastTime_gps=0;  // in ms
unsigned long time_gps;      // in ms

// Timer for alarm VARIO MODULE
// for refresh time
int alarm_interval_pressure = 10000; // in ms
static unsigned long lastTime_pressure=0;  // in ms
unsigned long time_pressure;      // in ms

// For communication
static uint8_t octet1 = 0;  // reception
static uint8_t octet2 = 0;  // reception
boolean mode_text = 0;

// For Lipo
static uint8_t nb_Lipo = 4; // Nb d'éléments accu Lipo par défaut avant détection
float  Lipo_total = 0;      // Tension totale mesurée
float  lipo1 = 0.0;         // Tension sur chacun des éléments
float  lipo2 = 0.0;
float  lipo3 = 0.0;
float  lipo4 = 0.0; 
float  lipo5 = 0.0; 
float  lipo_mini_bat1 = 0.0; // Valeur tension mini element
uint8_t Jauge = 0;          // Jauge carburant

// For saving settings in EEPROM
/*
 !WARNING!
 Writing takes 3.3ms.
 Maximum life of the EEPROM is 100000 writings/readings.
 Be careful not to use it too much, it is not replacable!
 */
#define adr_eprom_test                    0    // For the test for 1st time init of the Arduino (First power on)
#define adr_eprom_alarm_lipo              2    // Default alarm min is 3.60v
#define adr_eprom_alarm_on_off_batt1      4    // 0=FALSE/Disable 1=TRUE/Enable
#define adr_eprom_alarm_interval          6    // Audio warning alarm interval for GENERAL MODULE
#define adr_eprom_alarm_on_off_gps1       8   // 0=FALSE/Disable 1=TRUE/Enable
#define adr_eprom_alarm_interval_gps      10   // Audio warning alarm interval for GPS MODULE
#define adr_eprom_alarm_on_off_pressure1  12
#define adr_eprom_alarm_interval_pressure 14   // Audio warning alarm interval for GPS MODULE
#define adr_eprom_alarm_pressure          16   // Default alarm min is 100km/h
#define adr_eprom_start_calibration       18   // 0=FALSE/Disable 1=TRUE/Enable
#define adr_eprom_calibration_ok          20   // 0=FALSE/Disable 1=TRUE/Enable
#define adr_eprom_pressure_sensor_min     22   //
#define adr_eprom_pressure_sensor_max     24   //

/*GPS variables*/
int gps_fix=-1;
double lat=0;
uint8_t lat_D=0;        // latitude Degree
uint16_t lat_M=0;       // latitude Minutes
uint8_t lat_home_D=0;   // latitude Degree
uint16_t lat_home_M=0;  // latitude Minutes
uint16_t lat_home_S=0;  // latitude Seconds
uint16_t lat_hott_M=0;  // latitude Minutes for Hott Graupner
uint16_t lat_hott_S=0;  // latitude Seconds for Hott Graupner

double   lon=0;
uint8_t  lon_D=0;
uint16_t lon_M=0;
uint8_t  lon_home_D=0;
uint16_t lon_home_M=0;
uint16_t lon_home_S=0;
uint16_t lon_hott_M=0;
uint16_t lon_hott_S=0;

int16_t gps_alt_m=0;
short   gps_hdop_cm=0;
short   gps_vdop_cm=0;
float   gps_speed_kmh=0;
float   gps_speed_max=0;
float   gps_heading_d=90.0;
double  gps_cog=0;
double  gps_climb_speed=0;
uint8_t gps_numsats=0;
uint8_t gps_year=00;
uint8_t gps_month=00;
uint8_t gps_day=00;
//uint8_t gps_hour=00;   // hours given by the GPS Naza Lite are false so can't use
//uint8_t gps_minute=00; // hours given by the GPS Naza Lite are false so can't use
//uint8_t gps_second=00; // hours given by the GPS Naza Lite are false so can't use
uint16_t altitude_table[11];
long    home_lat;
long    home_lon;
int16_t  home_altitude = 0;
int16_t  gps_alt_min   = 999;
int16_t  gps_alt_max   = 0;
uint32_t gps_dist_max  = 0;

bool is_set_home = 0;

//Gimbal position 
float pitch_rad=0;
float roll_rad=0;

// state pressure sensor
int b=0;
static uint16_t pressure_sensor_min = 0;    // Min pressure sensor by default (this value can be changed by the Radio OSD)
static uint16_t pressure_sensor_max = 2000; // Max pressure sensor by default (this value can be changed by the Radio OSD)


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
         
         
        /*case HOTT_TELEMETRY_GEA_SENSOR_ID: //0x89
          {  
			  LEDPIN_ON
			  //init_gea_msg();

			  //send(sizeof(struct HOTT_GEA_MSG));
			  LEDPIN_OFF
		  break;
          }
         */
        
        case HOTT_TELEMETRY_GAM_SENSOR_ID: //0x8D
          {
			gamModule->createMessage();
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
      //  uint8_t  octet3 = HottSerialPort.read();
      //  byte id_sensor = (octet3 >> 4);
      //  byte id_key = octet3 & 0x0f;
      //  static byte ligne_select = 4 ;
      //  static int8_t ligne_edit = -1 ;
      //  hott_txt_msg->start_byte = 0x7b;
      //  hott_txt_msg->esc = 0;
      //  hott_txt_msg->warning_beeps = 0;
      //  
      //  memset((char *)&hott_txt_msg->text, 0x20, HOTT_TEXTMODE_MSG_TEXT_LEN);
      //  hott_txt_msg->stop_byte = 0x7d;

      //  if (id_key == HOTT_KEY_LEFT && page_settings == 1)
      //  {   
      //    hott_txt_msg->esc = 0x01;
      //  }
      //  else
      //  {
      //    
      //    
      //    if (id_sensor == (HOTT_TELEMETRY_GAM_SENSOR_ID & 0x0f)) 
      //    {
      //      if (id_key == HOTT_KEY_RIGHT && ligne_edit == -1)
      //                {
      //                  if (page_settings >=1)// change it if you want more pages
      //                    page_settings = 1;
      //                  else
      //                    page_settings+=1;
      //                }
      //        if (id_key == HOTT_KEY_LEFT && ligne_edit == -1)
      //                  {
      //                  if (page_settings <=1)
      //                    page_settings = 1;
      //                  else
      //                    page_settings-=1;
      //                  } 
      //      switch (page_settings) { //SETTINGS
      //              
      //                  
      //                  
      //        case 1://PAGE 1 SETTINGS
      //        
      //              {
      //              // test si alarme active pour affichage ON ou OFF à l'écran
      //              if (alarm_on_off_batt1 == 0)
      //                *alarm_on_off_batt = " Alarm:OFF";
      //                else
      //                *alarm_on_off_batt = " Alarm: ON";
      //                                     
      //              if (id_key == HOTT_KEY_UP && ligne_edit == -1)
      //              ligne_select = min(6,ligne_select+1); // never gets above line 6 max
      //              else if (id_key == HOTT_KEY_DOWN && ligne_edit == -1)
      //              ligne_select = max(4,ligne_select-1); // never gets above line 4 min
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == -1)
      //              ligne_edit =  ligne_select ;
  
      //              //LIGNE 4 SELECTIONNEE = text[4]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 4 )
      //                {
      //                  alarm_on_off_batt1 = 1;
      //                  *alarm_on_off_batt = " Alarm: ON";
      //                }
      //                
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 4 )
      //                {
      //                  alarm_on_off_batt1 = 0;
      //                  *alarm_on_off_batt = " Alarm:OFF";
      //                 }
      //                
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 4)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_on_off_batt1,alarm_on_off_batt1);
      //                 }
      //              
      //              
      //              //LIGNE 5 SELECTIONNEE = text[5]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 5 )
      //                alarm_lipo+=5;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 5 )
      //                alarm_lipo-=5;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 5)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_lipo,alarm_lipo);
      //                 }

      //              else if (alarm_lipo>420) // not over 4.2v
      //                  {
      //                    alarm_lipo=5;
      //                  } 
      //              else if (alarm_lipo<1)  // not behind 0v
      //                  {
      //                 alarm_lipo=420;
      //                  }

      //              //LIGNE 6 SELECTIONNEE = text[6]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 6 )
      //                alarm_interval+=1000;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 6 )
      //                alarm_interval-=1000;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 6)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_interval,alarm_interval);
      //                 }
      //              else if (alarm_interval>60000)
      //              {
      //                 alarm_interval=1000;  
      //              }
      //              else if (alarm_interval<0)
      //                 {alarm_interval=0; }
      //                        
      //              // Affichage de la page 1
      //              
      //              //line 0:
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("   LIPOMETRE v1.1   "));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Lipo Total: %i.%iv"),(int) Lipo_total, ((int) (Lipo_total*100)) % 100);
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Cells : %iS"),nb_Lipo);
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Change settings :"));
      //              //line 4:
      //              snprintf((char *)&hott_txt_msg->text[4],21,*alarm_on_off_batt);
      //              //line 5:
      //              if (((int) (alarm_lipo % 100)) == 5 || ((int) (alarm_lipo % 100)) == 0)// gestion affichage pour mettre un 0 apres la virgule des valeurs X.05v et X.00v car le modulo renvoi 0 ou 5 pour les valeurs types 100,105,200,205,300,305,etc...
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR(" Alarm value : %i.0%i v."),(int) (alarm_lipo/100),(int) (alarm_lipo % 100)); // adding a 0
      //              else // affichage normal
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR(" Alarm value : %i.%i v."),(int) (alarm_lipo/100),(int) (alarm_lipo % 100)); // no need of adding 0
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR(" Alarm repeat: %i s."),(alarm_interval/1000));
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/1"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              
      //              hott_txt_msg->text[ligne_select][0] = '>';
      //              _hott_invert_ligne(ligne_edit);
      //              break;                    
      //              }//END PAGE 1
      //            
      //            default://PAGE 
      //            {
      //              break;
      //            }
      //            
      //            
      //            
      //      }//END SETTINGS

      //    } // END IF
      //    
      //    //********************************************************************************************************************
      //    else if(id_sensor == (HOTT_TELEMETRY_GPS_SENSOR_ID & 0x0f)) {
      //      
      //     if (id_key == HOTT_KEY_RIGHT && ligne_edit == -1)
      //                {
      //                  if (page_settings >=5)// change it if you want more pages
      //                    page_settings = 1;
      //                  else
      //                    page_settings+=1;
      //                }
      //     if (id_key == HOTT_KEY_LEFT && ligne_edit == -1)
      //                  {
      //                  if (page_settings <1)
      //                    page_settings = 5;
      //                  else
      //                    page_settings-=1;
      //                  }
      //      
      //      switch (page_settings) { //SETTINGS
      //        
      //        case 1://PAGE 1 SETTINGS
      //              {
      //              // Affichage de la page 1
      //              //line 0:
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("GPS  DJI  NAZA Lite"));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Current Position:"));
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("N %id %i.%i'"),lat_D,lat_M,lat_hott_S);
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("E %id %i.%i'"),lon_D,lon_M,lon_hott_S);
      //              //line 4:
      //              snprintf_P((char *)&hott_txt_msg->text[4],21,PSTR("Start Position:"));
      //              //line 5: 
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR("N %id %i.%i'"),lat_home_D,lat_home_M,lat_home_S);
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR("E %id %i.%i'"),lon_home_D,lon_home_M,lon_home_S);
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/5"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              break;                    
      //              }//END PAGE 1
      //           
      //            case 2: // PAGE 2
      //              {
      //              // Affichage de la page 2 settings
      //              //line 0:                                  
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("GPS  DJI  NAZA Lite"));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Dist. Current: %im"),(int) calculateDistance());
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Dist. Maximum: %im"),(int) gps_dist_max);
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("COG : %i"),(int) gps_cog);
      //              //line 4:
      //              snprintf_P((char *)&hott_txt_msg->text[4],21,PSTR("Direction    : %i"),(int) gps_heading_d);  
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR("Average Speed: %i"),(int) gps_speed_kmh);
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR("Maximum Speed: %i"),(int) gps_speed_max);
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/5"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              break;
      //              }//END PAGE 2
      //              
      //               
      //              case 3: // PAGE 3
      //              {
      //              // Affichage de la page 3  settings
      //              //line 0:                                  
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("GPS  DJI  NAZA Lite"));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Altitude    : %i"),gps_alt_m);
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Alt. Min.   : %i"),gps_alt_min);
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Alt. Max.   : %i"),gps_alt_max);
      //              //line 4:
      //              snprintf_P((char *)&hott_txt_msg->text[4],21,PSTR("Alt. Ground : %i"),(gps_alt_m - home_altitude));
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR("Diff./Sec   : %i"),(altitude_table[0] - altitude_table[1])*100);
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR("Diff./3Sec  : %i"),(altitude_table[0] - altitude_table[3]));
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/5"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              break;
      //              }//END PAGE 3
      //            
      //              case 4: // PAGE 4
      //              {
      //              // Affichage de la page 4 settings
      //              //line 0:                                  
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("GPS  DJI  NAZA Lite"));
      //              //line 1:
      //              //snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("%i.%i.20%i  %i:%i:%i"),gps_day,gps_month,gps_year,gps_hour,gps_minute,gps_second);
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("%i.%i.20%i"),gps_day,gps_month,gps_year);
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Fix : %i"),gps_fix);
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Sat.: %i"),gps_numsats);
      //              //line 4:
      //              snprintf_P((char *)&hott_txt_msg->text[4],21,PSTR("VDOP: %icm"),gps_hdop_cm);
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR("HDOP: %icm"),gps_vdop_cm);
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR("COG : %i"),gps_cog);
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/5"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              break;
      //              }//END PAGE 4
      //              
      //              
      //              case 5://PAGE 5
      //              {
      //              // test si alarme active pour affichage ON ou OFF à l'écran
      //              if (alarm_on_off_gps1 == 0)
      //                *alarm_on_off_gps = " Alarm:OFF";
      //                else
      //                *alarm_on_off_gps = " Alarm: ON";
      //                
      //              if (id_key == HOTT_KEY_UP && ligne_edit == -1)
      //              ligne_select = min(6,ligne_select+1); // never gets above line 6 max
      //              else if (id_key == HOTT_KEY_DOWN && ligne_edit == -1)
      //              ligne_select = max(4,ligne_select-1); // never gets above line 4 min
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == -1)
      //              ligne_edit =  ligne_select ;

      //              //LIGNE 4 SELECTIONNEE = text[4]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 4 )
      //                {
      //                  alarm_on_off_gps1 = 1;
      //                  *alarm_on_off_gps = " Alarm: ON";
      //                }
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 4 )
      //                {
      //                  alarm_on_off_gps1 = 0;
      //                  *alarm_on_off_gps = " Alarm:OFF";
      //                 }
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 4)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_on_off_gps1,alarm_on_off_gps1);
      //                 }
      //                 
      //              //LIGNE 5 SELECTIONNEE = text[5]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 5 )
      //                alarm_interval_gps+=1000;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 5 )
      //                alarm_interval_gps-=1000;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 5)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_interval_gps,alarm_interval_gps);
      //                 }
      //              else if (alarm_interval_gps>60000)
      //              {alarm_interval_gps=60000;}
      //              else if (alarm_interval_gps<0)
      //              {alarm_interval_gps=0;}
      //              
      //              
      //              // Affichage de la page 5
      //              //line 0:
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("GPS  DJI  NAZA Lite"));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR(""));
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR(""));
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Change settings :"));
      //              //line 4:
      //              snprintf((char *)&hott_txt_msg->text[4],21,*alarm_on_off_gps);
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR(" Alarm repeat: %i s."),(alarm_interval_gps/1000));
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR(""));
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/5"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              //cursor 
      //              hott_txt_msg->text[ligne_select][0] = '>';
      //              _hott_invert_ligne(ligne_edit);
      //              break;                    
      //              }//END PAGE 5
      //              
      //              
      //      }//END SETTINGS   
      //    }
      //    
      //    //********************************************************************************************************************
      //    else if(id_sensor == (HOTT_TELEMETRY_GEA_SENSOR_ID & 0x0f)) {
      //      //snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("EAM sensor module    <"));
      //      snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Nothing here"));
      //    }
      //     
      //    //********************************************************************************************************************
      //    else if(id_sensor == (HOTT_TELEMETRY_VARIO_SENSOR_ID & 0x0f)) {         
      //      
      //      if (id_key == HOTT_KEY_RIGHT && ligne_edit == -1)
      //                {
      //                  if (page_settings >=2)// change it if you want more pages
      //                    page_settings = 1;
      //                  else
      //                    page_settings+=1;
      //                }
      //     if (id_key == HOTT_KEY_LEFT && ligne_edit == -1)
      //                  {
      //                  if (page_settings <=1)
      //                    page_settings = 2;
      //                  else
      //                    page_settings-=1;
      //                  }
      //                  
      //      switch (page_settings) { //SETTINGS
      //     
      //              case 1://PAGE 1 SETTINGS
      //              {
      //               
      //                
      //              // test si alarme active pour affichage ON ou OFF à l'écran
      //              if (alarm_on_off_pressure1 == 0)
      //                *alarm_on_off_pressure = " Alarm:OFF";
      //                else
      //                *alarm_on_off_pressure = " Alarm: ON";
      //                
      //              if (id_key == HOTT_KEY_UP && ligne_edit == -1)
      //              ligne_select = min(6,ligne_select+1); // never gets above line 6 max
      //              else if (id_key == HOTT_KEY_DOWN && ligne_edit == -1)
      //              ligne_select = max(4,ligne_select-1); // never gets above line 4 min
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == -1)
      //              ligne_edit =  ligne_select ;

      //              //LIGNE 4 SELECTIONNEE = text[4]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 4 )
      //                {
      //                  alarm_on_off_pressure1 = 1;
      //                  *alarm_on_off_pressure = " Alarm: ON";
      //                }
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 4 )
      //                {
      //                  alarm_on_off_pressure1 = 0;
      //                  *alarm_on_off_pressure = " Alarm:OFF";
      //                 }
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 4)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_on_off_pressure1,alarm_on_off_pressure1);
      //                 }
      //                 
      //              //LIGNE 5 SELECTIONNEE = text[5]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 5 )
      //                alarm_pressure+=10;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 5 )
      //                alarm_pressure-=10;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 5)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_pressure,alarm_pressure);
      //                 }
      //              else if (alarm_pressure>300)
      //              {alarm_pressure=300;}
      //              else if (alarm_pressure<0)
      //              {alarm_pressure=0; }
      //              
      //              //LIGNE 6 SELECTIONNEE = text[6]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 6 )
      //                alarm_interval_pressure+=1000;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 6 )
      //                alarm_interval_pressure-=1000;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 6)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_alarm_interval_pressure,alarm_interval_pressure);
      //                 }
      //              else if (alarm_interval_pressure>60000)
      //              {alarm_interval_pressure=1000;}
      //              else if (alarm_interval_pressure<0)
      //              {alarm_interval_pressure=0; }

      //              // Affichage de la page 1
      //              //line 0:
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("  PRESSURE SENSOR   "));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Pressure: %d Pa"),Pressure.GetPressure_Pa());
      //              //line 2:
      //              snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Speed: %d km/h"),Pressure.GetAirSpeed());
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Change settings :"));
      //              //line 4:
      //              snprintf((char *)&hott_txt_msg->text[4],21,*alarm_on_off_pressure);
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR(" Alarm at: %d km/h"),alarm_pressure);
      //              //line 6:
      //              snprintf_P((char *)&hott_txt_msg->text[6],21,PSTR(" Alarm repeat: %i s."),(alarm_interval_pressure/1000));
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/2"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              //cursor 
      //              hott_txt_msg->text[ligne_select][0] = '>';
      //              _hott_invert_ligne(ligne_edit);
      //              break;                    
      //              }//END PAGE 1

      //              case 2://PAGE 2 SETTINGS
      //              {
      //              // test si calibration active pour affichage YES ou NO à l'écran
      //              if (calibration_ok == 1)
      //                *init_on_off_pressure = " Do calib.?:YES";
      //                else
      //                *init_on_off_pressure = " Do calib.?: NO";

      //              if (id_key == HOTT_KEY_UP && ligne_edit == -1)
      //              ligne_select = min(6,ligne_select+1); // never gets above line 6 max
      //              else if (id_key == HOTT_KEY_DOWN && ligne_edit == -1)
      //              ligne_select = max(4,ligne_select-1); // never gets above line 4 min
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == -1)
      //              ligne_edit =  ligne_select ;

      //              //LIGNE 4 SELECTIONNEE = text[4]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 4 )
      //                pressure_sensor_min+=10;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 4 )
      //                pressure_sensor_min-=10;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 4)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_pressure_sensor_min,pressure_sensor_min);
      //                 }
      //              else if (pressure_sensor_min>6000)
      //              {pressure_sensor_min=6000;}
      //              else if (pressure_sensor_min<0)
      //              {pressure_sensor_min=0; }

      //              //LIGNE 5 SELECTIONNEE = text[5]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 5 )
      //                pressure_sensor_max+=10;
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 5 )
      //                pressure_sensor_max-=10;
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 5)
      //                {
      //                 ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_pressure_sensor_max,pressure_sensor_max);
      //                 }
      //              else if (pressure_sensor_max>6000)
      //              {pressure_sensor_max=6000;}
      //              else if (pressure_sensor_max<0)
      //              {pressure_sensor_max=0;}

      //              //LIGNE 6 SELECTIONNEE = text[6]
      //              else if (id_key == HOTT_KEY_UP && ligne_select == 6 )
      //                {calibration_ok = 0;} 
      //              else if (id_key == HOTT_KEY_DOWN && ligne_select == 6 )
      //                {calibration_ok = 1;}
      //              else if (id_key == HOTT_KEY_SET && ligne_edit == 6)
      //                {ligne_edit = -1 ;
      //                 write_eprom(adr_eprom_calibration_ok,calibration_ok);
      //                 if (calibration_ok == 1) do_pressure_calibration();}

      //              // Affichage de la page 1
      //              //line 0:
      //              snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("  PRESSURE SENSOR   "));
      //              //line 1:
      //              snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Pressure: %d Pa"),Pressure.GetPressure_Pa());
      //              //line 2:
      //              if (calibration_ok == 0)
      //              {snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("No Calibration"));}
      //              else
      //              {snprintf_P((char *)&hott_txt_msg->text[2],21,PSTR("Calibration done %d"),b);}
      //              //line 3:
      //              snprintf_P((char *)&hott_txt_msg->text[3],21,PSTR("Settings :"));
      //              //line 4:
      //              snprintf_P((char *)&hott_txt_msg->text[4],21,PSTR(" MinSensor: %d Pa"),pressure_sensor_min);
      //              //line 5:
      //              snprintf_P((char *)&hott_txt_msg->text[5],21,PSTR(" MaxSensor: %d Pa"),pressure_sensor_max);
      //              //line 6:
      //              snprintf((char *)&hott_txt_msg->text[6],21,*init_on_off_pressure);
      //              //line 7:
      //              snprintf_P((char *)&hott_txt_msg->text[7],21,PSTR("johnlenfr.1s.fr  %d/2"),page_settings); //Affichage numero page courant en bas de l'écran à droite
      //              //cursor
      //              hott_txt_msg->text[ligne_select][0] = '>';
      //              _hott_invert_ligne(ligne_edit);
      //              break;
      //              }//END PAGE 2
      //      }//END SETTINGS
      //    }
      //    else {
      //      // snprintf_P((char *)&hott_txt_msg->text[0],21,PSTR("Unknow sensor module <"));
      //      snprintf_P((char *)&hott_txt_msg->text[1],21,PSTR("Nothing here"));
      //    }
      //  }
      //  _hott_send_text_msg();
        break;
      }
    } // END BINARY CASE
  } // END SWITCH OCTET 1
}






//void HottMessage::_hott_send_text_msg() {
//  for(byte *_hott_msg_ptr = hott_txt_msg->text[0]; _hott_msg_ptr < &hott_txt_msg->stop_byte ; _hott_msg_ptr++){
//    if (*_hott_msg_ptr == 0)
//      *_hott_msg_ptr = 0x20;
//  }  
//  //_hott_send_msg(_hott_serial_buffer, sizeof(struct HOTT_TEXTMODE_MSG));
//  send(sizeof(struct HOTT_TEXTMODE_MSG));
//}


char * HottMessage::_hott_invert_all_chars(char *str) {
  return _hott_invert_chars(str, 0);
}


char * HottMessage::_hott_invert_chars(char *str, int cnt) {
  if(str == 0) return str;
  int len = strlen(str);
  if((len < cnt)  && cnt > 0) len = cnt;
  for(int i=0; i< len; i++) {
    str[i] = (byte)(0x80 + (byte)str[i]);
  }
  return str;
}

void HottMessage::send(uint8_t * serialBuffer, int lenght)
{
	uint8_t sum = 0;
	delay(5);
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



/*
void HottMessage::debug(){
   // FOR DEBUG
    Serial.println(F("------------------------"));
    Serial.print(F("Alt:")); Serial.println(gps_alt_m);
    Serial.print(F("Alt min:")); Serial.println(gps_alt_min);
    Serial.print(F("Alt max:")); Serial.println(gps_alt_max);
    Serial.print(F("Fix:")); Serial.println(gps_fix);
    Serial.print(F("Sat:"));Serial.println(gps_numsats);
    Serial.print(F("heading:"));Serial.println(gps_heading_d);
    Serial.print(F("COG:"));Serial.println(gps_cog);
    Serial.print(F("20"));Serial.print(gps_year);Serial.print("."); Serial.print(gps_month);Serial.print(".");Serial.println(gps_day);
    //Serial.print(gps_hour); Serial.print(F("h"));Serial.print(gps_minute);Serial.print(F("m"));Serial.print(gps_second);Serial.println(F("s UTC"));
    Serial.print(F("Lat naza:")); Serial.println(NazaDecoder.getLat(),7);
    Serial.print(F("Lon naza:")); Serial.println(NazaDecoder.getLon(),7);
    Serial.print(F("Lat Hott")); Serial.print(lat_hott_M);Serial.print(F("'"));Serial.println(lat_hott_S);
    Serial.print(F("Lon Hott")); Serial.print(lon_hott_M);Serial.print(F("'"));Serial.println(lon_hott_S);
    Serial.println(freeRAM());
}

int HottMessage::freeRAM (){
  extern int __heap_start, *__brkval;
  int v;
  int free = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  return free;
}
*/
