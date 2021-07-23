#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
const char* ssid="Arunkumar M"; 
const char* pwd="arun_2002";

char array[7];
char *smsg[2]; 
char *ptr=NULL; 

const int ms=27; 

bool d=false;
#define BOTtoken "1848766361:AAEhUBkZ99IAhFXE5Rv1ETQVVdsUVHNf5QE"

#define CHAT_ID "1824993117"

WiFiClientSecure client; 
UniversalTelegramBot bot(BOTtoken,client); 

int Reqdelay=1000; 
unsigned long prevMillis; 
char a[10];
Adafruit_BMP280 bmp; 

String getReadings()
{
  float t,m; 
  t = bmp.readTemperature();
  m=bmp.readPressure()/100;
  return "Temperature: " + String(t) + " ºC \nPressure: " +String(m)+"hPa";
}  

void handler(int count)
{
  Serial.println("Handling Messages"); 
  Serial.println(String(count)); 
  for(int i=0;i<count;i++)
  {
    String id=String(bot.messages[i].chat_id); 
    if(id!=CHAT_ID)
    {
      bot.sendMessage(id,"Unauthorised user",""); 
      continue;
    }  
    String msg=bot.messages[i].text; 
    //msg.toCharArray(array,7);
    Serial.println(msg); 
    String sender=bot.messages[i].from_name; 
    if(msg=="/start"){  
      String welcome="Welcome, "+sender+"\n Commands are:\n /stat \n/_on_ \n/_off \nFollow up the command with = and GPIO no.\n available pins are 4,15,34,35,25,14,12,13";
    bot.sendMessage(id, welcome, ""); }
    
    strcpy(array,msg.c_str());
    ptr=strtok(array,"="); 
    smsg[0]=ptr; 
    ptr=strtok(NULL,"=");
    if(ptr!=NULL){smsg[1]=ptr;} 
    
    
    Serial.println(smsg[0]); 
    Serial.println(smsg[1]);
    if(msg.startsWith("/stat"))
    {
       if(digitalRead(atoi(smsg[1]))){bot.sendMessage(id,"ON","");} 
       else{bot.sendMessage(id,"OFF","");}
    } 
    if(msg.startsWith("/_on_"))
    {
      Serial.println(smsg[0]); 
      digitalWrite(atoi(smsg[1]),HIGH);
    } 
    if(msg.startsWith("/_off"))
    {
      digitalWrite(atoi(smsg[1]),LOW);
    } 
    if(msg.startsWith("/read"))
    {
      bot.sendMessage(id,getReadings(),"");
    }
}} 

void IRAM_ATTR detectsMovement() 
{d= true;}





void setup() {
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid,pwd); 
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(1000); 
    Serial.print("."); 
  } 
  Serial.println("Connected to: "); 
  Serial.println(WiFi.localIP());
  pinMode(ms,INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(ms), detectsMovement, RISING); 
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT); 
  pinMode(15,OUTPUT); 
  pinMode(34,OUTPUT);
  pinMode(35,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(14,OUTPUT); 
  pinMode(12,OUTPUT); 
  pinMode(13,OUTPUT);
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}





void loop() {
  bot.sendMessage(CHAT_ID,"HELLO"," ");
  if(millis()>=Reqdelay+prevMillis)
  {
    if(d)
  {
    bot.sendMessage(CHAT_ID,"Motion detected!!"," "); 
    d=false;
  } 
    int msgcount=bot.getUpdates(bot.last_message_received+1); 
    while(msgcount)
    {
      Serial.print("Message Received"); 
      handler(msgcount); 
      msgcount=bot.getUpdates(bot.last_message_received+1);
    } 
    prevMillis=millis();
  } 
  
  
}
