#include <Arduino.h>
#include <Serial.h>

#include "./BTLE.h"

RF24 radio(3, 4);

BTLE btle(&radio);

uint8_t buffer[1];
void setup() {

  Serial.begin(9600);
  // while (!Serial) { }
  Serial.println("BTLE advertisement sender");

  btle.begin("foobar");

  buffer[0] = 0;
}

void loop() {
  buffer[0]++;
  btle.advertise(buffer, 1);
  btle.hopChannel();
  Serial.print(".");
}

