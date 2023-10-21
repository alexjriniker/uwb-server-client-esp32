#ifndef __CONFIG_H__
#define __CONFIG_H__

// GLOBAL ALLOW FLAG(S)
#define ALLOW_SERIAL
#define ALLOW_DEBUG

#ifdef ALLOW_SERIAL

#define SERIAL_STARTUP_DELAY                                                    \
  200 // wait for x (ms) after `Serial.begin` call to allow the serial monitor
      // to catchup

#endif // ALLOW_SERIAL

// FLAGS THAT CAN ONLY OCCUR IF (ALLOW_SERIAL && ALLOW_DEBUG) are defined
#if defined ALLOW_DEBUG && defined ALLOW_SERIAL

#define ALLOW_SERIAL_DEBUG_WIFI       // PRINTS DEBUGGING INFO FOR `WIFI`
#define ALLOW_SERIAL_DEBUG_SERVER_API // PRINTS DEBUGGING INFO FOR `SERVER API`

#endif // defined ALLOW_DEBUG && defined ALLOW_SERIAL

// GENERAL WIFI VARIABLE(S)
#define WIFI_CONNECT_DELAY                                                     \
  200 // wait for x (ms) then recheck to see if WiFi is connected

// GENERAL DEVICE VARIABLE(S)
#define DEVICE_TYPE                                                            \
  "anchor" // Device type:
           // Must be either "anchor" or "tag"; otherwise, the
           // server will throw an error
#define DEVICE_ID                                                              \
  "1234" // Device id:
         // Should be unique but different from the id derived from mac address
#define DEVICE_ADDR                                                            \
  "7D:00:22:EA:82:60:3B:54" // Device addr:
                            // Should be unique
#define DEVICE_LABEL                                                           \
  "Alex #3" // Device label:
            // Should but is not required to be unique helpful for going through
            // data

// GENERAL SERVER API VARIABLE(S)
#define SERVER_API_BATCH_SIZE 100 // size of server batches

// API VARIABLE(S)
// API STUFF FOR BASE SERVER
#define API_URL "cow-tracking.loca.lt"
#define API_PORT 80

// API ROUTE(S)
#define API_BASE "/api/v1"

#define API_LIVE_DEVICE_ROUTE API_BASE "/live_devices"
#define API_DEVICE_CREATE_ROUTE API_LIVE_DEVICE_ROUTE // METHOD: POST
// #define API_DEVICE_REFRESH_ROUTE                                               \
//   API_BASE "/live_devices" // METHOD: PUT, PARAMS: id

#define API_DEVICE_SERIAL_ROUTE_PARTIAL "/serial" // METHOD: POST, PARAMS: id
#define API_DEVICE_SERIAL_BATCH_ROUTE_PARTIAL                                   \
  "/serial_batch" // METHOD: POST, PARAMS: id

#endif // __CONFIG_H__
