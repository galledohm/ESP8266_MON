# ESP8266_MON

A DHT22 sensor and ESP8266-01S module based monitoring environment. It allows to easily acquire temperature, relative humidity, heat index and dew point data and send it periodically to [Beebotte](https://beebotte.com) IoT Cloud PLatform.

## Contents

- [Getting Started](#Getting-Started)
	- [HW Prerequisites](#HW-Prerequisites)
	- [Dependencies](#Dependencies)
- [Testing HW Components](#Testing-HW-Components)
	- [DHT22 Test](#BHT22-Test)
	- [ESP8266 Test](#ESP8266-Test)
- [Deployment](#Deployment)
- [Built With](#Built-With)
- [Authors](#Authors)
- [License](#License)
- [Acknowledgments](#Acknowledgments)

## Getting Started

These instructions will let you understand how to configure and setup the whole environment. Starting from the required HW to the final implementation.

### HW Prerequisites

The following HW components are required to build this project (I have included some links for each of the components):

- [Arduino-UNO](https://store.arduino.cc/arduino-uno-rev3) - It will serve as the Serial Debugger for the ESP8266 and as the Serial converter. Any other arduino board will also work, but the pins used and referenced in this project refer to the Arduino-UNO board.
- [DHT22](https://www.banggood.com/AM2302-DHT22-Temperature-And-Humidity-Sensor-Module-p-937403.html?rmmds=search&cur_warehouse=CN) - Temperature and Humidity Sensor.
- [ESP8266-01S](https://www.banggood.com/ESP-01S-ESP8266-Serial-to-WiFi-Module-Wireless-Transparent-Transmission-Industrial-Grade-Smart-Home-Internet-of-Things-IOT-p-1471346.html?rmmds=search&cur_warehouse=CN) - WiFi Module.
- [Breadboard](https://www.banggood.com/Wholesale-Test-Develop-DIY-830-Point-Solderless-PCB-Bread-Board-For-MB-102-MB102-p-51331.html?rmmds=search&cur_warehouse=CN)
- [Jumpers Male to Male](https://www.banggood.com/40pcs-20cm-Male-To-Male-Color-Breadboard-Cable-Jumper-Cable-Dupont-Wire-p-70127.html?rmmds=search&cur_warehouse=USA) - Arduino to breadboard.
- [Jumpers Male to Female](https://www.banggood.com/40pcs-30cm-Male-To-Female-Jumper-Cable-Dupont-Wire-For-Arduino-p-994062.html?rmmds=search&cur_warehouse=ES) - Sensor and WiFi module to Breadboard.

### Dependencies

The following libraries are required in order to correctly execute and build this project (all of them were used with the latest stable version by 17th feb. 2020):

- stdlib.h: Used for Dew Point calculations.
- [DHT.h](https://github.com/adafruit/DHT-sensor-library): Adafruit DHT Library, used for DHT22 testing.
- [DHTesp.h](https://github.com/beegee-tokyo/DHTesp): Optimized DHT library for ESP8266. Obtained better results with this library.
- [WiFiEsp.h](https://github.com/bportaluri/WiFiEsp)
- [ESP8266WiFi.h](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
- [WiFiClient.h](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
- [PubSubClient.h](https://github.com/knolleary/pubsubclient/)
- [ArduinoJson.h](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
- [SoftwareSerial.h](https://www.arduino.cc/en/Reference/SoftwareSerial)

## Testing HW Components

### DHT22 Test

In order to test the temperature and humidity sensor. Load the file [DHT22_test.ino](Test/DHT22_test.ino) into your arduino and connect it as follows:

| Arduino       | DHT22    | 
| ------------- |--------- |
| 3.3v          | Vcc      |
| GND           | GND      |
| Digital Pin 4 | Data     |

Every 7s you should see the acquired data through the Serial Bus in your computer.

### ESP8266 Test

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Arduino-IDE](https://www.arduino.cc/en/Main/Software) - Arduino framework 

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for more details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc



