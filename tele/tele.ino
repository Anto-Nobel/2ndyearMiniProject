#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
const char* ssid="nobel"; 
const char* pwd="sarobert";

char array[7];
char *smsg[2]; 
char *ptr=NULL; 
const int ms=27; 
bool d=false;
#define BOTtoken "1853711197:AAFCs29IvhgcMvZ3TC6Zot97jG0aWcN-r0g"

#define CHAT_ID "876922029"

WiFiClientSecure client; 
UniversalTelegramBot bot(BOTtoken,client); 

int Reqdelay=1000; 
unsigned long prevMillis; 
char a[10];
Adafruit_BME280 bme; 

String getReadings()
{
  float t,h; 
  t = bme.readTemperature();
  h = bme.readHumidity();
  return "Temperature: " + String(t) + " ºC \nHumidity: " + String (h) + " % \n";
}  

void handler(int count)
{
  Serial.println("Handling Messages"); 
  Serial.println(String(count)); 
  for(int i=0;i<count+1;i++)
  {
    String id=String(bot.messages[i].chat_id); 
    if(id!=CHAT_ID)
    {
      bot.sendMessage(id,"Unauthorised user",""); 
      continue;
    }  
    String msg=bot.messages[i].text; 
    msg.toCharArray(array,7);
    Serial.println(msg); 
    String sender=bot.messages[i].from_name; 
    String welcome="Welcome, "+sender+"\n Commands are:\n /stat \n/_on_ \n/_off \nFollow up the command with a space and GPIO no.\n available pins are 4,15,34,35,25,14,12,13";
    ptr=strtok(array," "); 
    smsg[0]=ptr; 
    ptr=strtok(NULL," "); 
    if(ptr!=NULL){smsg[1]=ptr;}
    if(smsg[0]=="/stat")
    {
       if(digitalRead(atoi(smsg[1]))){bot.sendMessage(id,"ON","");} 
       else{bot.sendMessage(id,"OFF","");}
    } 
    if(smsg[0]=="/_on_")
    {
      pinMode(atoi(smsg[1]),OUTPUT); 
      digitalWrite(atoi(smsg[1]),HIGH);
    } 
    if(smsg[0]=="/_off")
    {
      digitalWrite(atoi(smsg[1]),LOW);
    } 
    if(smsg[0]=="/read")
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
}





void loop() {
  if(millis()-prevMillis>=Reqdelay)
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
  
  bot.sendMessage(CHAT_ID,"HELLO"," ");
}
