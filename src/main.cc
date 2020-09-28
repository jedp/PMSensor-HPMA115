/*
 * Render the AQI as blinks-per-minute on the LED.
 *
 *    Tempo            AQI
 * ==============================================
 * Larghissimo    = Good
 * Largo          = Moderate
 * Allegro        = Unsafe for sensitive groups
 * Prestissimo    = Unhealthy
 */

#ifndef UNIT_TEST

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

// STM32 Nucleo pinout
#define UART_TX PB6
#define UART_RX PB7
#define LED     PB3

// A channel for interacting with the sensor.
SoftwareSerial hpmSerial(UART_TX, UART_RX);

// Remember that there are two varieties of the Honeywell HPM115:
// A "standard" and a "compact". They return different auto-read data messages.
HPMA115_Compact hpm = HPMA115_Compact();

uint32_t lastBlink = 0;
uint32_t lastCheck = 0;
uint32_t blinkIntervalMS;
uint32_t checkIntervalMS = 1000;
uint32_t calculateBlinkIntervalMS(uint16_t aqi);

void setup() {
  hpmSerial.begin(HPMA115_BAUD);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Configure the HPM device to use our data stream.
  // (Note carefully the '&' in the next line.)
  hpm.begin(&hpmSerial);

  delay(1000);

  // When the device powers up, it goes directly into auto-send mode.
  while (!hpm.isNewDataAvailable()) {}

  blinkIntervalMS = calculateBlinkIntervalMS(hpm.getAQI());
}

void loop() {
  if ((millis() - lastBlink) > blinkIntervalMS) {
    lastBlink = millis();
    digitalWrite(LED, HIGH);
  } else if ((millis() - lastBlink) > blinkIntervalMS / 2) {
    digitalWrite(LED, LOW);
  }

  // The sensor only sends readings at intervals of one second.
  // There is no point in hammering it with data requests.
  if (millis() - lastCheck > checkIntervalMS) {
    if (hpm.isNewDataAvailable()) {
      blinkIntervalMS = calculateBlinkIntervalMS(hpm.getAQI());
    }
  }
}

uint32_t calculateBlinkIntervalMS(uint16_t aqi) {
  return 1000 * aqi / 60;
}

#endif

