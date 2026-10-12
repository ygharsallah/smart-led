#include "wifi_access_point.h"

#include <WiFi.h>

wifi_access_point::wifi_access_point(const char *name, const char *password, int max_clients)
  : network_name(name), network_password(password), max_connected_clients(max_clients) {}

bool wifi_access_point::start() {
  const IPAddress ip_address(192, 168, 4, 1);
  const IPAddress subnet_mask(255, 255, 255, 0);

  return WiFi.mode(WIFI_AP) &&
         WiFi.softAPConfig(ip_address, ip_address, subnet_mask) &&
         WiFi.softAP(network_name, network_password, 1, false, max_connected_clients);
}
