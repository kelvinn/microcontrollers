/*
ESP32 located in my kitchen with various sensors.
Reporting back to MQTT and Home Assistant
*/
#include <Arduino.h>

#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "configuration.h" // This is the configuration file with passwords and stuff

#define DHTPIN 16     // what pin we're connected to
#define CLIENT_ID "30aea4181060"
#define AQIPIN 34

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


// Initialize the Ethernet client object
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg_t[50];
char msg_h[50];
char msg_aqi[50];
int value = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
   
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(10000);
  run_temp_hum_dht11();
  run_aqi();

}

void run_temp_hum_dht11() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  Serial.print("Sending to MQTT...");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  dtostrf(t,7, 3, msg_t);
  dtostrf(h,7, 3, msg_h);
  
  //client.publish("home/bedroom/temperature", msg_t);
  //client.publish("home/bedroom/humidity", msg_h);
  
}

void run_aqi() {

    int aqi = analogRead(AQIPIN);
    
    Serial.print("AQI: ");
    Serial.println(aqi);
    if (aqi > 0) {
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
      dtostrf(aqi,7, 3, msg_aqi);
      client.publish("home/kitchen/aqi", msg_aqi);
    }



}
