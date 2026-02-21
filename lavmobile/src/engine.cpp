#include "engine.h"
#include <Arduino.h>

#define AIN1 A1
#define AIN2 A2
#define SERVO 12

void Engine::setup()
{
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    // attach and stop servo
    servo.attach(SERVO, 500, 2500);
    servo.write(90);
}

/**
 * Drive!
 * -1 is backwards
 * 1 is forwards
 */
void Engine::drive(int direction)
{
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);

    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(direction == -1 ? AIN2 : AIN1, duty_cycle);
        delay(10);
    }
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(direction == -1 ? AIN2 : AIN1, duty_cycle);
        delay(10);
    }
}

/**
 * Turn the car
 *
 * Direction -1 is left, 1 is right
 */
void Engine::turn(int direction)
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