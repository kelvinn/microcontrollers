#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "processdata.h"

#define ONE_WIRE_BUS 15
#define DHTPIN 16
#define CLIENT_ID "30aea4108248"
#define LIGHTPIN 33
#define AQIPIN 34
#define RXD2 16
#define TXD2 17

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char msg_t[10];
char msg_h[10];
char msg_aqi[10];
char msg_light[10];
char msg_temperature[10];
char msg_pm25[10];
char msg_pm10[10];


void initSensors() {
  sensors.begin();
  SDS011.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

struct values get_temp_hum_dht11() {
  struct values temp_hum;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))  {
      Serial.println("Failed to read from DHT sensor!");
      strncpy(temp_hum.temperature, "0.0", 10);
      strncpy(temp_hum.humidity, "0.0", 10);
      return temp_hum;
  }
  
  dtostrf(t, 7, 1, msg_t);
  dtostrf(h, 7, 1, msg_h);

  strncpy(temp_hum.temperature, msg_t, 10);
  strncpy(temp_hum.humidity, msg_h, 10);
  
  Serial.print("Humidity: ");
  Serial.print(msg_h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(msg_t);
  Serial.println(" *C ");
  return temp_hum;
}

char* get_aqi() {

    int aqi = analogRead(AQIPIN);
    
    Serial.print("AQI: ");
    Serial.println(aqi);
    //dtostrf(aqi,7, 0, msg_aqi);
    sprintf(msg_aqi, "%d", aqi);
    return msg_aqi;
    
}

char* get_light() {

    int light = analogRead(LIGHTPIN);
    
    Serial.print("Light: ");
    Serial.println(light);
    //dtostrf(aqi,7, 0, msg_aqi);
    sprintf(msg_light, "%d", light);
    return msg_light;
}

char* get_temperature() {

  sensors.requestTemperatures(); 
  float temperature=sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  dtostrf(temperature, 7, 1, msg_temperature);
  return msg_temperature;
}

struct pm_values get_pm() {
  struct pm_values pm;
  turnon();
  delay(2000); // Wait 2s for sensor to turn on

  ProcessSerialData();

  //Pm25 = Pm25 / (1.0 + 0.48756 * pow((h / 100.0), 8.60068));
  //Pm10 = Pm10 / (1.0 + 0.81559 * pow((h / 100.0), 5.83411));

  turnoff();
  
  dtostrf(Pm25, 7, 0, msg_pm25);
  dtostrf(Pm10, 7, 0, msg_pm10);

  strncpy(pm.pm25, msg_pm25, 10);
  strncpy(pm.pm10, msg_pm10, 10);

  Serial.print("PM2.5: ");
  Serial.print(msg_pm25);
  Serial.print(" µg/m3 ");
  Serial.print("PM10: ");
  Serial.print(msg_pm10);
  Serial.println(" µg/m3 ");

  return pm;
}

void turnoff() {
  SDS011.flush();
  for (uint8_t j = 0; j < 19; j++) SDS011.write(SLEEPCMD[j]);
}

void turnon() {
  SDS011.write(0x01);
}
