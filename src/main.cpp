#include <Arduino.h>
#include <ADC_74HC4066.h>
#include <CurrentSensors.h>
#include <WiFi.h>
#include <eMonitorWebServer.h>
#include <SPIFFS.h>

eMonitorWebServer server(4444);
const char* ssid = "Ciau";
const char* password = "EaSyNeTMilk2";

/* Function to connect to WiFi */
void connectToWiFi();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectToWiFi();
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  server.begin();
  currentSensors.begin();
}

void loop()
{
  currentSensors.loop();
}

void connectToWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected. ");
}
