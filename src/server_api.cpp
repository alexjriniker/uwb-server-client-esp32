#include "server_api.h"
#include <functional>

ServerApi::~ServerApi() { delete serial_batch; }
ServerApi::ServerApi() : serial_batch{nullptr} {
  last_update = 0;
  serial_batch = new ApiBatch(
      SERVER_API_BATCH_SIZE,
      std::bind(&ServerApi::send_serial_batch, this, std::placeholders::_1));
}

void ServerApi::update_last() { last_update = millis(); }

bool ServerApi::wifi_connected() const { return WiFi.status() == WL_CONNECTED; }

bool ServerApi::register_device(const char *type, const char *id,
                                const char *addr, const char *label) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.println("\n\nATTEMPTING TO REGISTER DEVICE");
  Serial.println(String("URL: ") + API_DEVICE_CREATE_ROUTE);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  if (!wifi_connected()) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("CANNOT REGISTER DEVICE BECAUSE WIFI IS NOT CONNECTED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    return false;
  }

  http.begin(API_URL, API_PORT, API_DEVICE_CREATE_ROUTE);
  http.addHeader("Content-Type", "application/json");

  String payload = String("{\"type\":\"") + type + "\",\"id\":\"" + id +
                   "\",\"macAddr\":\"" + addr + "\",\"macAddr\":\"" + addr +
                   "\",\"label\":\"" + label + "\"}";

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("PAYLOAD: ");
  Serial.println(payload);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  int status = http.POST(payload);

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("STATUS: ");
  Serial.println(status);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  http.end();

  if (status == 201 || status == 409) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("DEVICE WAS EITHER JUST REGISTERED OR ALREADY REGISTERED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    update_last();
    return true;
  } else if (status < 0)
    return false;
  else {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.print("UNKOWN RESPONSE CODE: ");
    Serial.print(status);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    return false;
  }
}

bool ServerApi::send_serial(const String &str, const char *id) {
  String url = String(API_LIVE_DEVICE_ROUTE) + "/" + id +
               API_DEVICE_SERIAL_ROUTE_PARTIAL;

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.println("\n\nATTEMPTING TO SEND SERIAL");
  Serial.println(String("URL: ") + url);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  if (!wifi_connected()) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("CANNOT SEND SERIAL BECAUSE WIFI IS NOT CONNECTED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    return false;
  }

  http.begin(API_URL, API_PORT, url);
  http.addHeader("Content-Type", "application/json");

  String payload = String("{\"message\":\"") + str + "\"}";

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("PAYLOAD: ");
  Serial.println(payload);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  int status = http.POST(payload);

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("STATUS: ");
  Serial.println(status);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  http.end();

  if (status == 201) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("DEVICE SERIAL WAS SUCCESSFULLY CREATED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    update_last();
    return true;
  } else if (status < 0)
    return false;
  else {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.print("UNKOWN RESPONSE CODE: ");
    Serial.print(status);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    return false;
  }
}

void ServerApi::add_to_serial_batch(const String &str) {
  serial_batch->add(str.c_str());
};

void ServerApi::send_serial_batch(const char *str) {
  String url = String(API_LIVE_DEVICE_ROUTE) + "/" + DEVICE_ID +
               API_DEVICE_SERIA_BATCH_ROUTE_PARTIAL;

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.println("\n\nATTEMPTING TO SEND SERIALBATCH");
  Serial.println(String("URL: ") + url);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  if (!wifi_connected()) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("CANNOT SEND SERIAL BATCH BECAUSE WIFI IS NOT CONNECTED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    return;
  }

  http.begin(API_URL, API_PORT, url);
  http.addHeader("Content-Type", "application/json");

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("PAYLOAD: ");
  Serial.println(str);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  int status = http.POST(str);

#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  Serial.print("STATUS: ");
  Serial.println(status);
#endif // ALLOW_SERIAL_DEBUG_SERVER_API

  http.end();

  // TODO: add more response codes
  if (status == 201) {
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
    Serial.println("DEVICE SERIAL BATCH WAS SUCCESSFULLY CREATED");
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
    update_last();
  }
#ifdef ALLOW_SERIAL_DEBUG_SERVER_API
  else if (status > 0) {
    Serial.print("UNKOWN RESPONSE CODE: ");
    Serial.print(status);
  }
#endif // ALLOW_SERIAL_DEBUG_SERVER_API
}
