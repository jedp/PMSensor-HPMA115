#ifndef UNIT_TEST

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

// STM32F303:
#define UART_TX PB4  // to STM Compact pin 9
#define UART_RX PB3  // to STM Compact pin 7

// A channel for interacting with the sensor.
SoftwareSerial hpmSerial(UART_TX, UART_RX);

// Remember that there are two varieties of the Honeywell HPM115:
// A "standard" and a "compact". They return different auto-read data messages.
HPMA115_Compact hpm = HPMA115_Compact();

// Result of attempt to read data from the sensor.
uint8_t err = 0;

// Helper function to print current measurement results.
void printResults();

void setup() {
  Serial.begin(HPMA115_BAUD);
  Serial.println("Hello");

  hpmSerial.begin(HPMA115_BAUD);

  // Configure the HPM device to use our data stream.
  // (Note carefully the '&' in the next line.)
  hpm.begin(&hpmSerial);

  delay(1000);

  // When the device powers up, it goes directly into auto-send mode.
  while (!hpm.isNewDataAvailable()) {
    Serial.println("Still waking up ...");
    delay(1000);
  }

  // We can disable auto-send if we want.
  hpm.stopAutoSend();

  // And we can fetch readings manually.
  hpm.readParticleMeasurementResults();

  Serial.println("Manually fetching result:");
  printResults();

  // Let's set this back to audo-send. We'll catch the readings in the loop().
  hpm.startAutoSend();

  Serial.println("Looping ...");
}

void loop() {
  if (hpm.isNewDataAvailable()) {
    printResults();
  }

  // The sensor only sends readings at intervals of one second.
  // There is no point in hammering it with data requests.
  delay(1000);
}

void printResults() {
  Serial.print("AQI ");
  Serial.print(hpm.getAQI());
  Serial.print("  PM 1.0 = ");
  Serial.print(hpm.getPM1());
  Serial.print(", PM 2.5 = ");
  Serial.print(hpm.getPM25());
  Serial.print(", PM 4.0 = ");
  Serial.print(hpm.getPM4());
  Serial.print(", PM 10.0 = ");
  Serial.println(hpm.getPM10());
}

#endif

