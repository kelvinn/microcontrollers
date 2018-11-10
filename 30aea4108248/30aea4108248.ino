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
#define CLIENT_ID "30aea4108248"
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
char msg[50];
char topic[50];
char msg_h[50];
char msg_aqi[50];
int value = 0;

uint64_t chipid; 

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 30000;  //the value is a number of milliseconds

DHT dht(DHTPIN, DHTTYPE);

void setup() {
   
  Serial.begin(115200);
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  startMillis = millis();  //initial start time

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

void createHassDevices() {

  char const *temperature_config[] = { "homeassistant/sensor/temperature/config", "{\"name\": \"Test Temperature\"}"};
  client.publish(temperature_config[0], temperature_config[1]);

  char const *humidity_config[] = { "homeassistant/sensor/humidity/config", "{\"name\": \"Test Humidity\"}"};
  client.publish(humidity_config[0], humidity_config[1]);

  //char topic[50] = "homeassistant/sensor/living/temperature/config";
  //char msg[25] = "{\"name\": \"th\"}";
  Serial.println("Creating HASS node");

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

struct values {
   float temperature;
   float humidity;
};

void loop() {
  // Wait a few seconds between measurements.
  delay(10000);
  //run_temp_hum_dht11();
  //int a, int b = get_temp_hum_dht11();

  struct values theValues;
  theValues = get_temp_hum_dht11();
  Serial.println(theValues.temperature);
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    createHassDevices();
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
  //run_aqi();

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
        temp_hum.temperature = 0.0;
        temp_hum.humidity = 0.0;
        return temp_hum;
    }
    
    temp_hum.temperature = t;
    temp_hum.humidity = h;
    
    Serial.print("Sending to MQTT...");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

    dtostrf(t,7, 3, msg_t);
    dtostrf(h,7, 3, msg_h);
    
    //client.publish("homeassistant/sensor/temperature/state", msg_t);
    //client.publish("homeassistant/sensor/humidity/state", msg_h);
    return temp_hum;
}