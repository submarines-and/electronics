# AVR ISP
ISP code for AVR devices. Supports the devices that I use. Based on this [tutorial](https://www.youtube.com/watch?v=tBq3sO1Z-7o&list=PLNyfXcjhOAwOF-7S-ZoW2wuQ6Y-4hfjMR&index=2), but for esp instead of an atmega/arduino. Last video uses a attiny.

# Supported devices (that I have tested for)
Host: Adafruit Feather v2, ESP8266
Target: ATTiny85

For all boards, connect a 10kohm pull-up resistor from attiny pin1 to power rail (see schematics).

## Feather v2
https://docs.platformio.org/en/latest/boards/espressif32/adafruit_feather_esp32_v2.html

| Feather V2    | Attiny86 |
| -------- | ------- |
| SCK   | pin 7 (SCK)     |
| MISO  | pin 6 (MISO)      |
| MOSI     | pin 5 (MOSI)     |
| D14    | pin 1 (RESET)     |
| GND    | pin 4     |
| 3.3v     | pin 8    |


![Diagram featherv2](./img/diagram-featherv2.png)

![Circuit featherv2](./img/circuit-featherv2.png)



## Esp8266
https://docs.platformio.org/en/latest/boards/espressif8266/esp12e.html

| Esp8266    | Attiny86 |
| -------- | ------- |
| SCK (D5)  | pin 7 (SCK)     |
| MISO (D6) | pin 6 (MISO)      |
| MOSI (D7)    | pin 5 (MOSI)     |
| D4    | pin 1 (RESET)     |
| GND    | pin 4     |
| 3.3v     | pin 8    |



![Diagram esp8266](./img/diagram-esp8266.png)

![Circuit esp8266](./img/circuit-esp8266.png)





