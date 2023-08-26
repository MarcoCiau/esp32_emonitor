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
        request->send(200, "application/json", apiHandler.handleGetAllCalibrate());
        Serial.println("GET /calibration");
    });

    // Handle GET request for current data
    server->on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", apiHandler.handleGetAllCurrentSensor());
        Serial.println("GET /current");
    });

    // Handle POST request for calibration update
    server->on("/calibrate", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        StaticJsonDocument<128> doc;
        DeserializationError error = deserializeJson(doc, data);
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return request->send(404, "text/plain", "deserializeJson() failed.\n");
        }
        String response;
        response.reserve(256);
        bool res = apiHandler.handleCalibrate(doc, &response);
        if (!res) return request->send(404, "text/plain", "handleCalibrate() failed.\n");
        request->send(200, "text/plain", response);
    });

    // Start the server
    server->begin();
    Serial.print("Server running on port: ");
    Serial.println(this->port);
}
