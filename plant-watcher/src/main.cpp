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

// Fake sleep in dev, real sleep in prod (using lib)
void sleep(int durationSeconds)
{
#ifdef ARDUINO_ADAFRUIT_FEATHER_ESP32_V2
    delay(durationSeconds * 1000)
#else
    snore(durationSeconds * 1000);
#endif
}

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

    // calibrate sensor if pin is connected
    if (digitalRead(SENSOR_CALIBRATION) == HIGH) {
        digitalWrite(SENSOR_POWER, HIGH);
        if (digitalRead(SENSOR_INPUT) == 1) {
            digitalWrite(NOTIFICATION, HIGH); // 1 = dry
        }
        else {
            digitalWrite(NOTIFICATION, LOW); // 0 = wet
        }

        delay(15);
        return;
    }

    // normal operation
    digitalWrite(SENSOR_POWER, HIGH);
    delay(20);
    if (digitalRead(SENSOR_INPUT) == 1) {
        digitalWrite(SENSOR_POWER, LOW); // 1 = dry

        for (byte i = 0; i < 5; i++) {
            digitalWrite(NOTIFICATION, HIGH);
            delay(25);
            digitalWrite(NOTIFICATION, LOW);
            delay(25);
        }

        // sleep for a short time
        sleep(10);
    }
    else {
        digitalWrite(SENSOR_POWER, LOW); // 0 = wet
        digitalWrite(NOTIFICATION, LOW);

        // sleep for a long time
        sleep(3600);
    }
}