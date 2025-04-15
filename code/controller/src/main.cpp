#include <Arduino.h>
#include <config.h>
#include <HardwareSerial.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <SplitFlap.h>
#include <utils.h>

#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(1);

WebServer server(80);

// defines pins
#define STEP_PIN 13
#define DIR_PIN 12

SplitFlap splitFlap(STEP_PIN, DIR_PIN);

void handleRoot()
{
  server.send(200, "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>");
}

void handleNotFound()
{
  server.send(404, "text/plain", "Not found");
}

void handleCharacter()
{
  if (server.hasArg("c"))
  {
    char c = server.arg("c")[0];
    splitFlap.setCharacter(c);
    server.send(200, "text/plain", "OK");
    return;
  }

  server.send(400, "text/plain", "Bad request");
}

void handleFlaps()
{
  if (server.hasArg("f"))
  {
    int f = server.arg("f").toInt();
    splitFlap.setFlap(f);
    server.send(200, "text/plain", "OK");
    return;
  }

  server.send(400, "text/plain", "Bad request");
}

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1); // UART setup

  Serial.println("ESP32 UART Transmitter");

  while (!Serial)
    ;

  splitFlap.init();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Establishing connection to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize mDNS
  if (!MDNS.begin("splitflap"))
  { // Set the hostname to "splitflap.local"
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.on("/", handleRoot);
  server.on("/character", handleCharacter);
  server.on("/flaps", handleFlaps);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();

  splitFlap.update();

  // send a message over serial every 2 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000)
  {
    // data
    std::vector<uint8_t> data = {
        static_cast<uint8_t>(random(65)),
        static_cast<uint8_t>(random(65)),
        static_cast<uint8_t>(random(65)),
        static_cast<uint8_t>(random(65)),
        static_cast<uint8_t>(random(65))};

    auto packet = createPacket(data);

    // print the packet
    Serial.print("Packet: ");
    for (uint8_t byte : packet)
    {
      Serial.print(byte);
      Serial.print(" ");
    }
    Serial.println();

    mySerial.write(packet.data(), packet.size());

    lastSend = millis();
  }
}
