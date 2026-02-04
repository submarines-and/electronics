#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    pinMode(12, INPUT);  // sets the digital pin  as input
    pinMode(15, OUTPUT); // sets the digital pin  as output
    Serial1.begin(115200, SERIAL_8N1, 12, 15);
}

void loop()
{
    if (Serial1.available()) {
        int inByte = Serial1.read();
        Serial.write(inByte);
    }
}