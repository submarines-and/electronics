#include "engine.h"
#include "pins.h"
#include <Arduino.h>

void Engine::setup()
{
    Serial.println("Engine.setup");

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

/** Process current direction */
void Engine::loop()
{
    if (currentDirection == 1) {
        // forward
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else if (currentDirection == -1) {
        // reverse
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    else {
        // stop
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
    }
}

/**
 * Drive!
 * -1 is backwards
 * 1 is forwards
 */
void Engine::drive(int direction)
{
    Serial.print("Engine.drive, direction: ");
    Serial.println(direction);

    currentDirection = direction;
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
    Serial.print("Engine.turn ");
    Serial.println(direction);

    int degrees = 90;

    // offset that makes it turn enough
    int offset = 40;

    if (direction == -1) {
        degrees = 0 + offset;
    }
    else if (direction == 1) {
        degrees = 180 - offset;
    }

    Serial.print("Engine.turn, direction: ");
    Serial.print(direction);
    Serial.print(", degrees (speed):");
    Serial.println(degrees);

    // turn and wait for turn to complete
    servo.write(degrees);
    delay(300);

    // stop
    servo.write(90);
}