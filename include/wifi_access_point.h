#pragma once

class wifi_access_point {
 public:
  wifi_access_point(const char *name, const char *password, int max_clients);

  bool start();

 private:
  const char *network_name;
  const char *network_password;
  int max_connected_clients;
};
