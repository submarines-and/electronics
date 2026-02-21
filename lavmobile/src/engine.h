#pragma once
#include <ESP32Servo.h>

class Engine {

private:
    Servo servo;

public:
    void setup();
    void turn(int direction);
    void drive(int direction);
};