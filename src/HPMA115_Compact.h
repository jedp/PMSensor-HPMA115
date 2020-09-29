#ifndef SRC_HPMA115_COMPACT_H_
#define SRC_HPMA115_COMPACT_H_

#ifdef UNIT_TEST
#include "fake/FakeStream.h"
#else
#include <Stream.h>
#endif

#include <stddef.h>
#include "hpm/HPMA115_Common.h"

/*
 * Byte sequence for the autosend data format from the compact series,
 * HPMA115C0-003 and HPMA115C0-004. (Table 7 in the Honeywell data sheet.)
 *
 * Index of bytes, omitting the two header bytes.
 */
typedef enum {
  LEN_H = 0,
  LEN_L,
  DATA0_H,
  DATA0_L,
  DATA1_H,
  DATA1_L,
  DATA2_H,
  DATA2_L,
  DATA3_H,
  DATA3_L,
  CS_H = 28,
  CS_L,
  AUTO_SEND_DATA_BYTES,
} compact_autosend_state_t;

/*
 * A reading from the compact sensor.
 */
typedef struct {
  uint16_t pm1;
  uint16_t pm25;
  uint16_t pm4;
  uint16_t pm10;
  uint16_t aqi;
  uint16_t checksum;
} compact_auto_result_t;

class HPMA115_Compact {
 public:
  HPMA115_Compact();

  /*
   * Configure the instance to use this stream to communicate with the STM.
   */
  bool begin(Stream *stream);

  /*
   * Check for data received. Return true if new data has arrived, false
   * otherwise.
   *
   * When it powers on, the HPM automatically starts collecting measurements
   * and sending them every second (auto send). Calling this command every
   * second will let you capture that stream of readings.
   *
   * This command takes on the order of 70 microseconds to execute.
   * It's pretty fast, because it's plucking ready data off the UART.
   *
   * If this returns true, then you can access the new data by calling
   * the getPM25() or  getAQI(), etc.
   */
  bool isNewDataAvailable();

  /*
   * Read the current measurement from the HPM.
   *
   * Blocks until the HPM responds either with an error code, or with the
   * result data.
   *
   * Note that this command takes on the order of 25 milliseconds to
   * complete. That could be a huge amount of time to be blocking your
   * microcontroller from doing anything else. You may wish to consider
   * using auto-send and `isNewDataAvailable()` if this is problematic.
   *
   * As an alternative to requesting data this way, you can leave the device
   * in auto-send mode, and call isNewDataAvailable() every second.
   */
  bool readParticleMeasurementResults();

  /*
   * Tell the HPM to stop collecting measurements. This will also cause the
   * HPM fan to stop.
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool stopParticleMeasurement();

  /*
   * Tell the HPM to begin measureing particles.
   *
   * When it powers on, the HPM automatically starts collecting measurements
   * and sending them every second (auto send).
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool startParticleMeasurement();

  /*
   * Stop the HPM from automatically sending results.
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool stopAutoSend();

  /*
   * Tell the HPM to start sending results automatically.
   *
   * When it powers on, the HPM will automatically enter auto-send mode
   * on its own.
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool startAutoSend();

  /* Get the most recent PM 1.0 reading. */
  uint16_t getPM1();

  /* Get the most recent PM 2.5 reading. */
  uint16_t getPM25();

  /* Get the most recent PM 4.0 reading. */
  uint16_t getPM4();

  /* Get the most recent PM 10.0 reading. */
  uint16_t getPM10();

  /*
   * Get the most recent AQI reading.
   *
   * Based on the higher of the PM 2.5 and PM 10.0 readings.
   */
  uint16_t getAQI();

 private:
  /* The stream connected to the STM UART. */
  Stream *hpma = NULL;

  /* Latest readings. */
  compact_auto_result_t result_data = { 0xFF };

  /*
   * Write a command and block until the HPM responds with a success code.
   * Return true for success, false otherwise.
   */
  bool writeSimpleCommand(uint8_t cmdByte);
};

#endif  // SRC_HPMA115_COMPACT_H_

