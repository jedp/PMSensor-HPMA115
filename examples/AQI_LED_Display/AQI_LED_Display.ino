/*
 * Render the AQI on a 7-segment LED display using an Adafruit LED Backpack.
 *
 * Requires the Adafruit LED Backpack Library.
 */

#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>
#include <Adafruit_LEDBackpack.h>

// Possible pins for connecting to the HPM.
#define UART_TX 2       // to HPM Compact pin 9
#define UART_RX 3       // to HPM Compact pin 7
#define ADDR_LEDS 0x70  // I2C address of the Adafruit LED Backpack

// A channel for interacting with the sensor.
// For boards that have multiple hardware serial UARTs, consider
// making this a hardware serial.
SoftwareSerial hpmSerial(UART_TX, UART_RX);

// Remember that there are two varieties of the Honeywell HPM115:
// A "standard" and a "compact". They return different auto-read data messages.
HPMA115_Compact hpm = HPMA115_Compact();

// Use 5V power in. Connect the C and D pins to I2C Clock (SCL) and Data (SDA).
Adafruit_7segment display = Adafruit_7segment();

void setup() {
  display.begin(ADDR_LEDS);
  Serial.begin(HPMA115_BAUD);
  hpmSerial.begin(HPMA115_BAUD);

  // Configure the HPM device to use our data stream.
  // (Note carefully the '&' in the next line.)
  hpm.begin(&hpmSerial);

  delay(1000);

  // When the device powers up, it goes directly into auto-send mode.
  while (!hpm.isNewDataAvailable()) {
    delay(500);
  }
}

void loop() {
  if (hpm.isNewDataAvailable()) {
    // Print the AQI on the LED display.
    display.print(hpm.getAQI(), DEC);
    display.writeDisplay();

    // Might be interesting to see all the data on the Serial console.
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

