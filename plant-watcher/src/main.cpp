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

    // calibrate sensor if pin is connected
    if (digitalRead(SENSOR_CALIBRATION) == HIGH) {
        digitalWrite(SENSOR_POWER, HIGH);
        if (digitalRead(SENSOR_INPUT) == 1) {
            digitalWrite(NOTIFICATION, HIGH); // 1 = dry
        }
        else {
            digitalWrite(NOTIFICATION, LOW); // 0 = wet
        }

        delay(100);
        return;
    }

    // normal operation
    digitalWrite(SENSOR_POWER, HIGH);
    delay(100);
    if (digitalRead(SENSOR_INPUT) == 1) {
        digitalWrite(NOTIFICATION, HIGH); // 1 = dry
    }
    else {
        digitalWrite(NOTIFICATION, LOW); // 0 = wet
    }

    digitalWrite(SENSOR_POWER, LOW);
    delay(2000);
}