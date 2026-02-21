#include "logic.h"
#include <Arduino.h>

#define TRIGGER 14
#define ECHO 15

void Logic::setup()
{
#ifdef DEBUG
    Serial.println("Logic.setup");
#endif

    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
}

/** Returns distance in CM */
int Logic::getDistance()
{
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    // read distance
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

    // Speed of sound wave divided by 2 (there and back again)
    int distanceCM = pulseIn(ECHO, HIGH) * 0.034 / 2;

#ifdef DEBUG
    Serial.print("Logic.getDistance ");
    Serial.println(distanceCM);
#endif

    return distanceCM;
}