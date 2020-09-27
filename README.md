# Honeywell HPMA115 series Particulate Matter Sensors

Library and example for receiving data from and interacting with Honeywell HPM
series particulate matter sensors.

Includes calculation of AQI (air quality index).

## Usage

See `main.cc` for a working example that streams AQI data over your serial
console.

Here are the guts of an example Arduino sketch to receive data from the HPMA115
Compact sensor:

```
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HPMA115_Compact.h>

// Create a serial stream to communicate with the physical HPM.
// Arduino pins 2 and 3 to send and receive, as an example.
SoftwareSerial hpmSerial(2, 3);

// Create an object to communicate with the HPM Compact sensor.
HPMA115_Compact hpm = HPMA115_Compact();

// Create an object to store the result data that the sensor will fill.
compact_auto_result_t data;

void setup() {
  // Console serial.
  Serial.begin(HPMA115_BAUD);
  // Serial for ineracting with HPM device.
  hpmSerial.begin(HPMA115_BAUD);

  // Configure the hpm object to refernce this serial stream and data object.
  // Note carefully the '&' characters in this line.
  hpm.begin(&hpmSerial, &data);
}

void loop() {
  if (hpm.receive() == NEW_DATA) {
    Serial.print("AQI: ");
    Serial.print(data.aqi);
    Serial.print("  PM 2.5 = ");
    Serial.print(data.aqi25);
    // etc.
    Serial.println();
  }

  // The physical sensor only sends data once per second.
  delay(1000);
}
```

## Wiring

Consult Table 3 of the datasheet for the [HPM Sensors].

## Build and upload

Using one of the platforms specified in the `platformio.ini` file, run, e.g.,

```
pio run -e nucleo_f303k8 --target upload
```

## Printing AQI to the console

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

## Testing

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

