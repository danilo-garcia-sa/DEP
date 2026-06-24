#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
const char *ssid = "PEINE-3"; // Change this to your WiFi SSID
const char *password = "etecPeine3"; // Change this to your WiFi password
const char *endpoint="http://10.56.13.21:5000/api/sensor";
#define DHTTYPE DHT11
#define DHTPIN 32
DHT dht(32, DHT11);

int potenciometro = 34; 


void setup() {

  dht.begin();
  Serial.begin(115200);

  

  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(potenciometro, INPUT);
  analogSetPinAttenuation(34, ADC_11db);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  delay(5000);
}

void loop() {
    float humedad = dht.readHumidity();
    float temperatura = dht.readTemperature();
   
  int valorPotenciometro = analogRead(potenciometro);

  Serial.print("Valor del potenciometro: ");
  Serial.println(valorPotenciometro);
  
  Serial.print("Valor de humedad: ");
  Serial.println(humedad);

  Serial.print("Valor de temp: ");
  Serial.println(temperatura);

  if (WiFi.status() == WL_CONNECTED){
   HTTPClient http;
   http.begin(endpoint);
   http.addHeader("Content-Type", "application/json");
   JsonDocument doc;
   JsonArray nombre = doc ["nombres"].to<JsonArray>();
   nombre.add("Danilo");
   nombre.add("Facundo");

   doc["potenciometro"] = valorPotenciometro;
   doc["humedad"] = humedad;
   doc["temperatura"] = temperatura;
   String payload;
   serializeJson(doc,payload);
   int httpCode = http.POST(payload);
   const int OK = 200;
    if (httpCode == OK) {
     Serial.print("Datos enviados: ");
     Serial.print(payload); 
    } else {
      Serial.print("no se pudo conectar");
      Serial.print(httpCode);
    }
    delay(5000);
  }
 }