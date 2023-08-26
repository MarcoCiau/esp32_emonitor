#ifndef EMONITORWEBSERVER_H
#define EMONITORWEBSERVER_H
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ApiHandler.h>
/**
 * @brief Custom class for a web server with asynchronous handling.
 */
class eMonitorWebServer {
public:
    /**
     * @brief Constructor for the eMonitorWebServer class.
     * @param port The port number to listen on.
     */
    eMonitorWebServer(int port);

    /**
     * @brief Initialize and start the asynchronous web server.
     */
    void begin();

private:
    AsyncWebServer *server; /**< The underlying asynchronous web server instance. */
    int port; /**< Server port to listen on. */
};
#endif // EMONITORWEBSERVER_H

