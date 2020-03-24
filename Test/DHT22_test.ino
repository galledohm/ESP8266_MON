#include <stdlib.h>
#include <DHT.h>

//Constants
const uint8_t DHT_data_pin = 4;               // Arduino pin were DHT22 data pin is connected
const uint8_t DP_David_Bolton_1980 = 0;
const uint8_t DP_Sonntag_1990 = 1;
const uint8_t DP_Paroscientific_1974 = 2;
const uint8_t DP_Arden_buck_1 = 3;
const uint8_t DP_Arden_buck_2 = 4;
const uint8_t DP_MODEL = 3;

// Variables
float temp = 0.0;
int hum = 0;
float heat_index = 0.0;
float dp = 0.0;

//Objects
DHT dht(DHT_data_pin, DHT22);
void setup() 
{
  // Serial conf
  Serial.begin(9600);
  dht.begin();
}

void loop() 
{
  // Temperature Reading
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  heat_index = dht.computeHeatIndex(temp, hum, false);
  dp = calcDewpoint(hum, temp, DP_MODEL);
  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Humidity (%): "); Serial.println(hum);
  Serial.print("Heat Index: "); Serial.println(heat_index);
  Serial.print("Dew Point: "); Serial.println(dp);
  Serial.println();
  delay(7000);
}

float calcDewpoint(float humidity, float temperature, uint8_t model)
{
  //NOTE: Humidity (%) must be between 1%<H<100%
  float gamma = 0.0, b = 0.0, c = 0.0;
  switch (model)
  {
    case 0:           // David Bolton - 1980 (error ±0.1ºC%, -30ºC<T<35ºC)
      b = 17.67; 
      c = 243.5;     // ºC
      break;
    case 1:           // Sonntag - 1990 (error ±0.35ºC%, -45ºC<T<60ºC)
      b = 17.62; 
      c = 243.12;     // ºC
      break;
    case 2:           // Paroscientific - 1974 (error ±0.4ºC, 0ºC<T<60ºC)
      b = 17.27; 
      c = 237.7;     // ºC
      break;
    case 3:           // Arden Buck 1 (error ±0.05ºC, 0ºC<T<50ºC)
      b = 17.368; 
      c = 238.88;     // ºC
      break;
    case 4:           // Arden Buck 2 (error ±0.06C, -40ºC<T<0ºC)
      b = 17.966; 
      c = 247.15;     // ºC
      break;
    default:          // Default values (Sonntag)
      b = 17.62; 
      c = 243.12;     // ºC
      break;
  }
  
  gamma = log(humidity/100) + (b * temperature) / (c + temperature);
  return (c * gamma / (b - gamma));
}
