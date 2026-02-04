#include <Arduino.h>

/**
 * Pins!!
 *
 */
#ifdef ARDUINO_ADAFRUIT_FEATHER_ESP32_V2
// featherv2 pins
#define SENSOR_CALIBRATION 37
#define SENSOR_INPUT 12
#define SENSOR_POWER 15
#define NOTIFICATION 33
#else
// attiny pins
#include <avr/sleep.h>
#include <avr/wdt.h>

#define SENSOR_CALIBRATION PB1 // 6
#define SENSOR_INPUT PB3       // 2
#define SENSOR_POWER PB4       // 3
#define NOTIFICATION PB2       // 7

#endif

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
#endif

    pinMode(SENSOR_CALIBRATION, INPUT);
    pinMode(SENSOR_INPUT, INPUT);
    pinMode(SENSOR_POWER, OUTPUT);
    pinMode(NOTIFICATION, OUTPUT);

    digitalWrite(SENSOR_POWER, LOW);
}

void sleep(const byte duration)
{
#ifdef ARDUINO_ADAFRUIT_FEATHER_ESP32_V2
    delay(8000);
#else
    MCUSR = 0;
    WDTCR |= 0b00011000;
    WDTCR = 0b01000000 | duration;

    wdt_reset();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
#endif
}

void loop()
{

    // calibrate sensor if pin is connected
    if (digitalRead(SENSOR_CALIBRATION) == HIGH) {
        digitalWrite(SENSOR_POWER, HIGH);
        if (digitalRead(SENSOR_INPUT) == 1) {
            digitalWrite(NOTIFICATION, HIGH); // 1 = dry
        }
        else {
            digitalWrite(NOTIFICATION, LOW); // 0 = wet
        }

        delay(100);
        return;
    }

    // normal operation
    digitalWrite(SENSOR_POWER, HIGH);
    delay(100);
    if (digitalRead(SENSOR_INPUT) == 1) {
        digitalWrite(SENSOR_POWER, LOW); // 1 = dry

        for (byte i = 0; i < 10; i++) {
            digitalWrite(NOTIFICATION, HIGH);
            delay(200);
            digitalWrite(NOTIFICATION, LOW);
            delay(200);
        }

        // sleep 8 seconds
        sleep(0b100001);
    }
    else {
        digitalWrite(SENSOR_POWER, LOW); // 0 = wet
        digitalWrite(NOTIFICATION, LOW);

        // sleep 30 min
        for (byte j = 0; j <= 225; j++) {
            sleep(0b100001);
        }
    }
}

// called when coming back from sleep
#if !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32_V2)

ISR(PCINT0_vect)
{
    wdt_disable();
}
#endif