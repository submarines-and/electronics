#include "control.h"
#include <Arduino.h>

void setup(void)
{
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial) {
    }
#endif

    setupControl();
}

void loop(void)
{
    processControlInput();
}
