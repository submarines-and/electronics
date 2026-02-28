#pragma once
#include <WebServer.h>

/** Logic for updating the app */
class OTA {

private:
    WebServer server;

public:
    OTA() : server(80) {}

    void setup();
    void checkForUpdate();
};