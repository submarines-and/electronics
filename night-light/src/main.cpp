#ifdef DEBUG
#define TX_PIN PB2 // physical 7
#include <ATtinySerialOut.hpp>
#endif

#include <Arduino.h>

// pin config
#define MOTION_INPUT_PIN A3   // physical 2
#define DAYLIGHT_INPUT_PIN A2 // physical 3
#define LIGHT_OUTPUT_PIN PB1  // physical 6

void setup()
{
#ifdef DEBUG
    initTXPin();
#endif

    pinMode(LIGHT_OUTPUT_PIN, OUTPUT);
}

void loop()
{
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