#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include "digits.h"

#include "prese.h"
#include "NotoSansMonoSCB20.h"
#include "Latin_Hiragana_24.h"
#include "Final_Frontier_28.h"

#define latin Latin_Hiragana_24
#define final Final_Frontier_28
#define c1 0x528A //dark gray
#define c2 0xA1C7 //dark red
#define c3 0x22A4 //dark green
#define c4 0xE71C

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

//EDIT THIS..........................................
const char* ssid = "xxxxxxx";   //name of your wifi network
const char* password = "xxxxxxx";    //your wifi password
int yourTimeZone=1;     //edit this number
unsigned long event=1671926401;  //this is christmas epoch time
//......................................................

int fromTop=68;
int positions[9]={10,40,70,110,140,180,210,250,280};
int numbers[9]={0};

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

unsigned long c[4]={86400,3600,60,1};
long now[4];
String nowString[4];


long rest=0;
String d,h,m,s;
byte ts,tm,th,td=0;
bool started;
byte f=0;

String now1,now2;
String date;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void formatData()
{
 epochTime = getTime();
  epochTime=epochTime+(yourTimeZone*3600);
  unsigned long ti=event-epochTime;
  int days=ti/86400;
  rest=ti-(days*86400);
  int hours=rest/3600;
  rest=rest-(hours*3600);
  int minutes=rest/60;
  int sec=rest-(minutes*60);
  
  if(sec<10) s="0"+String(sec); else s=String(sec);
  if(minutes<10) m="0"+String(minutes); else m=String(minutes);
  if(days<100 && days>9) d="0"+String(days);else if(days<10) d="00"+String(days); else d=String(days);
  if(hours<10) h="0"+String(hours); else h=String(hours);

  numbers[0]=d.substring(0,1).toInt();
  numbers[1]=d.substring(1,2).toInt();
  numbers[2]=d.substring(2,3).toInt();
  numbers[3]=h.substring(0,1).toInt();
  numbers[4]=h.substring(1,2).toInt();
  numbers[5]=m.substring(0,1).toInt();
  numbers[6]=m.substring(1,2).toInt();
  numbers[7]=s.substring(0,1).toInt();
  numbers[8]=s.substring(1,2).toInt();

}


void setup() {
  
  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(1);
  sprite.createSprite(320,170);
  sprite.setSwapBytes(true);
  
  initWiFi();
  configTime(0, 0, ntpServer);
  
}



void drawSprite()
{
  formatData();
  sprite.fillSprite(TFT_WHITE);
  sprite.setTextColor(c1,TFT_WHITE);
  sprite.loadFont(NotoSansMonoSCB20);
  sprite.setTextDatum(4);

  sprite.pushImage(262,6,38,48,prese);

  for(int i=0;i<9;i++)
  {
  sprite.pushImage(positions[i],fromTop,30,53,digits[numbers[i]]);
  }


sprite.drawString("DAYS",55, fromTop+72);
sprite.drawString("HOUR",140, fromTop+72);
sprite.drawString("MIN",210, fromTop+72);
sprite.drawString("SEC",280, fromTop+72);

sprite.setTextDatum(0);
sprite.setTextColor(c3,TFT_WHITE);
sprite.drawString("TIME UNTIL",10,10);


sprite.unloadFont();
sprite.loadFont(latin);
sprite.setTextColor(c1,TFT_WHITE);
sprite.drawString("CHRISTMAS",10, 32);
sprite.unloadFont();

sprite.fillSmoothCircle(302,14,6,c2,TFT_WHITE);

sprite.fillTriangle(136,10,170,50,250,50,c4);
sprite.fillTriangle(214,10,136,10,250,50,c4);

  sprite.pushSprite(0,0);
}




void loop() {
  drawSprite();
  delay(980);
}


