#include "eMonitorWebServer.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

/**
 * @brief Constructor for the eMonitorWebServer class.
 * @param port The port number to listen on.
 */
eMonitorWebServer::eMonitorWebServer(int port)
{
    this->port = port;
    server = new AsyncWebServer(this->port);
}

/**
 * @brief Initialize and start the asynchronous web server.
 */
void eMonitorWebServer::begin() {
    // Serve index.html on the root path
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html");
    });

    // Serve dashboard.js
    server->on("/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/dashboard.js");
    });

    // Handle GET request for calibration data
    server->on("/calibration", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]\n");
        Serial.println("GET /calibration");
    });

    // Handle GET request for current data
    server->on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "[1.23, 2.34, 0.98, 1.45, 2.78, 0.53, 3.12, 0.87, 1.89, 2.34, 1.56, 0.99, 2.78, 0.87, 1.01, 2.45]\n");
        Serial.println("GET /current");
    });

    // Handle POST request for calibration update
    server->on("/calibrate", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        Serial.println((char*)data);
        StaticJsonDocument<128> doc;

        DeserializationError error = deserializeJson(doc, data);

        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return request->send(404, "text/plain", "deserializeJson() failed.\n");
        }

        int channel = doc["channel"]; 
        int value = doc["calibrationValue"]; 
        Serial.printf("Data-> channel : %d , %d \n", channel, value);
        request->send(200, "text/plain", "Calibration value updated successfully.\n");
    });

    // Start the server
    server->begin();
    Serial.print("Server running on port: ");
    Serial.println(this->port);
}
