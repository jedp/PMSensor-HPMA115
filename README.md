[![Build Status](https://travis-ci.org/jedp/PMSensor-HPMA115.svg?branch=master)](https://travis-ci.org/jedp/PMSensor-HPMA115)

# Honeywell HPMA115 series Particulate Matter Sensors

Library and example for receiving data from and interacting with Honeywell HPM
series particulate matter sensors using the Arduino platform.

Includes calculation of AQI (air quality index).

tl;dr:

```C++
HPMA115_Compact hpm = HPMA115_Compact();
hpm.begin(&serial);

if (hpm.isNewDataAvailable()) {
  // use hpm.getPM25(), hpm.getAQI(), etc.
}

```

## Compatibility

Please note that this code works with the "Compact" series of HPMA115 sensors.
It will not work with the "Standard" series. I do not have a Standard module,
but would be happy to collaborate with anyone who does.

## Using the Library

This library is available via the Arduino Library Manager. To make it part of
your library collection, simply do the following in the Arduino sketch editor:

- Select **Sketch** &#8594; **Include Library** &#8594; **Manage Libraries ...**
- In the Library Manager, search for HPM or AQI and the library will be listed.
- Tap the **Install** button.

Now for your sketch you can select **Sketch** &#8594; **Include Library** and
find `PMSensor-HPMA115` under **Contributed libraries**

You can also open and build the examples from **File** &#8594; **Examples** and
then in the "Examples from Custom Libraries" section, choose
**PMSensor-HPMA115** to open any of the examples.

## Commands

- `isNewDataAvailable()` Poll for new sensor data. Do this no more than once
  per second.

- `readParticleMeasurementResults()` Read the current sensor results. This
  is an alternative to using auto-send and polling with `isNewDataAvailable()`.
  (But see the note on Efficiency below.)

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

### A Note about Efficiency

The HPM API offers two different approaches for collecting the current reading,
direct readings (`readParticleMeasurementResults()`) and auto-send
(`isNewDataAvailable()`). The former may be more intiutive -- it feels so like
a simple assignment, `x = y` -- but it comes at a huge cost.

On my setup, it takes about 25 milliseconds to execute
`readParticleMeasurementResults()`; that's 25ms during which your program will
be frozen while waiting for data. It takes so long because the command sends
data to the HPM and then sits and waits for the HPM to post a response back on
the UART. Depending on your application, that could be quite noticeable.

With the auto-send approach, by contrast, it takes only 70 microseconds to
execute `isNewDataAvailable()`. That's 350 times faster! It's fast because it
is capturing data that's already sitting there on the serial bus, waiting to be
picked up. There is no laggy back-and-forth between your program and the HPM.
So if performance matters, you will want to find a way to use the auto-send
approach.

## Platform.IO Commands

This project is structured to conform to the Arduino library structure, but it
is developed using [Platform.IO](https://platformio.org/).

If you are not using Platform.IO, ignore this part.

### Build and upload

Using one of the boards specified in the `platformio.ini` file, run, e.g.,

```
pio run -e nucleo_f303k8 --target upload
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

## Wiring

Consult Table 3 of the datasheet for the [HPM Sensors].

Note that the data sheet specifies the part number for the actul wiring
assemblies you need in Figure 3 and Figure 4.

For the compact sensor, I recommend ordering:

- [The wiring assembly](https://www.digikey.com/product-detail/en/samtec-inc/SFSD-05-28-H-05.00-SR/SAM8662-ND/1785913)
- [Breadboard-friendly terminal blocks](https://www.digikey.com/product-detail/en/phoenix-contact/1990083/277-1802-ND/950930) to connect the wires.

## To-Do

- Finish implementing the API (set/read adjustment)
- Support for non-compact HPM "Standard"

## About

Written by Jed Parsons. Distributed under the BSD license. Read `license.txt`
for details. All text above must be included in any redistribution.

