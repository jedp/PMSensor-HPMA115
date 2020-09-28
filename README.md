[![Build Status](https://travis-ci.org/jedp/PMSensor-HPMA115.svg?branch=master)](https://travis-ci.org/jedp/PMSensor-HPMA115)

# Honeywell HPMA115 series Particulate Matter Sensors

Library and example for receiving data from and interacting with Honeywell HPM
series particulate matter sensors using the Arduino platform.

Includes calculation of AQI (air quality index).

tl;dr:

```
HPMA115_Compact hpm = HPMA115_Compact();
hpm.begin(&serial);

if (hpm.isNewDataAvailable()) {
  // use hpm.getPM25(), hpm.getAQI(), etc.
}

```

## Commands

- `isNewDataAvailable()` Poll for new sensor data. Do this no more than once
  per second.

- `readParticleMeasurementResults()` Read the current sensor results. This
  is an alternative to using auto-send and polling with `isNewDataAvailable()`.

- `getAQI()` Get the current Air Quality Index value.

- `getPM1()` Get the current PM 1.0 quantity.

- `getPM25()` Get the current PM 2.5 quantity.

- `getPM4()` Get the current PM 4 quantiy.

- `getPM10()` Get the current PM 10 quantity

- `stopParticleMeasurement()` Stop the HPM device from collecting readings.
  (Also switches off the fan.)

- `startParticleMeasurement()` Start collecting readings.

- `stopAutoSend()` Stop the HPM from automatically sending readings every
  second.

- `startAutoSend()` Start sending autoatic readings.

When the HPM cold starts, it will automatically begin collecting readings and
sending them every second.

## Examples

You can open the sketches in `examples` directly in your Arduino IDE.

If you are using Platform.IO, look for `main.cc` in the `src` dir.

Here are the guts of two example Arduino sketches to read data from
the HPMA115 Compact sensor. The first waits for the sensor to send data
automatically. The second reads it directly.

### Receiving auto-send data

```C++
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

// Create a serial stream to communicate with the physical HPM.
// Arduino pins 2 and 3 to send and receive, as an example.
SoftwareSerial hpmSerial(2, 3);

// Create an object to communicate with the HPM Compact sensor.
HPMA115_Compact hpm = HPMA115_Compact();

void setup() {
  // Console serial.
  Serial.begin(HPMA115_BAUD);
  // Serial for ineracting with HPM device.
  hpmSerial.begin(HPMA115_BAUD);

  // Configure the hpm object to refernce this serial stream.
  // Note carefully the '&' in this line.
  hpm.begin(&hpmSerial);
}

// In the loop, we can just poll for new data since the device automatically
// enters auto-send mode on startup.
void loop() {
  if (hpm.isNewDataAvailable()) {
    Serial.print("AQI: ");
    Serial.print(hpm.getAQI());
    Serial.print("  PM 2.5 = ");
    Serial.print(hpm.getPM25());
    // etc.
    Serial.println();
  }

  // The physical sensor only sends data once per second.
  delay(1000);
}
```

### Reading data without auto-send

Same results, different approach.

```C++
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

SoftwareSerial hpmSerial(2, 3);
HPMA115_Compact hpm = HPMA115_Compact();
unsigned long lastCheck = 0;

void setup() {
  Serial.begin(HPMA115_BAUD);
  hpmSerial.begin(HPMA115_BAUD);

  hpm.begin(&hpmSerial);

  // One way to wait for the device to be ready.
  while(!hpm.isNewDataAvailable()) {}

  hpm.stopAutoSend();
}

void loop() {

  // It still takes the device 1 second to update its readings, so there's
  // no point in hammering on this function call any more than once per
  // second. All you will do is burn CPU.
  if (millis() - lastCheck > 1000) {
    lastCheck = mills();

    if (hpm.readParticleMeasurementResults()) {
      // Can now hpm.getAQI() etc.
    }
  }
}
```

## Wiring

Consult Table 3 of the datasheet for the [HPM Sensors].

## Platform.IO Commands

This project is structured to conform to the Arduino library structure, but it
is developed using [Platform.IO](https://platformio.org/).

If you are not using Platform.IO, ignore this part.

### Build and upload

Using one of the platforms specified in the `platformio.ini` file, run, e.g.,

```
pio run -e nucleo_f303k8 --target upload
```

### Printing AQI to the console

(If you are using the Arduino editor, and not PIO command-line tools, just open
the serial window in the Arduio editor, set Baud to 9600, and you should be
able to see the same output.)

```
pio run -e nucleo_f303k8 --target upload
pio device monitor
```

This will stream the current AQI together with the particle readings. Example:

```
AQI 29  PM 1.0 = 4, PM 2.5 = 7, PM 4.0 = 9, PM 10.0 = 11
AQI 29  PM 1.0 = 5, PM 2.5 = 7, PM 4.0 = 9, PM 10.0 = 11
AQI 29  PM 1.0 = 5, PM 2.5 = 7, PM 4.0 = 10, PM 10.0 = 11
...
```

### Testing

Run unit tests with:

```pio test -e native```

## HPM Sensors

- [HPMA115S0-XXX](https://www.digikey.com/product-detail/en/honeywell-sensing-and-productivity-solutions/HPMA115S0-XXX/480-7035-ND/7202204)
  Standard (larger) sensor. Senses PM2.5 and PM10 levels.

- [HPMA115C0-003](https://www.digikey.com/product-detail/en/honeywell-sensing-and-productivity-solutions/HPMA115C0-003/480-HPMA115C0-003-ND/10427615)
  Compact. Senses PM1.0, 2.5, 4.0, and 10. Air intake and exhaust
  are on the same side of the device.

- [HPMA115C0-004](https://www.digikey.com/product-detail/en/honeywell-sensing-and-productivity-solutions/HPMA115C0-004/480-HPMA115C0-004-ND/10427622)
  Compact. Senses PM1.0, 2.5, 4.0, and 10. Air intake and exhaust
  are on opposite sides of the device.

## To-Do

- Finish implementing the API (synchronous commands)
- Support for non-compact HPM "Standard"

## About

Written by Jed Parsons. Distributed under the BSD license. Read `license.txt`
for details. All text above must be included in any redistribution.

