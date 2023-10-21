#ifndef __SERVER_API_H__
#define __SERVER_API_H__

#include "config.h"
#include "api_batch.h"
#include <Arduino.h>
#include <HTTPClient.h> // include for `HTTPClient`
#include <WiFi.h>       // include for `WiFi`

class ServerApi {
private:
  unsigned long last_update;
  HTTPClient http;
  ApiBatch *serial_batch;

  void update_last();
  ServerApi(const ServerApi &);
  ServerApi &operator=(const ServerApi &);

public:
  ~ServerApi();
  ServerApi();

  /**
   * @brief Returns whether or not the WiFi is connected
   *
   * @return `true` - WiFi is connected
   * @return `false` - WiFi is not connected
   */
  bool wifi_connected() const;
  /**
   * @brief registures current device with server
   *
   * @return `true` - device was successfully registered or was already
   * @return `false` - device was not able to be registered
   */
  bool register_device(const char * = DEVICE_TYPE, const char * = DEVICE_ID,
                       const char * = DEVICE_ADDR, const char * = DEVICE_LABEL);
  /**
   * @brief add serial to server `LiveDevice`
   *
   * @return true - the serial was succesfully added
   * @return false - the serial was not able to be added
   */
  bool send_serial(const String &, const char * = DEVICE_ID);
  void add_to_serial_batch(const String&);

private:
  void send_serial_batch(const char *);
};

#endif // __SERVER_API_H__