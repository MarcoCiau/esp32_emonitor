#include "ApiHandler.h"
#include "CurrentSensors.h"
#include <HWConfig.h>
/**
 * @brief Constructor for the ApiHandler class.
 */
ApiHandler::ApiHandler() {
    // Constructor initialization if needed
}

/**
 * @brief Handle the calibration update request.
 * @param json JSON data containing calibration information.
 * @param response The plain text response message.
 * @return The status. True if it's successfull or False if it's failed.
 */
bool ApiHandler::handleCalibrate(const JsonDocument& json, String *response) {
    int channel = json["channel"]; 
    int value = json["calibrationValue"]; 
    if (channel >= NUM_MAX_CURRENT_SENSOR) return false;
    Serial.printf("Data-> channel : %d , %d \n", channel, value);
    // Perform the calibration update tasks here
    currentSensors.setCalibration(channel, value);
    // Prepare and return a response message
    *response = "Calibration value for channel " + String(channel) + " updated to " + String(value);
    return true;
}

String ApiHandler::handleGetAllCalibrate()
{
    String response = "[";
    for (uint8_t i = 0; i < NUM_MAX_CURRENT_SENSOR; i++)
    {
        response += String(currentSensors.getCalibration(i));
        if (i < NUM_MAX_CURRENT_SENSOR - 1) response += ",";
    }
    response += "]";
    return response;
}

String ApiHandler::handleGetAllCurrentSensor()
{
    String response = "[";
    for (uint8_t i = 0; i < NUM_MAX_CURRENT_SENSOR; i++)
    {
        response += String(currentSensors.getValue(i));
        if (i < NUM_MAX_CURRENT_SENSOR - 1) response += ",";
    }
    response += "]";
    return response;
}

ApiHandler apiHandler;
