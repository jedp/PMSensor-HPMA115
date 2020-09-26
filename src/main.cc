#ifndef UNIT_TEST

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <PMSensor-HPMA115.h>
#include <aqi.h>

#define TX_TO_HPM PB4
#define RX_FROM_HPM PB3

SoftwareSerial hpma(PB4, PB3);

uint16_t frame_length;
uint16_t pm1_0;
uint16_t pm2_5;
uint16_t pm4_0;
uint16_t pm10_0;
uint16_t checksum;

unsigned long last_report_ms = 0;

void setup() {

  Serial.begin(HPMA115_BAUD);
  while(!Serial);
  Serial.println("Serial ok");

  hpma.begin(HPMA115_BAUD);
  while(!hpma);
  Serial.println("HPM ok");

  delay(100);
}

compact_autosend_state_t state = UNKNOWN;
uint8_t idx = 0;
bool reported = false;

void loop() {
  while (hpma.available() < 1) ;

  uint8_t byte = hpma.read();

  switch (state) {
    case HEAD_0:
      {
        // Loop in this state until we encounter the second header byte.
        if (byte == 0x42) {
          state = HEAD_1;
        }
        break;
      }
    case HEAD_1:
      {
        if (byte == 0x4D) {
          state = LEN_H;
        } else {
          // The last byte wasn't a head byte after all.
          // Start back at the beginning.
          state = HEAD_0;
          idx = 0;
        }
        break;
      }
    case LEN_H:
      {
        // No reason to care about this value.
        state = LEN_L;
        break;
      }
    case LEN_L:
      {
        // No reason to care about this value.
        state = DATA0_H;
        break;
      }
    case DATA0_H:
      {
        pm1_0 = 256 * (uint16_t) byte;
        state = DATA0_L;
        break;
      }
    case DATA0_L:
      {
        pm1_0 += byte;
        state = DATA1_H;
        break;
      }
    case DATA1_H:
      {
        pm2_5 = 256 * (uint16_t) byte;
        state = DATA1_L;
        break;
      }
    case DATA1_L:
      {
        pm2_5 += byte;
        state = DATA2_H;
        break;
      }
    case DATA2_H:
      {
        pm4_0 = 256 * (uint16_t) byte;
        state = DATA2_L;
        break;
      }
    case DATA2_L:
      {
        pm4_0 += byte;
        state = DATA3_H;
        break;
      }
    case DATA3_H:
      {
        pm10_0 = 256 * (uint16_t) byte;
        state = DATA3_L;
        break;
      }
    case DATA3_L:
      {
        pm10_0 += byte;
        state = DATA_RESERVED;
        break;
      }
    case DATA_RESERVED:
      {
        // Keep looping in this state until we read the first checksum byte.
        if (idx == CS_H) {
          state = CS_H;
        }
        break;
      }
    case CS_H:
      {
        checksum = 256 * (uint16_t) byte;
        state = CS_L;
        break;
      }
    case CS_L:
      {
        checksum += byte;
        idx = 0;
        state = HEAD_0;
        reported = false;
        break;
      }
    case UNKNOWN:
      {
        // Loop here until we encounter the first header byte.
        if (byte == 0x42) {
          state = HEAD_0;
        }
        break;
      }
  }

  if (state == HEAD_0) {
    idx = 0;
  } else {
    idx++;
  }

  if (!reported && state == CS_L) {
    reported = true;
    uint32_t aqi25 = aqi_pm25(pm2_5);
    uint32_t aqi10 = aqi_pm10(pm10_0);
    last_report_ms = millis();
    Serial.print("AQI ");
    if (aqi25 > aqi10) {
      Serial.print(aqi25);
      Serial.print(" (PM 2.5) ");
    } else {
      Serial.print(aqi10);
      Serial.print(" (PM 10.0) ");
    }
    Serial.print("  PM 1.0 = ");
    Serial.print(pm1_0);
    Serial.print(", PM 2.5 = ");
    Serial.print(pm2_5);
    Serial.print(", PM 4.0 = ");
    Serial.print(pm4_0);
    Serial.print(", PM 10.0 = ");
    Serial.println(pm10_0);
  }
}

#endif

