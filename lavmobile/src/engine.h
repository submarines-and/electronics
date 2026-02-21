#pragma once
#include <ESP32Servo.h>

class Engine {

private:
    Servo servo;
    int currentDirection;
    bool isDriving = false;

public:
    void setup();
    void turn(int direction);
    void drive(int direction);
    void stop();
};