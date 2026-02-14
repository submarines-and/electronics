# Lävmobile
Esp32 powered lego car. 


## Flashing
Create `config-wifi.h` under src with contents: 
````
#define WIFI_SSID "<SSID>"
#define WIFI_PASSWORD "<PASSWORD>"
```

Flash first time using env:usb, then you can upload using ota by setting ip in platformio.ini.

## Parts
| Part    | Link |
| -------- | ------- |
| Lego Technic 8032 | https://www.bricklink.com/v2/catalog/catalogitem.page?S=8032-1#T=S&O={%22iconly%22:0}     |
| Adafruit feather v2 | https://www.electrokit.com/en/adafruit-esp32-feather-v2 |
| 3v DC motor | https://www.electrokit.com/en/dc-motor-med-flaktblad-3-6v |
| Adafruit DRV8833 | https://www.amazon.se/-/en/Adafruit-DRV8833-Stepper-Driver-Breakout/dp/B01MFE1CZU |
| 3.7v LiPo battery (for feather) | https://www.electrokit.com/en/batteri-lipo-3.7v-2000mah |


## Guides
- https://lukebeales.com/electronics_iot/reviving-30-year-old-lego-technic-with-websockets/
- https://docs.elegantota.pro/getting-started/integration
- https://www.youtube.com/watch?v=zibtKRgKjVU
- https://github.com/ayushsharma82/ElegantOTA/blob/master/platformio_upload.py