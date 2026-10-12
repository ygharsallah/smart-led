#include <Arduino.h>

#include "color_web_server.h"
#include "config.h"
#include "wifi_access_point.h"

namespace {
  wifi_access_point access_point(config::wifi_name, config::wifi_password, config::max_connected_clients);
  color_web_server color_server;
}

void setup() {
  Serial.begin(config::serial_baud_rate);
  Serial.setTxTimeoutMs(20);
  delay(500);

  if (!access_point.start()) {
    Serial.println("ERROR: Wi-Fi access point could not start. Press RESET to retry.");
    return;
  }

  color_server.start();
}

void loop() {
  color_server.handle_requests();
  delay(1);
}
