# azdelivery-rc522

This repo contains a ESPhome config to connect your RC522 to Home Assistant.

Use the following wiring diagram:

| RC522 Pin | ESP32 GPIO |
| --- | --- |
| SDA (SS) | GPIO5 |
| SCK | GPIO18 |
| MOSI | GPIO23 |
| MISO | GPIO19 |
| RST | GPIO22 |
| 3.3V | 3.3V |
| GND | GND |