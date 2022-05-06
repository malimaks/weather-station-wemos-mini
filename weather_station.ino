#include <Wire.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define dht_dpin D5
Adafruit_BME280 bme;
DHT dht(dht_dpin, DHTTYPE);
float temperatura_DHT;
float vlaga_DHT;
unsigned long delayTime;
unsigned int raw=0;
float volt=0.0;
float temperatura, vlaga, tlak;
char Temperatura;
char Vlaga;
char Tlak;
String apiKey = "YOUR WRITE API KEY FROM THINGSPEAK";
const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* server = "api.thingspeak.com";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);
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
  Serial.println(WiFi.localIP());
  bme.begin(0x76); 
  pinMode(A0, INPUT);
  raw = analogRead(A0);
  volt=raw/1023.0;
  volt=volt*4.2;
  String v=String(volt);
  dht.begin();
}
void loop() {
  temperatura = bme.readTemperature();
  vlaga = bme.readHumidity();
  tlak = bme.readPressure() / 100.0F;
  delay(200);
  float vlaga_DHT = dht.readHumidity();
  float temperatura_DHT = dht.readTemperature();
  delay(1000);
    Serial.print("Temperatura = ");
    Serial.println(temperatura);
    Serial.print("Vlaga = ");
    Serial.println(vlaga);
    Serial.print("Tlak = ");
    Serial.println(tlak);
    Serial.print("volt = ");
    Serial.println(volt);
    delay(1000);
    Serial.print("Temperatura DHT = ");
    Serial.println(temperatura_DHT);
    Serial.print("Vlaga DHT = ");
    Serial.println(vlaga_DHT);
    if (client.connect(server,80))
    Serial.println("Connect to ThingSpeak - OK"); 
    {
       String postStr = apiKey;
       postStr +="&field1=";
       postStr += String(temperatura);
       postStr +="&field2=";
       postStr += String(vlaga);
       postStr +="&field3=";
       postStr += String(tlak);
       postStr += "\r\n\r\n";
       postStr +="&field4=";
       postStr += String(volt);
       postStr += "&field5=";
       postStr += String(temperatura_DHT);
       postStr += "&field6=";
       postStr += String(vlaga_DHT);
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);        
    }
    delay(2000);
    client.stop();
    ESP.deepSleep(6e8); //10 minutes 6e8
}
