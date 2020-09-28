#ifndef UNIT_TEST

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <hpm/HPMA115_Compact.h>

// Uncomment and/or change for Arduino uno
// #define UART_TX 2
// #define UART_RX 3
// Uncomment and/or change for STM32 Nucleo
#define UART_TX PB4
#define UART_RX PB3

// A channel for interacting with the sensor.
SoftwareSerial hpmSerial(UART_TX, UART_RX);

// Remember that there are two varieties of the Honeywell HPM115:
// A "standard" and a "compact". They return different auto-read data messages.
HPMA115_Compact hpm = HPMA115_Compact();

// Result of attempt to read data from the sensor.
uint8_t err = 0;

void setup() {
  Serial.begin(HPMA115_BAUD);
  hpmSerial.begin(HPMA115_BAUD);

  // Configure the HPM device to use our data stream.
  // (Note carefully the '&' in the next line.)
  hpm.begin(&hpmSerial);

  delay(1000);
}

void loop() {
  if (hpm.checkAutoReceive() == NEW_DATA) {
    // No error, so the data was updated automatically for us. Yay.
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

  // The sensor only sends readings at intervals of one second.
  // There is no point in hammering it with data requests.
  delay(1000);
}

#endif

