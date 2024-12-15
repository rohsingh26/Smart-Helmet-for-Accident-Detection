#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Rohit";
const char* password = "123456789";

// Initialize Telegram BOT
#define BOTtoken "7708660511:AAHDclGkNhcU1DeO28CIG92utFGm6oW6F0s"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1160616574"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int buzzer=4;


#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

#include <TinyGPS.h>
TinyGPS gps;

void getgps(TinyGPS &gps);

char * dtostrf(
  double __val,
  signed char __width,
  unsigned char __prec,
  char * __s);

float latitude, longitude;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
//  Serial2.begin(9600);
  Serial.println("Accident Detection Vehicle");
   pinMode(buzzer,OUTPUT);


    // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
     
  bot.sendMessage(CHAT_ID, "Accident Detection Vehicle");


  
   if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }

 
}

void loop() 
{
  // put your main code here, to run repeatedly:
   byte a;
  
  if ( Serial.available() > 0 ) // if there is data coming into the serial line
  
  {
  
    a = Serial.read(); // get the byte of data
    
    if(gps.encode(a)) // if there is valid GPS data...
    {
    
      getgps(gps); // grab the data and display it on the LCD
    
    }
  
  }

 

}


void ADXL()
{
  sensors_event_t event; 
   accel.getEvent(&event);
   float X=event.acceleration.x;
   float Y=event.acceleration.y;
   float Z=event.acceleration.z;
//   Serial.print("X: "); Serial.print(X); Serial.print("  ");
//   Serial.print("Y: "); Serial.print(Y); Serial.print("  ");
//   Serial.print("Z: "); Serial.print(Z); Serial.print("  ");



//   Serial.println("");
   delay(500);
   if(X<-8||X>8||Y<-8||Y>8)
   {
    Serial.println("Accident Occured");
    
   
   
  String one = "ACCIDENT OCCURED AT:https://www.google.com/maps/?q=";
  String two = "," ;   
 
  String message = one + String(latitude, 6) + two + String(longitude, 6);
  // Convert String to char array
  int str_len = message.length() + 1;
  char textmessage[str_len];
  message.toCharArray(textmessage,str_len);
  Serial.println(textmessage);
  digitalWrite(buzzer,HIGH);
  bot.sendMessage(CHAT_ID,textmessage);
 
  delay(2000);  
  digitalWrite(buzzer,LOW);
  delay(500);
 }
}

void getgps(TinyGPS &gps)
{ 
  gps.f_get_position(&latitude, &longitude);
  
//  lcd.setCursor(0,0);
  Serial.print("Lat:");
  Serial.print("Lat:");
//  lcd.print("Lat:");
  Serial.println(latitude,5);
  Serial.println(latitude,5);
  //lat_buff=latitude;
 // sprintf(lat_buff, "LAT:%f", latitude,5);
  //dtostrf(
//  lcd.print(latitude,5);
  //Serial.println(lat_buff);
  //lcd.print(" ");
  
//  lcd.setCursor(0,1);
  Serial.print("Long:");
  Serial.print("Long:");
//  lcd.print("Long:");
  //Serial.print(floatToString());
  Serial.println(longitude,5);
  Serial.println(longitude,5);
  //long_buff=longitude;
 // sprintf(long_buff, "%f", longitude,5);
  //dtostrf(long_buff,8,5,longitude);
  //float latitude = -23.123456;
  //float longitude = 135.123456;
  
  String one = "Vehicle LOCATION AT: https://www.google.com/maps/?q=";
  String two = "," ;   
  String message = one + String(latitude, 6) + two + String(longitude, 6);

  int str_len = message.length() + 1;
  char textmessage[str_len];
  message.toCharArray(textmessage,str_len);
  Serial.println(textmessage);
  delay(1000);  
   ADXL();
}
