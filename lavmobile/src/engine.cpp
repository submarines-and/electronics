#include "engine.h"
#include <Arduino.h>

#define AIN1 A1
#define AIN2 A2
#define SLP 27
#define SERVO 13

void Engine::setup()
{
#ifdef DEBUG
    Serial.println("Engine.setup");
#endif

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(SLP, OUTPUT);
    pinMode(SERVO, OUTPUT);

    // start engine
    digitalWrite(SLP, HIGH);

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
    if (isDriving) {
        return;
    }

#ifdef DEBUG
    Serial.print("Engine.drive, direction: ");
    Serial.println(direction);
#endif

    currentDirection = direction;

    // turn of the other direction
    digitalWrite(direction == -1 ? AIN1 : AIN2, LOW);

    // ramp speed up
    for (int duty_cycle = 0; duty_cycle < 256; duty_cycle++) {
        analogWrite(direction == -1 ? AIN2 : AIN1, duty_cycle);
        delay(10);
    }

    isDriving = true;
}

/**
 * Turn the car
 *
 * Direction
 *  -1 left
 *  1 right
 */
void Engine::turn(int direction)
{
#ifdef DEBUG
    Serial.print("Engine.turn ");
    Serial.println(direction);
#endif

    int degrees = 90;

    // offset that makes it turn enough
    int offset = 40;

    if (direction == -1) {
        degrees = 0 + offset;
    }
    else if (direction == 1) {
        degrees = 180 - offset;
    }

#ifdef DEBUG
    Serial.print("Engine.turn, direction: ");
    Serial.print(direction);
    Serial.print(", degrees (speed):");
    Serial.println(degrees);
#endif

    // turn and wait for turn to complete
    servo.write(degrees);
    delay(500);

    // stop
    servo.write(90);
}

void Engine::stop()
{
#ifdef DEBUG
    Serial.println("Engine.stop");
#endif
    // ramp speed down
    for (int duty_cycle = 255; duty_cycle >= 0; duty_cycle--) {
        analogWrite(currentDirection == -1 ? AIN2 : AIN1, duty_cycle);
        delay(10);
    }

    isDriving = false;
}