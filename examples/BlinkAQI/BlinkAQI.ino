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

#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

// Possible pins for connecting to the HPM.
#define UART_TX 2
#define UART_RX 3

// The default Arduno LED pin
#define LED 13

// A channel for interacting with the sensor.
SoftwareSerial hpmSerial(UART_TX, UART_RX);

// Remember that there are two varieties of the Honeywell HPM115:
// A "standard" and a "compact". They return different auto-read data messages.
HPMA115_Compact hpm = HPMA115_Compact();

unsigned long lastBlink = 0;
unsigned long lastCheck = 0;
unsigned long blinkIntervalMS;
unsigned long checkIntervalMS = 1000;
unsigned long calculateBlinkIntervalMS(uint16_t aqi);

void setup() {
  Serial.begin(HPMA115_BAUD);
  hpmSerial.begin(HPMA115_BAUD);

  pinSetup(LED, OUTPUT);
  digitalWrite(LOW, LED);

  // Configure the HPM device to use our data stream.
  // (Note carefully the '&' in the next line.)
  hpm.begin(&hpmSerial);

  delay(1000);

  // When the device powers up, it goes directly into auto-send mode.
  while (!hpm.isNewDataAvailable()) {}

  blinkInterval = calculateBlinkInterval(hpm.getAQI());
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

unsigned long calculateBlinkIntervalMS(uint16_t aqi) {
  return 1000 * aqi / 60;
}

