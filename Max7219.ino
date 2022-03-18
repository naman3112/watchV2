#include <Timer.h>

// Header file includes
#include <WiFi.h>
#include <time.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <Keypad.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns


char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};



byte pin_rows[ROW_NUM] = {13, 12, 14, 27}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {26, 25, 33};  // GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

#define CLK_PIN   18 // or SCK
#define DATA_PIN  19 // or MOSI
#define CS_PIN    5 // or SS

// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_Parola P1 = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME  75
#define PAUSE_TIME  0
#define MAX_MESG  20

/**********  User Config Setting   ******************************/
char* ssid = "watch12345";
char* password = "watch12345";
//calculate your timezone in seconds,1 hour = 3600 seconds and 5.30Hrs = 19800
const int timezoneinSeconds = 19800;
/***************************************************************/
int dst = 0;
uint16_t  h, m, s;
uint8_t dow;
int  day;
uint8_t month;
String  year;
// Global variables
char szTime[9];    // mm:ss\0
char szsecond[4];    // ss
char szMesg[MAX_MESG+1] = "";

char msgDisp[14];
char TimeInp[14];

bool validity(){
  P1.displayAnimate();
delay(100);
  int h1=0,h2=0,m1=0,m2=0,s1=0,s2=0,hours=0,minutes=0,seconds=0;
   h1=(int)(TimeInp[0]);
   h1=h1-48;
   h2=(int)(TimeInp[1]);
   h2=h2-48;
   m1=(int)(TimeInp[2]);
  m1=m1-48;
   m2=(int)(TimeInp[3]);
   m2=m2-48;
   s1=(int)(TimeInp[4]);
  s1=s1-48;
   s2=(int)(TimeInp[5]);
   s2=s2-48;
hours=h1*10 +h2;
 minutes= m1*10+m2;
 seconds=s1*10+s2;

bool invalid=false;
Serial.println("hours ");
Serial.print(hours);
Serial.println("minutes ");
Serial.println(minutes);
Serial.print("Seconds .  ");
Serial.println(seconds);

invalid=hours<=12 && minutes<60 && seconds<60;
invalid=!invalid;
Serial.println("invalid flag");
Serial.println(invalid);

 if(invalid){
         Serial.println("Here in if ");       
      P1.displayZoneText(0, "Invalid INP", PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      delay(1500);
        return 0;
 }else{
             configTime(timezoneinSeconds, dst, "pool.ntp.org","time.nist.gov");

           Serial.println("Here in else");

        P1.displayZoneText(0, "Valid Time", PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);

             delay(1500);
               

             return 1;
 }


}



void initialSetup(){
  int i=0,len=0;
  while(1){
        P1.displayAnimate();
        
    if(i==1000){
            P1.displayReset(0);
           P1.displayReset(1);
          i=0;
    }
    i++;

      strcpy(msgDisp,"* OKAY");
      P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      delay(2000);      
                  P1.displayReset(0);
           P1.displayReset(1);

        P1.displayAnimate();

      strcpy(msgDisp,"# CANCEL");
      P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      delay(2000);      
                  P1.displayReset(0);
           P1.displayReset(1);
      
        P1.displayAnimate();

      strcpy(msgDisp,"Set Time");
      P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      delay(1500);     
                  P1.displayReset(0);
           P1.displayReset(1); 
        P1.displayAnimate();
int i=0;
delay(1000);
    memset(msgDisp,0,8);

      while(1){
              P1.displayAnimate();
    
      char key = keypad.getKey();
     
if (key) {
  
   if(len<7){
    if(key=='#'){
      if(len!=0){
        msgDisp[len-1]=' ';
        len--;
      }
      
      
    }
    else if(key=='*'){
      strcpy(TimeInp, msgDisp);
                        P1.displayReset(0);
           P1.displayReset(1); 
      validity();
      return;
    }
    
    else{
          msgDisp[len++]=key;
    }
    
   }else{
    len=0;
    memset(msgDisp,0,8);
   }
    Serial.println(key);
        P1.displayReset(0);
    P1.displayReset(1);

      P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      
          Serial.println("msgDisp is below dude");
          Serial.print(msgDisp);
  }


//    if(key=='#'){
//      
//            P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
//                       P1.displayReset(0);
//           P1.displayReset(1); 
//           if(len!=0){
//            len--;
//            msgDisp[len]=' ';
//            len--;
//            msgDisp[len]=' ';
//           }
//           P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
//
//      //return ;
//    }

        
      }


      strcpy(msgDisp,"End Time");
      P1.displayZoneText(0, msgDisp, PA_LEFT, SPEED_TIME,  PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
      delay(1000);  
                  P1.displayReset(0);
           P1.displayReset(1);    



  }
  return;
}

void getsec(char *psz)
{
  sprintf(psz, "%c%02d",':',s);
}

void getTime(char *psz, bool f = true)
{
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
      h = p_tm->tm_hour;
      m = p_tm->tm_min;
      s = p_tm->tm_sec;
  sprintf(psz, "%02d%c%02d", h, (f ? ':' : ' '), m);
  Serial.println(psz);
}

void setup(void)
{
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(3000);
  WiFi.mode(WIFI_STA);
  getTimentp();

  P.begin(3);
  P1.begin(3);
  P.setInvert(false);
  P1.setInvert(false);

  P.setZone(0, 3, 4);
  P.setZone(1, 5, 7);
  P1.setZone(0,3,7);
 //P.setFont(0, numeric7Se);
 // P.setFont(1, numeric7Se);
  P.displayZoneText(0, szsecond, PA_LEFT, SPEED_TIME, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayZoneText(1, szTime, PA_RIGHT, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);

  
  getTime(szTime);
  Serial.println("=---=--=-=--==---=-==-=-=-=-=================== ");
}
int len=0;
int k =0;
void loop(void)
{
  static uint32_t lastTime = 0; // millis() memory
  static uint8_t  display = 0;  // current display mode
  static bool flasher = false;  // seconds passing flasher

    if(k==0){
      k=1;
  initialSetup();    
   
    }
  

  int valid= validity();
    if (valid==1){
        P.displayAnimate();

      if (millis() - lastTime >= 1000)
  {
    lastTime = millis();
    
    
  //  getsec(szsecond);
    //getTime(szTime, flasher);
    flasher = !flasher;

    P.displayReset(0);
   // P1.displayReset(0);

    P.displayReset(1);
   // P1.displayReset(1);

  }
      
    }
    else{
     initialSetup(); 
    }

    



  


}

void getTimentp()
{
  configTime(timezoneinSeconds, dst, "pool.ntp.org","time.nist.gov");
  
  while(!time(nullptr)){
        delay(500);
        Serial.print(".");
  }
  Serial.print("Time Update");
}
