#include <Arduino.h>

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Hello from your Cyber B Necklace!");
}

void loop() {
  // The loop() function runs over and over again forever.
  // For this example, we don't need to do anything here.
  delay(1000);
  Serial.println("Still here!");
}
