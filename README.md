# Honeywell HPMA115 series Particulate Matter Sensors



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
AQI 25 (PM 2.5)   PM 1.0 = 4, PM 2.5 = 6, PM 4.0 = 7, PM 10.0 = 9
AQI 25 (PM 2.5)   PM 1.0 = 4, PM 2.5 = 6, PM 4.0 = 8, PM 10.0 = 9
AQI 25 (PM 2.5)   PM 1.0 = 4, PM 2.5 = 6, PM 4.0 = 8, PM 10.0 = 10
...
```

## Testing

Run unit tests with:

```pio test -e native```
