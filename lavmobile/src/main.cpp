#include "engine.h"
#include "logic.h"
#include "ota.h"

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
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(500);
}
