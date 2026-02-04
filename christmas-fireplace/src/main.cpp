#include "Arduino.h"

byte flicker[] = {180, 30, 89, 23, 255, 200, 90, 150, 60, 230, 180, 45, 90};
int length = sizeof(flicker) / sizeof(flicker[0]);

void setup()
{
    pinMode(PB1, OUTPUT);
    randomSeed(analogRead(PB3)); // seed from a crystal pin
}

void loop()
{
    analogWrite(PB1, flicker[random(0, length)]);
    delay(random(100));
}

/*
Direct register write

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRB |= (1 << 1); // PB1 output

    while (1) {
        PORTB |= (1 << 1); // PB1 HIGH
        _delay_ms(500);
        PORTB &= ~(1 << 1); // PB1 LOW
        _delay_ms(500);
    }

    return 0;
}



*/