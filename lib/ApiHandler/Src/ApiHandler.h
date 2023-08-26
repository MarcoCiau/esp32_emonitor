#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <ArduinoJson.h> // You need to include the appropriate library for JSON handling

/**
 * @brief Class to handle API functions for eMonitorWebServer.
 */
class ApiHandler {
public:
    /**
     * @brief Constructor for the ApiHandler class.
     */
    ApiHandler();

    /**
     * @brief Handle the calibration update request.
     * @param json JSON data containing calibration information.
     * @param response The plain text response message.
     * @return The status. True if it's successfull or False if it's failed.
     */
    bool handleCalibrate(const JsonDocument& json, String *response);

    /**
     * @brief Handle and GET ALL calibration data.
     * @return The response message. JSON data containing ALL calibration information.
     */
    String handleGetAllCalibrate();

    /**
     * @brief Handle and GET ALL current's sensors data.
     * @return The response message. JSON data containing ALL calibration information.
     */
    String handleGetAllCurrentSensor();


private:
    // You can add private helper functions or variables here if needed.
};
extern ApiHandler apiHandler;
#endif // APIHANDLER_H
