/*
 * ====================================================================
 * Descripción: Este programa conecta una placa ESP32 a la red
 * WiFi de la escuela para realizar peticiones HTTP a la API de
 * Open-Meteo. Obtiene datos meteorológicos en tiempo real
 * (temperatura y humedad), los procesa utilizando formato JSON y los
 * muestra en el Monitor Serie y en una pantalla OLED SSD1306
 * ====================================================================
 */
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

const char *ssid = "ETEC-UBA";
const char *password = "ETEC-alumnos@UBA";
const char *endpoint = "https://api.open-meteo.com/v1/forecast?latitude=11.4692&longitude=-85.8166&current=temperature_2m,relative_humidity_2m&timezone=auto";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(115200);
  display.setTextColor(WHITE, BLACK);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }
  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  delay(2000);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(endpoint);
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      String payload = http.getString();

      JsonDocument doc;
      deserializeJson(doc, payload);

      JsonObject values = doc["current"];
      float temp = values["temperature_2m"];
      float h = values["relative_humidity_2m"];
      Serial.print(F("Humidity: "));
      Serial.println(h);
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      Serial.print(F("°C "));
      Serial.println();

      display.clearDisplay();
      display.setCursor(5, 0);
      display.print(WiFi.localIP());
      display.setCursor(5, 10);
      display.print(WiFi.subnetMask());

      display.setCursor(5, 20);
      display.print("Humedad: ");
      display.print(h);
      display.print("%");

      display.setCursor(5, 32);
      display.print("Temp: ");
      display.print(temp);
      display.print(" C");

      display.display();
      delay(500);
    }
    else
    {
      Serial.print("no se pudo conectar");
    }
  }
  else
  {
    Serial.print("no esta conectado al wifi");
  }
}