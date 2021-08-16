#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <SPI.h>
#include <Wire.h>
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
String serverName="https://unspoilt-compounds.000webhostapp.com/telepost.php?api_key=jdhvbgsdjvhsdbcvjyegfdmsdcvh1278ewqndb23dbu";

#define CHAT_ID "1824993117" 
#define CHAT_ID1 "876922029"
#define BMP_SCK (13); 
#define BMP_MISO (12); 
#define BMP_MOSI (11); 
#define BMP_CS (10);

WiFiClientSecure client; 
UniversalTelegramBot bot(BOTtoken,client); 

int Reqdelay=1000,c=1; 
unsigned long prevMillis; 
char a[10];
Adafruit_BMP280 bmp; 

String getReadings()
{
  float t,m; 
  t = bmp.readTemperature();
  m=bmp.readPressure()/100;
  return "&t="+String(t) +"&p="+String(m);
}  

void handler(int count)
{
  Serial.println("Handling Messages"); 
  Serial.println(String(count)); 
  for(int i=0;i<count;i++)
  {
    String id=String(bot.messages[i].chat_id); 
    if(id!=CHAT_ID && id!=CHAT_ID1)
    {
      bot.sendMessage(id,"Unauthorised user",""); 
      continue;
    }  
    String msg=bot.messages[i].text; 
    //msg.toCharArray(array,7);
    Serial.println(msg); 
    String sender=bot.messages[i].from_name; 
    if(msg=="/start"){  
      String welcome="Welcome, "+sender+"\n Commands are:\n /stat \n/_on_ \n/_off \n/push \nFollow up the command with = and GPIO no.\n available pins are 4,15,34,35,25,14,12,13";
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
      bot.sendMessage(id,"units are deg celsius and hPa","");
    } 
    if(msg.startsWith("/push"))
    {
      digitalWrite(atoi(smsg[1]),HIGH); 
      delay(500); 
      digitalWrite(atoi(smsg[1]),LOW);
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
  if (!bmp.begin(0x76)) {
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
  //bot.sendMessage(CHAT_ID,"HELLO"," ");
  if(millis()>=Reqdelay+prevMillis)
  {
    if(d)
  {
    bot.sendMessage(CHAT_ID,"Motion detected!!"," ");
    bot.sendMessage(CHAT_ID1,"Motion detected!!"," ");
    d=false;
  } 
    int msgcount=bot.getUpdates(bot.last_message_received+1); 
    while(msgcount)
    {
      Serial.print("Message Received"); 
      handler(msgcount); 
      msgcount=bot.getUpdates(bot.last_message_received+1);
    }
    if(c==60)
    {
    WiFiClient client;
    HTTPClient http; 
    //http.begin("https://interscholastic-rec.000webhostapp.com/telepost.php"); 
    //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    serverName+=getReadings();
      http.begin(serverName.c_str());
      int httpResponseCode = http.GET(); 
      Serial.println(httpResponseCode);
      c=1;
    }
    prevMillis=millis(); 
    c+=1;
  } 
  
  
}
