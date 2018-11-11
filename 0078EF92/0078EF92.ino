#include <Adafruit_ADS1015.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <elapsedMillis.h>

#include "configuration.h" // This is the configuration file with passwords and stuff

#define CLIENT_ID "0078EF92"

Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;
float Voltage_1 = 0.0;
char msg_voltage[10];
char msg_voltage_1[10];

// TO keep track of time 
elapsedMillis timeSinceCreated;
elapsedMillis timeSinceRead;
unsigned int thirtySecInterval = 30000;
unsigned int tenSecInterval = 10000;

// Initialize the Ethernet client object
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf(" ESP8266 Chip id = %08X\n", ESP.getChipId());
  ads.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
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

void createHassDevices() {
  client.loop();
  Serial.println("Creating HASS nodes...");
  
  char const *mq4_config[] = { "homeassistant/sensor/living/mq4/config", "{\"name\": \"Living Room Methane\", \"unit_of_measurement\": \"V\" }"};
  client.publish(mq4_config[0], mq4_config[1]);

  char const *mq6_config[] = { "homeassistant/sensor/living/mq6/config", "{\"name\": \"Living Room Propane\", \"unit_of_measurement\": \"V\" }"};
  client.publish(mq6_config[0], mq6_config[1]);
  
}

void loop() {
  int16_t adc0;  //mq6
  int16_t adc1;  //mq4


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

    adc0 = ads.readADC_SingleEnded(0);
    Voltage = (adc0 * 0.1875)/1000;
    
    Serial.print("AIN0: "); 
    Serial.print(adc0);
    Serial.print("\tVoltage: ");
    Serial.println(Voltage, 7);  
    Serial.println();


    dtostrf(Voltage, 7, 3, msg_voltage);
    client.publish("homeassistant/sensor/living/mq6/state", msg_voltage);

    adc1 = ads.readADC_SingleEnded(1);
    Voltage_1 = (adc1 * 0.1875)/1000;
    
    Serial.print("AIN1: "); 
    Serial.print(adc1);
    Serial.print("\tVoltage: ");
    Serial.println(Voltage_1, 7);  
    Serial.println();

    dtostrf(Voltage_1, 7, 3, msg_voltage_1);
    client.publish("homeassistant/sensor/living/mq4/state", msg_voltage_1);
    
		timeSinceRead = 0;
	}

  client.loop();
}
