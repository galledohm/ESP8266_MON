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

In order to test the temperature and humidity sensor, load the file [DHT22_test.ino](Test/DHT22_test.ino) into your arduino and connect it as follows:

| Arduino       | DHT22    | 
| ------------- |--------- |
| 3.3v          | Vcc      |
| GND           | GND      |
| Digital Pin 4 | Data     |

Every 7s you should see the acquired data through the Serial Bus in your computer.

### ESP8266 Test

In order to test the WiFi module, load the file [ESP8266_Test.ino](Test/ESP8266_Test.ino) into the ESP8268-01S module. To do so, follow steps 1 and 2 of this [link](https://create.arduino.cc/projecthub/ROBINTHOMAS/programming-esp8266-esp-01-with-arduino-011389). Connect the pins as follows:

| Arduino       | ESP8266-01S    | 
| ------------- |--------------- |
| 3.3v          | Vcc            |
| GND           | GND            |
| Tx            | Tx             |
| Rx            | Rx             |
| -             | GPIO 0         |
| -             | GPIO 2         |
| 3.3v          | CE             |
| 3.3v          | RESET          |

GPIO ports are not used in this testing program. Chip Enable (CE) and RESET ESP pins need to be connected to 3.3v in order to let the ESP work normally. 
Once you have uploaded the code to the ESP8266 module you will be able to control its led from a webpage with two buttons (ON/OFF).

**NOTE:** Never connect ESP8266-01S Vcc to more than 3.6v.

## Deployment

Once both devices have been tested and everything works properly, the final project can be implemented; at this point, a breadboard is required. Connect the pins as follows:

### Pin Connections

#### Arduino to BreadBoard

| Arduino       | BreadBoard    |
| ------------- | ------------- |
| 3.3v          | 3.3v          |
| GND           | GND           |
| Tx            | Tx            |
| Rx            | Rx            |


#### DHT22 to BreadBoard

| DHT22      | BreadBoard    |
| ---------- | ------------- |
| Vcc        | 3.3v          |
| GND        | GND           |
| Data       | GPIO2         |

**NOTE:** For the Data pin you can select any of the breadboard columns as far as you connect the ESP8266-01S GPIO2 pin to it.

#### ESP8266-01S to BreadBoard

|  ESP8266-01S   | BreadBoard    |
|  ------------- | ------------- |
|  Vcc           | 3.3v          |
|  GND           | GND           |
|  Tx            | Tx            |
|  Rx            | Rx            |
|  GPIO 0        | GPIO 0        |
|  GPIO 2        | GPIO 2        |
|  CE            | 3.3v          |
|  RESET         | 3.3v          |

### Beebotte Account Creation

- Create a beebotte account at this [link](https://beebotte.com). 
- Create a Beebotte Channel and record its name and token (they are required by mqtt protocol in order to send the data to this database).
- Inside the generated Channel, create the following resources:

	| Resource     | Type          |
	| ------------ | ------------- |
	| Temperature  | Temp          |
	| Humidity     | Humidity      |
	| Heat_Index   | Number        |
	| Dew_Point    | Number        |

- Create a Beebotte Dashboard with the prefered layout.

### User configuration and program loading

Before loading the file [ESP8266_MON.ino](Source/ESP8266_MON.ino) into the ESP8268-01S module, a couple of parameters must be edited and configured in order to make it work in your specific enviroment. Bellow a list with the editable parameters is shown:

**Basic Configuration:**

- **WIFI_SSID:** Name of your WiFi network.
- **WIFI_PASS:** Password of the above-mentioned WiFi network.
- **TOKEN:** Beebotte Channel token.
- **BBT_CHANNEL:** Beebotte Channel name.
- **interval:** Time between data acquisition procedure; by default, half an hour.

**Advanced Configuration:**

- **DHT_DATA_PIN:** ESP8266 input pin for gathering the DHT22 data. By default the programm will use the pin "2" (GPIO2), but you can also select pin "0" (GPIO0), the only issue with this pin is that it is also used for the configuration of the operation mode of the ESP8266-01S module.
- **Third argument in "calcDewpoint()" function:** Allows the selection of the Dew Point constants (check calcDewpoint() function in order to learn the different possibilities).

Finally, once everything is correctly configured, load the the file [ESP8266_MON.ino](Source/ESP8266_MON.ino) into the ESP8268-01S module following the steps 1 and 2 of this [link](https://create.arduino.cc/projecthub/ROBINTHOMAS/programming-esp8266-esp-01-with-arduino-011389).

## Built With

* [Arduino-IDE](https://www.arduino.cc/en/Main/Software) - Arduino framework
* [Beebotte](https://beebotte.com) - Beebotte DataBase

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for more details

## Acknowledgments

* Lazyness is the quickest path to inspire automation :)
* And two hard boiled eggs.
* Tosta&Tisco©



