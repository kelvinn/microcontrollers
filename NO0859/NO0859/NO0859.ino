#include <SoftwareSerial.h>

SoftwareSerial esp8266(0,1);

char const *HELLO_WORLD_CONFIG[] = { "homeassistant/sensor/bathroom/temperature/config", "{\"name\": \"Bathroom Temperature\", \"unit_of_measurement\": \"C\" }"};
char const *hw_state[] = { "homeassistant/sensor/living/test/state", "25.0" };

// Send Data From Arduino to ESP8266 via Serial @ baud rate 9600
void setup() {
    
    Serial.begin(9600);
    esp8266.begin(9600);
}

void loop() {

    esp8266.println("Hi!");
    //esp8266.print((char *)hw_state);
    //Serial.print(HELLO_WORLD_CONFIG);
    //esp8266.print((char *)HELLO_WORLD_CONFIG);
    delay(1000);
}
