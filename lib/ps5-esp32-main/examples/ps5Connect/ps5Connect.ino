#include <ps5Controller.h>

void setup() {
  Serial.begin(115200);

  ps5.begin("7c:66:ef:77:8a:4f"); //replace with MAC address of your controller
  Serial.println("Ready.");
}

void loop() {
  if (ps5.isConnected()) {
    Serial.println("Connected!");
  }

  delay(3000);
}
