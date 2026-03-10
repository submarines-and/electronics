#include "ota.h"
#include <Arduino.h>

/**
    ESP32
    https://www.waveshare.com/wiki/ESP32-C3-Zero
    https://files.waveshare.com/wiki/ESP32-C3-Zero/esp32-c3_datasheet_en.pdf
*/

OTA ota;

// analog
#define DAYLIGHT_INPUT_PIN 3

// digital
#define MOTION_INPUT_PIN 8
#define LIGHT_OUTPUT_PIN 6

// config
#define LIGH_THRESHOLD 200
#define ON_SECONDS 10

// vars for keeping track of on-state
bool turnOn = false;
unsigned long turnedOnAtMs = 0;

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) {
    }
#else
    ota.setup();
#endif

    pinMode(LIGHT_OUTPUT_PIN, OUTPUT);
}

void loop()
{
#if !defined(DEBUG)
    ota.checkForUpdate();
#endif

    // 1 for motion, 0 for none
    int motionValue = digitalRead(MOTION_INPUT_PIN);

    // room with lights 900
    // dark room with finger over sensor 200-400
    int lightValue = analogRead(DAYLIGHT_INPUT_PIN);

    // flag light for turning on
    // this also resets on-duration for repeat motion (in this case, the turn on flag is checked instead, since the LED will trigger the light sensor)
    if (motionValue == HIGH && (lightValue < LIGH_THRESHOLD || turnOn)) {

        // reset turn-on time
        turnedOnAtMs = millis();

        // only flag for turning on if loop was not reentered during that state
        if (!turnOn) {
            turnOn = true;
        }
    }

    // keep turned on for x seconds (as defined in config above)
    if (turnOn) {
        digitalWrite(LIGHT_OUTPUT_PIN, HIGH);

        // check turn on time against now
        unsigned long nowMs = millis();
        if (nowMs - turnedOnAtMs >= ON_SECONDS * 1000) {

            // turn off
            digitalWrite(LIGHT_OUTPUT_PIN, LOW);
            turnOn = false;
        }
    }

#ifdef DEBUG
    Serial.print("Motion ");
    Serial.print(motionValue);
    Serial.println();

    Serial.print("Light ");
    Serial.print(lightValue);
    Serial.println();
#endif
}