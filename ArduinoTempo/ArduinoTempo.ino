  
#include <MD_DS1307.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Wire.h>

#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8 //mudar isto para testar metade.

#define CLK_PIN_0   4 // 13  // or SCK
#define DATA_PIN_0  3 // 11  // or MOSI
#define CS_PIN_0    2 // 10  // or SS

#define CLK_PIN_1   7
#define DATA_PIN_1  6
#define CS_PIN_1    5

#define CLK_PIN_2  10
#define DATA_PIN_2  9
#define CS_PIN_2    8

#define SPEED_TIME  80
#define PAUSE_TIME  10
#define MAX_MESG  20


MD_Parola row0 = MD_Parola(HARDWARE_TYPE, DATA_PIN_0, CLK_PIN_0, CS_PIN_0, MAX_DEVICES);
MD_Parola row1 = MD_Parola(HARDWARE_TYPE, DATA_PIN_1, CLK_PIN_1, CS_PIN_1, MAX_DEVICES);
MD_Parola row2 = MD_Parola(HARDWARE_TYPE, DATA_PIN_2, CLK_PIN_2, CS_PIN_2, MAX_DEVICES);

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  75
char strTIME[BUF_SIZE]= "00:00:00";
//char strTIMEsmall1[BUF_SIZE]= "00 HOURS";
//char strTIMEsmall2[BUF_SIZE]= "00 MINUTES 00 DAYS";
char strDATE[BUF_SIZE]= "0 YEARS 0 DAYS";

void setup()
{
  Serial.begin(57600);
  
  row0.begin();
  row1.begin();
  row2.begin();

  row0.setFont(BigFontLower); //grande
  row1.setFont(BigFontUpper); //grande
  //row0.setFont(NULL);
  //row1.setFont(NULL);
  row2.setFont(NULL);
  
  
  row2.displayText(strDATE, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  row1.displayText(strTIME, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT); //grande
  row0.displayText(strTIME, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT); //grande
  //row1.displayText(strTIMEsmall1, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  //row0.displayText(strTIMEsmall2, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  
  if (!RTC.isRunning())
    RTC.control(DS1307_CLOCK_HALT, DS1307_OFF);
    
  RTC.control(DS1307_12H, DS1307_OFF);
    RTC.readTime();
    
}


int HRSLEFT,MINSLEFT,SECSLEFT;
int YEARSLEFT, DAYSLEFT;

struct date {
  int year,day,hour,minute,sec;
};
//http://prntscr.com/13331o6 (Em 18:28 18/05/2021)
struct date limitdate = {
  //inicio esta a 2000,3,0,0,0
  //então tive que adicionar o quanto falta para obter o que é apresentado
  .year = 2006, //2006-2000
  .day = 230,//227+3
  .hour = 23,
  .minute = 59, 
  .sec = 59,
};
  
void loop()
{
  static uint32_t lastTime = 0; // millis() memory
  static uint8_t  display = 0;  // current display mode
  
  row0.displayAnimate();
  row1.displayAnimate();
  row2.displayAnimate();

  
  struct date currentdate = {
    .year = RTC.yyyy,
    .day = RTC.dd,
    .hour = RTC.h,
    .minute = RTC.m, 
    .sec = RTC.s,
    };
  
  
  
  
  // Finally, adjust the time string if we have to
  if (millis() - lastTime >= 1000)
  {
    RTC.readTime();
    lastTime = millis();
    
    YEARSLEFT = abs(limitdate.year - currentdate.year);
    DAYSLEFT = limitdate.day - currentdate.day;
    HRSLEFT = limitdate.hour - currentdate.hour;
    MINSLEFT = limitdate.minute - currentdate.minute;
    SECSLEFT = limitdate.sec - currentdate.sec;


    //sprintf(strTIMEsmall1, "%02d HOURS",HRSLEFT%24);
    //sprintf(strTIMEsmall2, "%02d M %02d S",MINSLEFT,SECSLEFT);

    sprintf(strTIME, "%02d:%02d:%02d",HRSLEFT%24,MINSLEFT,SECSLEFT); //grande.
    Serial.println(strTIME);
    //sprintf(strDATE, "%d YEARS %d DAYS",YEARSLEFT,DAYSLEFT%365); //grande.
    sprintf(strDATE, "%d Y %d DAYS",YEARSLEFT,DAYSLEFT%365);
    row0.displayReset();
    row1.displayReset();
    row2.displayReset();
  }
}
