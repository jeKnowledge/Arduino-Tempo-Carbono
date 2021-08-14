#include "ThreeWire.h" //INCLUSÃO DA BIBLIOTECA
#include "RtcDS1302.h" //INCLUSÃO DA BIBLIOTECA
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

ThreeWire myWire(12, 13, 11); //OBJETO DO TIPO ThreeWire
RtcDS1302<ThreeWire> Rtc(myWire); //OBJETO DO TIPO RtcDS1302

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

  Rtc.Begin(); //INICIALIZA O RTC
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); //VARIÁVEL RECEBE DATA E HORA DE COMPILAÇÃO

  if(Rtc.GetIsWriteProtected()){ //SE O RTC ESTIVER PROTEGIDO CONTRA GRAVAÇÃO, FAZ
        Serial.println("RTC está protegido contra gravação. Habilitando a gravação agora..."); //IMPRIME O TEXTO NO MONITOR SERIAL
        Rtc.SetIsWriteProtected(false); //HABILITA GRAVAÇÃO NO RTC
        Serial.println(); //QUEBRA DE LINHA NA SERIAL
    }
    
  if (!Rtc.GetIsRunning())
    Rtc.SetIsRunning(true); //INICIALIZA O RTC
    
  RtcDateTime now = Rtc.GetDateTime(); //VARIÁVEL RECEBE INFORMAÇÕES
   if (now < compiled) { //SE A INFORMAÇÃO REGISTRADA FOR MENOR QUE A INFORMAÇÃO COMPILADA, FAZ
        Serial.println("As informações atuais do RTC estão desatualizadas. Atualizando informações..."); //IMPRIME O TEXTO NO MONITOR SERIAL
        Rtc.SetDateTime(compiled); //INFORMAÇÕES COMPILADAS SUBSTITUEM AS INFORMAÇÕES ANTERIORES
        Serial.println(); //QUEBRA DE LINHA NA SERIAL
  }
}


int HRSLEFT,MINSLEFT,SECSLEFT;
int YEARSLEFT, DAYSLEFT;

struct date {
  int year,month,day,hour,minute,sec;
};
//http://prntscr.com/13331o6 (Em 18:28 18/05/2021)

struct date limitdate = {
  .year = 2028, 
  .month = 01,
  .day = 01,
  .hour = 12,
  .minute = 00, 
  .sec = 00,
};

#define countof(a) (sizeof(a) / sizeof(a[0]))
  
void loop()
{
  static uint32_t lastTime = 0; // millis() memory
  static uint8_t  display = 0;  // current display mode
  
  row0.displayAnimate();
  row1.displayAnimate();
  row2.displayAnimate();
  
  
  
  // Finally, adjust the time string if we have to
  if (millis() - lastTime >= 1000)
  {
    RtcDateTime now = Rtc.GetDateTime(); //VARIÁVEL RECEBE INFORMAÇÕES
    struct date currentdate = {
    .year = now.Year(),
    .month = now.Month(),
    .day = now.Day(),
    .hour = now.Hour(),
    .minute = now.Minute(), 
    .sec = now.Second(),
    };

    // printDateTime(now); Print Date and Time now
    
    lastTime = millis();
  
    YEARSLEFT = abs(limitdate.year - currentdate.year-1);
    DAYSLEFT = abs(dleft(currentdate.day,currentdate.month,currentdate.year,limitdate.day));  
    HRSLEFT = abs(24- limitdate.hour - currentdate.hour-1)%24;
    MINSLEFT = abs(60-limitdate.minute - currentdate.minute-1)%60;
    SECSLEFT = abs(60-limitdate.sec - currentdate.sec)%60;


    //sprintf(strTIMEsmall1, "%02d HOURS",HRSLEFT%24);
    //sprintf(strTIMEsmall2, "%02d M %02d S",MINSLEFT,SECSLEFT);

    sprintf(strTIME, "%02d:%02d:%02d",HRSLEFT%24,MINSLEFT,SECSLEFT); //grande.
    
    char debug[200];
    sprintf(debug, "y: %d, d: %d", YEARSLEFT, DAYSLEFT);
    
    Serial.println(debug);
    Serial.println(strTIME);
    
    //sprintf(strDATE, "%d YEARS %d DAYS",YEARSLEFT,DAYSLEFT%366); //grande.
    sprintf(strDATE, "%dY %d DAYS",YEARSLEFT,DAYSLEFT%366);
    row0.displayReset();
    row1.displayReset();
    row2.displayReset();
  }
}

void printDateTime(const RtcDateTime& dt){
    char datestring[20]; //VARIÁVEL ARMAZENA AS INFORMAÇÕES DE DATA E HORA
 
    snprintf_P( datestring, 
                countof(datestring),
                PSTR("%02u/%02u/%04u %02u:%02u:%02u"), //FORMATO DE EXIBIÇÃO DAS INFORMAÇÕES
                dt.Day(), //DIA
                dt.Month(), //MÊS
                dt.Year(), //ANO
                dt.Hour(), //HORA
                dt.Minute(), //MINUTOS
                dt.Second()
            ); //SEGUNDOS
    Serial.println(datestring); //IMPRIME NO MONITOR SERIAL AS INFORMAÇÕES
}

int dleft(int day, int month, int year, int limitedateday){
  int sum = 0;
  int days_in_feb = 28;
  int daysinmonth = 0;
  int totaldays=365;
  
  //não interessa verificar ano bissexto porque vou ver os dias restantes à mesma, ou seja, somar 1 e subtrair 1 é a mesma coisa.
  for(int i =1; i<month; i++){
    if(i ==2){
      daysinmonth = days_in_feb;
      /*Serial.println("");
      Serial.println(daysinmonth);*/
      sum += daysinmonth;
      continue; //passar o resto deste loop
    }
    if(i<8){
      if(i%2==1)
        daysinmonth = 31;
      else 
        daysinmonth = 30;
    }
    else {
      if(i%2==0)
        daysinmonth = 31;
      else
        daysinmonth=30;
    }
   /* Serial.println("");
    Serial.println(daysinmonth);*/
    sum += daysinmonth;
  }
  sum += day;
  return totaldays-sum;  
}
