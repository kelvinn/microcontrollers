#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "configuration.h" // This is the configuration file with passwords and stuff

#define CLIENT_ID "00196FEC"

String arduinoResp;

// Initialize the Ethernet client object
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
   Serial.begin(9600);  
   Serial.printf(" ESP8266 Chip id = %08X\n", ESP.getChipId());

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

void loop() {
    if (!client.connected()) {
      reconnect();
    }
    if(Serial.available()) {
        Serial.println("Serial ready.");
        //arduinoResp = Serial.readStringUntil('\n');
        client.publish("kelvin/test/hi", "Serial ready");
        //client.publish("kelvin/test/hi", Serial.readString());
    
    }
    client.loop();
}
