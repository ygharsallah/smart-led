#pragma once

#include <WebServer.h>

class color_web_server {
 public:
  void start();
  void handle_requests();

 private:
  WebServer web_server{80};
  bool server_ready = false;
  String current_color = "#0066FF";

  static bool is_valid_color(const String &color);

  void receive_color();
  void send_current_color();
  void send_web_page();
  void send_empty_response(int status_code);
};
