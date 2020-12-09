//#include <Arduino.h> Descomenta esta linea solo si usas Platformio como IDE
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define API_KEY "TU_KEY_DE_API" //API key de Youtube
#define CHANNEL_ID "UCqs6fKCKeNlzsOZAJFMsLAg" //ID del canal 

char ssid[] = "NOMBRE";         //El nombre de tu red WiFi
char password[] = "CONTRASENA"; //La password de tu red WiFi

LiquidCrystal_I2C lcd(0x27, 16, 2);


WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 60000;
unsigned long nextRunTime;

void setup(){
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

  Serial.begin(115200);

  //Seteamos el modo WIFI_STA y nos desconectamos si ya estabamos conectados
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  //Nos conectamos al Wifi
  lcd.setCursor(0, 0);
  lcd.print("Conectando:");
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 1);
  while (WiFi.status() != WL_CONNECTED){
    lcd.print(".");
    delay(500);
  }

  lcd.clear();
 
  // If using ESP8266 Core 2.5 RC, uncomment the following
  client.setInsecure();
}

void getYoutubeSubscribers(){ //Obtenemos suscriptores (y otras cosas si queremos) en Youtube.  
  if(api.getChannelStatistics(CHANNEL_ID)){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Suscriptores:");
      lcd.setCursor(0, 1);
      lcd.print(api.channelStats.subscriberCount);
      delay(30000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Visitas:");
      lcd.setCursor(0, 1);
      lcd.print(api.channelStats.viewCount);
    }
}

void loop(){
  if (millis() > nextRunTime)  {
    getYoutubeSubscribers();
    nextRunTime = millis() + timeBetweenRequests;
  }
  
}
