#include "engine.h"
#include "logic.h"
#include "ota.h"
#include <Arduino.h>

OTA ota;
Engine engine;
Logic logic;

void setup(void)
{
#ifdef DEBUG
    Serial.begin(115200);
#endif

    ota.setup();
    engine.setup();
    logic.setup();
}

void loop(void)
{
    ota.checkForUpdate();

    auto distance = logic.getDistance();

    // go forward i clear
    if (distance >= 30) {
        engine.drive(1);
        delay(100);
        return;
    }

    // obstacles detected, stop and reverse left
    engine.stop();
    engine.turn(-1);
    engine.drive(-1);
    delay(1000);

    // then continue straight (turn back to center)
    engine.turn(1);
}
