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
    // dark room with finger over 200-400
    int lightValue = analogRead(DAYLIGHT_INPUT_PIN);

    // enable light if motion + darkness
    if (motionValue == HIGH && lightValue < 100) {
        digitalWrite(LIGHT_OUTPUT_PIN, HIGH);

        // delay for an on-duration in prod
#if !defined(DEBUG)
        delay(10 * 1000);
#endif
    }
    else {
        digitalWrite(LIGHT_OUTPUT_PIN, LOW);
    }

#ifdef DEBUG
    Serial.print("Motion ");
    Serial.print(motionValue);
    Serial.println();

    Serial.print("Light ");
    Serial.print(lightValue);
    Serial.println();
#endif

    delay(100);
}