#include "config-wifi.h"
#include "engine.h"
#include "index.h"
#include <DIYables_ESP32_WebServer.h>
#include <ESPmDNS.h>

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

DIYables_ESP32_WebServer server;
DIYables_ESP32_WebSocket* webSocket;
Engine engine;

void onWebRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData)
{
    Serial.println("Control.onWebRequest");
    server.sendResponse(client, HTML_CONTENT);
}

void onWebSocketOpen(net::WebSocket& ws)
{
    Serial.println("Control.onWebSocketOpen");
}

void onWebSocketClose(net::WebSocket& ws, const net::WebSocket::CloseCode code, const char* reason, uint16_t length)
{
    Serial.println("Control.onWebSocketClose");
}

void onWebSocketMessage(net::WebSocket& ws, const net::WebSocket::DataType dataType, const char* message, uint16_t length)
{
    int command = String(message).toInt();

    Serial.print("Control.onWebSocketMessage - command: ");
    Serial.println(command);

    switch (command) {
    case CMD_STOP:
        engine.drive(0);
        break;

    case CMD_FORWARD:
        engine.drive(1);
        break;

    case CMD_BACKWARD:
        engine.drive(-1);
        break;

    case CMD_LEFT:
        engine.turn(-1);
        break;

    case CMD_RIGHT:
        engine.turn(1);
        break;

    default:
        Serial.println("Control.onWebSocketMessage - Unknown command");
    }
}

void setupControl()
{
    Serial.println("Control.setup");
    server.addRoute("/", onWebRequest);

    server.begin(WIFI_SSID, WIFI_PASSWORD);

    // web socket event handlers
    webSocket = server.enableWebSocket(81);
    if (webSocket != nullptr) {
        webSocket->onOpen(onWebSocketOpen);
        webSocket->onMessage(onWebSocketMessage);
        webSocket->onClose(onWebSocketClose);
    }
    else {
        Serial.println("Control.setup - Failed to web socket");
    }

    // init engine
    engine.setup();
}

void processControlInput()
{
    server.handleClient();
    server.handleWebSocket();
    engine.loop();
}