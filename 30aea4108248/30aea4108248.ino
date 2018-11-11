
/*
ESP32 located in my kitchen with various sensors.
Reporting back to MQTT and Home Assistant
*/
#include <Arduino.h>

#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <elapsedMillis.h>

#include "configuration.h" // This is the configuration file with passwords and stuff

#define CLIENT_ID "30aea4108248"

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


// Initialize the Ethernet client object
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;

char msg[50];
char topic[50];
char char_light[10];

int value = 0;
elapsedMillis timeSinceCreated;
elapsedMillis timeSinceRead;

uint64_t chipid; 

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned int thirtySecInterval = 30000;
unsigned int tenSecInterval = 10000;
const unsigned long period = 30000;  //the value is a number of milliseconds

struct values {
   char temperature[10];
   char humidity[10];
};

struct pm_values {
   char pm25[10];
   char pm10[10];
};

void setup() {
   
  Serial.begin(115200);
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  startMillis = millis();  //initial start time

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  initSensors();
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
  client.loop();
  Serial.println("Creating HASS nodes...");
  
  char const *aqi_config[] = { "homeassistant/sensor/kitchen/aqi/config", "{\"name\": \"Kitchen Air Quality\", \"unit_of_measurement\": \"V\" }"};
  client.publish(aqi_config[0], aqi_config[1]);

  char const *temperature_config[] = { "homeassistant/sensor/kitchen/onewire/config", "{\"name\": \"Kitchen Temperature\", \"unit_of_measurement\": \"°C\" }"};
  client.publish(temperature_config[0], temperature_config[1]);

  char const *light_config[] = { "homeassistant/sensor/kitchen/light/config", "{\"name\": \"Kitchen Light\", \"unit_of_measurement\": \"V - Light\" }"};
  client.publish(light_config[0], light_config[1]);

  char const *pm25_config[] = { "homeassistant/sensor/kitchen/pm25/config", "{\"name\": \"Kitchen PM 2.5\", \"unit_of_measurement\": \"µg/m3\" }"};
  client.publish(pm25_config[0], pm25_config[1]);

  char const *pm10_config[] = { "homeassistant/sensor/kitchen/pm10/config", "{\"name\": \"Kitchen PM 10\", \"unit_of_measurement\": \"µg/m3\" }"};
  client.publish(pm10_config[0], pm10_config[1]);

  //char const *pm_status_config[] = { "homeassistant/binary_sensor/kitchen/packet_status/config", "{\"name\": \"Kitchen PM Status\", \"unit_of_measurement\": \"µg/m3\", \"device_class\": \"problem\",   }"};
  //client.publish(pm_status_config[0], pm_status_config[1]);
  
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

  // Create HASS nodes
	if (timeSinceCreated > thirtySecInterval) 
	{				
    createHassDevices();
		timeSinceCreated = 0;
	}


	if (timeSinceRead > tenSecInterval) 
	{				
    if (!client.connected()) {
      reconnect();
    }
    
    //struct values tempHumValues;
    //tempHumValues = get_temp_hum_dht11();

    // Publish to MQTT
    //client.publish("homeassistant/sensor/kitchen/temperature/state", tempHumValues.temperature);
    //client.publish("homeassistant/sensor/kitchen/humidity/state", tempHumValues.humidity);

    char* aqi = get_aqi();
    client.publish("homeassistant/sensor/kitchen/aqi/state", aqi);

    char* light = get_light();
    client.publish("homeassistant/sensor/kitchen/light/state", light);

    char* temperature = get_temperature();
    client.publish("homeassistant/sensor/kitchen/onewire/state", temperature);

    struct pm_values tempPMValues;
    tempPMValues = get_pm();

    // Publish to MQTT
    client.publish("homeassistant/sensor/kitchen/pm25/state", tempPMValues.pm25);
    client.publish("homeassistant/sensor/kitchen/pm10/state", tempPMValues.pm10);
		timeSinceRead = 0;
	}

  client.loop();
}
