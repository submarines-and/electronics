#pragma once
#include <ESP32Servo.h>

class Engine {

private:
    Servo servo;
    int currentDirection = 0;

public:
    void setup();
    void loop();
    void turn(int direction);
    void drive(int direction);
};