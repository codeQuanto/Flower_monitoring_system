#include <config.h>

#include <WiFi.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <HTTPClient.h>

// Dane Wi-Fi
const char* ssid = CONFIG_WIFI_SSID;
const char* password = CONFIG_WIFI_PASSWORD;

// Adres Node-RED
const char* serverName = CONFIG_SERVER_NAME;  // Zmienna w Node-RED - IP PC

//czujnik bme
Adafruit_BME280 bme;  // I2C
const int soilSensorPin = 33; 
int wilgotnoscGleby = 0;

unsigned long delayTime;
unsigned long lastTime;

float temperaturaOtoczenia;
float wilgotnoscPowietrza;
float cisnienie;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(F("BME280 test"));

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie z Wi-Fi...");
  }

  Serial.println("Połączono z Wi-Fi");

  bool status;
  status = bme.begin(0x76);

  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  delayTime = 15000;
  lastTime = 0;

  Serial.println();
}

void loop() {

  if ((millis() - lastTime) > delayTime) {
    measureValues();
    printValues();
    sendValuesNodeRed();
    lastTime = millis();
  }
}

void measureValues() {
  temperaturaOtoczenia = bme.readTemperature();
  wilgotnoscPowietrza = bme.readHumidity();
  cisnienie = bme.readPressure() / 100.0F;
  wilgotnoscGleby = (int)(100 - (analogRead(soilSensorPin))*100/4095);
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(temperaturaOtoczenia);
  Serial.println(" °C");

  Serial.print("Pressure = ");
  Serial.print(cisnienie);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(wilgotnoscPowietrza);
  Serial.println(" %");

  Serial.print("SoilHumidity = ");
  Serial.print(wilgotnoscGleby);
  Serial.println(" %");

  Serial.println();
}

void sendValuesNodeRed() {
  // Przygotowanie danych w formie JSON
  String payload = "{\"temperature\": " + String(temperaturaOtoczenia) + ", \"humidity\": " + String(wilgotnoscPowietrza) + ", \"pressure\": " + String(cisnienie) + ", \"soilHumidity\": " + String(wilgotnoscGleby) + "}";

  // Tworzenie żądania HTTP
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName);  //adres Node-RED
    http.addHeader("Content-Type", "application/json");

    // Wysyłanie danych
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("Dane wysłane do Node-RED");
    } else {
      Serial.println("Błąd wysyłania danych");
    }

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println();

    http.end();
  }
}
