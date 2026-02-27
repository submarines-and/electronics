# Lävmobile
Esp32 powered lego car with over the air updates.

![Driving prototype](./img/prototype.mov)

## Wiring
n/a


## Flashing
Create `config-wifi.h` under src with contents: 
```
#define WIFI_SSID "<SSID>"
#define WIFI_PASSWORD "<PASSWORD>"
```

Flash first time using env:usb, then you can upload using ota by setting ip in platformio.ini.

## Parts

| Part    | Link |
| -------- | ------- |
| Lego Technic 8032 | https://www.bricklink.com/v2/catalog/catalogitem.page?S=8032-1#T=S&O={%22iconly%22:0}     |
| Adafruit feather v2 | https://www.electrokit.com/en/adafruit-esp32-feather-v2 |
| Adafruit DRV8833 | https://www.amazon.se/-/en/Adafruit-DRV8833-Stepper-Driver-Breakout/dp/B01MFE1CZU |
| HC-SR04 ultrasound sensor (3v) | https://shop.pchbutik.se/sv/sensor-givare/1654-kompatibel-med-hc-sr04-33-till-5-volt-passar-aruino.html |
| 3.7v LiPo battery (for feather) | https://www.electrokit.com/en/batteri-lipo-3.7v-2000mah |
| 6v worth of AA batteries (for motor) | https://www.electrokit.com/en/batterihallare-4xaa-box-brytare-sladd |

## DC motor and Servo alternatives
Depending on lego model and gearbox setup, you will need 1-4 DC motors and at least 1x servo for steering. For motor, either get a 9v lego motor for full compatibility with many technic model instructions or get any 3-6v motor and glue some [kind of adapter to a lego shaft](https://www.adafruit.com/product/3810?srsltid=AfmBOooq4cipMnCVcj6Y1Y-e8ZM3b0YRZ8ASicpiZ4WQOGnRRyRc6uer). Ink cannisters from ballpoint pens or shrink tube make great adapters. For the servo, I found a lego compatible version that connects directly to gears, but I am sure a similar result can be achieved using glue and an adapter.


| Motor    | Link |
| -------- | ------- |
| Lego Technic 8720 | https://www.bricklink.com/v2/catalog/catalogitem.page?S=8720-1#T=S&O=%7B%2522iconly%2522:0%7D     |
| Example 3v DC motor | https://www.electrokit.com/en/dc-motor-med-flaktblad-3-6v |
| Lego 360 Servo | https://www.ebay.co.uk/itm/334400887998?mkpid=0&emsid=e111011.m144671.l197929&ch=osgood&euid=4ab9c8da634c4369bebbeadfb5204bc9&bu=45738976813&exe=0&ext=0&osub=-1%7E1&crd=20260207041000&segname=111011 |


## Guides
- https://lukebeales.com/electronics_iot/reviving-30-year-old-lego-technic-with-websockets/
- https://docs.elegantota.pro/getting-started/integration
- https://www.youtube.com/watch?v=zibtKRgKjVU
- https://github.com/ayushsharma82/ElegantOTA/blob/master/platformio_upload.py
- https://www.build-electronic-circuits.com/arduino-ultrasonic-sensor-hc-sr04/
- https://esp32io.com/tutorials/esp32-controls-car-via-web
- https://learn.adafruit.com/adafruit-drv8833-dc-stepper-motor-driver-breakout-board/arduino-dc-motor

![Lego model](./img/prototype.jpeg)