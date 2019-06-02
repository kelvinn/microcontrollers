#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX on Arduino
void setup() {
 Serial.begin(115200);
 mySerial.begin(115200);
}
void loop() {
 if (mySerial.available()) {
  String msg = mySerial.readString();
  Serial.print("Data received: ");
  Serial.println(msg);
 }
}
