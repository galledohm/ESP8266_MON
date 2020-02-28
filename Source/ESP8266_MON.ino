#include <stdlib.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/* WiFi Configuration*/ 
const char *WIFI_SSID = "*****"; //Enter your WIFI ssid 
const char *WIFI_PASS = "*****"; //Enter your WIFI password 
WiFiClient espclient;    

/* Beebotte MQTT DB Configutarion*/
const char *BBT_DOMAIN = "mqtt.beebotte.com";   // Beebotte mqtt URL
const char *TOKEN = "*****";            // Set your channel token here
const char *BBT_CHANNEL = "*****";      // Replace with your channel name
const char *T_BBT_RES = "Temperature";  // Replace Resource names if others have been defined at BeeBotte
const char *HU_BBT_RES = "Humidity";
const char *HE_BBT_RES = "Heat_Index";
const char *DP_BBT_RES = "Dew_Point";
PubSubClient pubclient(espclient);

/* DHT22 Parameters */
const uint8_t DHT_DATA_PIN = 2;               // GPIO Port were DHT22 data pin is connected (on ESP8266-S01 only GPIO 0 and GPIO-2 are available.)
const uint8_t DP_DAVID_BOLTON_1980 = 0;       // Dew Point Calculation Constants - Mode Selection (see function "calcDewpoint" for more details)
const uint8_t DP_SONNTAG_1990 = 1;
const uint8_t DP_PAROSCIENTIFIC_1974 = 2;
const uint8_t DP_ARDEN_BUCK_1 = 3;
const uint8_t DP_ARDEN_BUCK_2 = 4;
float temp = 0.0, heat_index = 0.0, dp = 0.0;
int hum = 0;
DHTesp dht;

/* Time Variables */
long lastReconnectAttempt = 0;
const unsigned long interval = 1800000;         // 0.5h - Modify if desired (value in ms)
unsigned long lastReadingMillis = 0;

void setup()
{
  dht.setup(DHT_DATA_PIN, DHTesp::DHT22);
  Serial.begin(115200);                         // ESP8266 default Baudrate

  // Open serial communications and wait for port to open:
  while (!Serial) ;                             // wait for serial port to connect. Needed for native USB port only
    
  Serial.println(); 
  Serial.print("Connecting to WiFi..."); 

  /* Configure ESP8266 as WiFi Client only (by default comes as client+AP) and connec to to WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS); 
  while (WiFi.status() != WL_CONNECTED)
  { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.println("WiFi connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  
  /* Set MQTT connection to BeeBotte Server */
  pubclient.setServer(BBT_DOMAIN, 1883);

  /* Debug Data Acquisition. These first data are not sent to BeeBotte in order to avoid potential erroneous data uploads to the DB */
  delay(dht.getMinimumSamplingPeriod());
  temp = dht.getTemperature();
  hum = dht.getHumidity();
  heat_index = dht.computeHeatIndex(temp, hum, false);
  dp = calcDewpoint(hum, temp, DP_ARDEN_BUCK_1);
  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Humidity (%): "); Serial.println(hum);
  Serial.print("Heat Index: "); Serial.println(heat_index);
  Serial.print("Dew Point: "); Serial.println(dp);
  Serial.println();
 }

void loop()
{
  /* Check */
  if (!pubclient.connected())  // Client not connected
  {
    long now = millis();
    if (now - lastReconnectAttempt > 5000)
    {
      lastReconnectAttempt = now;
      
      // Attempt to reconnect
      if (reconnect())
        lastReconnectAttempt = 0;      
    }
  }  
  else  // Client connected
  {
    unsigned long currentMillis = millis();

    /* Read sensor data and publish values to Beebotte */
    if (currentMillis - lastReadingMillis >= interval)
    {      
      lastReadingMillis = currentMillis;
      temp = dht.getTemperature();
      hum = dht.getHumidity();
      heat_index = dht.computeHeatIndex(temp, hum, false);
      dp = calcDewpoint(hum, temp, DP_ARDEN_BUCK_1);
      publishdata(temp, hum, heat_index, dp);
      Serial.print("Temperature: "); Serial.println(temp);
      Serial.print("Humidity (%): "); Serial.println(hum);
      Serial.print("Heat Index: "); Serial.println(heat_index);
      Serial.print("Dew Point: "); Serial.println(dp);
      Serial.println();
    }

    pubclient.loop();
  }
  
}


/* Calculates Dew Point */
float calcDewpoint(float humidity, float temperature, uint8_t model)
{
  // NOTE: Humidity (%) must be between 1%<H<100%
  // NOTE2: Each mode has its own temperature threshold and relative error. Select cautiously
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
    case 3:           // Arden Buck 1 (error ±0.05, 0ºC<T<50ºC)
      b = 17.368; 
      c = 238.88;     // ºC
      break;
    case 4:           // Arden Buck 2 (error ±0.06, -40ºC<T<0ºC)
      b = 17.966; 
      c = 247.15;     // ºC
      break;
    default:          // Default values (sonntag)
      b = 17.62; 
      c = 243.12;     // ºC
      break;
  }
  
  /* Dew point simplified calculation. Visit: https://en.wikipedia.org/wiki/Dew_point for more info */
  gamma = log(humidity/100) + (b * temperature) / (c + temperature);
  return (c * gamma / (b - gamma));
}

/* Publishes data to the specified resource */
void publish(const char* resource, float data, bool persist)
{
    StaticJsonDocument<128> root;
    root["channel"] = BBT_CHANNEL;
    root["resource"] = resource;
    if (persist)    
        root["write"] = true;
    
    root["data"] = data;

    // Print the JSONdoc into a char buffer
    char buffer[128];
    serializeJson(root, buffer, sizeof(buffer));

    // Create the topic to publish to
    char topic[64];
    sprintf(topic, "%s/%s", BBT_CHANNEL, resource);

    // Publish char buffer to Beebotte
    pubclient.publish(topic, buffer);
}

/* Publishes sensor data to BeeBotte */
void publishdata(float temperature, int humidity, float heat_index, float dp)
{
  publish(T_BBT_RES, temperature, true);
  publish(HU_BBT_RES, (float) humidity, true);
  publish(HE_BBT_RES, heat_index, true);
  publish(DP_BBT_RES, dp, true);
}

/* Reconnects to Beebotte MQTT server */
boolean reconnect() 
{
  if (pubclient.connect(generateID(), TOKEN, "")) 
    Serial.println("Connected to Beebotte MQTT");
  
  return pubclient.connected();
}

/* Generates random client ID. Needed for MQTT protocol */
const char *generateID()
{
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  return clientId.c_str();
}
