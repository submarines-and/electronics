#include <Arduino.h>

#define AIN1 A1
#define AIN2 A2

void setup()
{

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
}

void loop()
{
    //
    // FORWARD
    //
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

    //
    // REVERSE
    //
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