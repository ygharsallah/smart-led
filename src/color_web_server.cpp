#include "color_web_server.h"

#include <Arduino.h>

extern const uint8_t web_page_start[] asm("_binary_assets_web_page_html_start");
extern const uint8_t web_page_end[] asm("_binary_assets_web_page_html_end");

void color_web_server::start() {
  web_server.on("/", HTTP_GET, [this]() { send_web_page(); });
  web_server.on("/color", HTTP_GET, [this]() { send_current_color(); });
  web_server.on("/message", HTTP_POST, [this]() { receive_color(); });
  web_server.on("/favicon.ico", HTTP_GET, [this]() { send_empty_response(204); });

  web_server.begin();
  server_ready = true;
}

void color_web_server::handle_requests() {
  if (server_ready) {
    web_server.handleClient();
  }
}

bool color_web_server::is_valid_color(const String &color) {
  if (color.length() != 7 || color[0] != '#') {
    return false;
  }

  for (size_t index = 1; index < color.length(); ++index) {
    const char digit = color[index];
    const bool is_hex_digit = (digit >= '0' && digit <= '9') ||
                              (digit >= 'a' && digit <= 'f') ||
                              (digit >= 'A' && digit <= 'F');
    if (!is_hex_digit) {
      return false;
    }
  }

  return true;
}

void color_web_server::receive_color() {
  if (!web_server.hasArg("message")) {
    send_empty_response(400);
    return;
  }

  String color = web_server.arg("message");
  if (!is_valid_color(color)) {
    send_empty_response(400);
    return;
  }

  color.toUpperCase();
  current_color = color;
  const String client_ip = web_server.client().remoteIP().toString();
  Serial.printf("Color: %s (client: %s)\n", color.c_str(), client_ip.c_str());
  send_empty_response(204);
}

void color_web_server::send_current_color() {
  web_server.sendHeader("Cache-Control", "no-store");
  web_server.send(200, "text/plain; charset=utf-8", current_color);
}

void color_web_server::send_web_page() {
  const char *page_content = reinterpret_cast<const char *>(web_page_start);
  const size_t page_length = web_page_end - web_page_start - 1;

  web_server.sendHeader("Cache-Control", "no-store");
  web_server.send_P(200, "text/html; charset=utf-8", page_content, page_length);
}

void color_web_server::send_empty_response(int status_code) {
  web_server.send_P(status_code, "text/plain", "");
}
