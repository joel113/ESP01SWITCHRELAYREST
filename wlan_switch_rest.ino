#include <ESP8266WiFi.h>
#include <aREST.h>

extern "C" {
  #include "user_interface.h"
}

#define LISTEN_PORT 80

const char* ssid = "";
const char* password = "";
char* wifiHostName = "joel-pump";

WiFiClient espClient;
aREST rest = aREST();
WiFiServer server(LISTEN_PORT);

int waterControl(String water);

void setup() {
  Serial.println("Setting up the controller.");
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  digitalWrite(0, 1);
  Serial.println("Starting to establish a connection.");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  wifi_station_set_auto_connect(true);
  wifi_station_set_hostname(wifiHostName);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  rest.function("water", waterControl);
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if(!client) {
    return;
  }
  while(!client.available()) {
    delay(1);
  }
  rest.handle(client);
}

int waterControl(String command) {
  Serial.println("Water on for 10 seconds.");
  digitalWrite(0,0);
  delay(10000);
  digitalWrite(0,1);
  return 1;
}
