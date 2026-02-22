#include <Arduino.h>

/**
 * Pins!!
 *
 */
#ifdef ARDUINO_ADAFRUIT_FEATHER_ESP32_V2

// featherv2 pins
#define SENSOR_CALIBRATION 37
#define SENSOR_INPUT 12
#define SENSOR_POWER 15
#define NOTIFICATION 33

#else

// sleep library for attiny
#include "tinysnore.h"

// attiny pins
#define SENSOR_CALIBRATION PB1 // 6
#define SENSOR_INPUT PB3       // 2
#define SENSOR_POWER PB4       // 3
#define NOTIFICATION PB2       // 7

#endif

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
#endif

    pinMode(SENSOR_CALIBRATION, INPUT);
    pinMode(SENSOR_INPUT, INPUT);
    pinMode(SENSOR_POWER, OUTPUT);
    pinMode(NOTIFICATION, OUTPUT);

    digitalWrite(SENSOR_POWER, LOW);
}

void loop()
{
    // activate sensor then delay to ensure it has time to turn on
    digitalWrite(SENSOR_POWER, HIGH);
    delay(100);

    // calibrate sensor if pin is connected
    if (digitalRead(SENSOR_CALIBRATION) == HIGH) {

        // plant needs water if input is high
        // to calibrate, adjust potentiometer until the light comes on
        // this will mean that the next time the soil has the same dryness, it will come on again
        if (digitalRead(SENSOR_INPUT) == HIGH) {
            digitalWrite(NOTIFICATION, HIGH);
        }
        else {
            digitalWrite(NOTIFICATION, LOW);
        }

        delay(100);
        return;
    }

    if (digitalRead(SENSOR_INPUT) == HIGH) {
        digitalWrite(SENSOR_POWER, LOW);

        // blink lights if plant needs water
        for (int i = 0; i < 5; i++) {
            digitalWrite(NOTIFICATION, HIGH);
            delay(200);
            digitalWrite(NOTIFICATION, LOW);
            delay(200);
        }

        // slight delay before blinking again
        delay(10000);
    }
    else {
        digitalWrite(SENSOR_POWER, LOW);
        digitalWrite(NOTIFICATION, LOW);

#ifdef ARDUINO_ADAFRUIT_FEATHER_ESP32_V2

        // in dev, wait a short while
        delay(5000)
#else

        // in prod, sleep for an hour
        // use a loop to avoid int overflow
        uint32_t tenMinutes = 600000;
        for (int i = 0; i < 6; i++) {
            snore(tenMinutes);
        }
#endif
    }
}