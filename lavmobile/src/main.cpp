#include "config-wifi.h"
#include <ESP32Servo.h>
#include <ElegantOTA.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

// pins
#define AIN1 A1
#define AIN2 A2
#define SERVO 12
#define TRIGGER 14
#define ECHO 15

// instances
WebServer server(80);
Servo servo;

void setup(void)
{
#ifdef DEBUG
    Serial.begin(115200);
#endif

    /**
     * OTA
     */
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // from "config-wifi.h, see readme file for contents"

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    server.on("/", []() { server.send(200, "text/plain", "/update"); });

    ElegantOTA.begin(&server);
    server.begin();

    /**
     * App
     */
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);

    // attach and stop servo
    servo.attach(SERVO, 500, 2500);
    servo.write(90);
}

// Direction = -1 | 1
void turn(int direction)
{
    int degrees = 90;
    int offset = 40;
    if (direction == -1) {
        degrees = 0 + offset;
    }
    else if (direction == 1) {
        degrees = 180 - offset;
    }

    servo.write(degrees);
    delay(500);
    servo.write(90);
}

void checkDistance()
{
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    // read distance
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

    // Speed of sound wave divided by 2 (there and back again)
    int32_t distance = pulseIn(ECHO, HIGH) * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(500);
}

void loop(void)
{
    /**
     * OTA
     */
    server.handleClient();
    ElegantOTA.loop();

    /**
     * App
     */

    // FORWARD
    digitalWrite(AIN2, LOW);

    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(AIN1, duty_cycle);
        delay(10);
    }
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(AIN1, duty_cycle);
        delay(10);
    }

    // REVERSE
    digitalWrite(AIN1, LOW);
    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(AIN2, duty_cycle);
        delay(10);
    }
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(AIN2, duty_cycle);
        delay(10);
    }
}
