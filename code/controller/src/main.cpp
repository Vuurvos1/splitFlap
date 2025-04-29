#include <Arduino.h>
#include <config.h>
#include <HardwareSerial.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <LittleFS.h>

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
#define HALL_PIN 15

SplitFlap splitFlap(STEP_PIN, DIR_PIN, HALL_PIN);

String getContentType(String filename)
{
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".json"))
    return "application/json";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".svg"))
    return "image/svg+xml";
  else if (filename.endsWith(".woff2"))
    return "font/woff2";
  else if (filename.endsWith(".woff"))
    return "font/woff";
  else if (filename.endsWith(".ttf"))
    return "font/ttf";
  else if (filename.endsWith(".mp3"))
    return "audio/mpeg";
  else if (filename.endsWith(".mp4"))
    return "video/mp4";
  return "application/octet-stream";
}

bool handleFileRead(String path)
{
  if (path.endsWith("/"))
    path += "index.html";

  String contentType = getContentType(path);

  if (LittleFS.exists(path))
  {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleRoot()
{
  server.send(200, "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>");
}

void handleNotFound()
{
  if (!handleFileRead(server.uri()))
  {
    {
      // Fall back to index.html for client-side routing
      handleFileRead("/index.html");
    }

    // server.send(404, "text/plain", "Not found");
  }
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

void handleSplitflap()
{
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String body = server.arg("plain");

  // Simple validation - must start with [ and end with ]
  if (body[0] != '[' || body[body.length() - 1] != ']')
  {
    server.send(400, "text/plain", "Invalid array format");
    return;
  }

  // Remove the brackets
  body = body.substring(1, body.length() - 1);

  std::vector<uint8_t> data;

  // Split string into numbers and add to data vector
  int start = 0;
  while (true)
  {
    int end = body.indexOf(',', start);
    bool isLast = (end == -1);
    if (isLast)
    {
      end = body.length();
    }

    String numStr = body.substring(start, end);
    numStr.trim();
    if (numStr.length() > 0)
    {
      data.push_back(static_cast<uint8_t>(numStr.toInt()));
    }

    if (isLast)
      break;
    start = end + 1;
  }

  // print the data
  Serial.print("Data: ");
  for (uint8_t byte : data)
  {
    Serial.print(byte);
    Serial.print(" ");
  }
  Serial.println();

  // set splitflap to the first data value
  splitFlap.setFlap(data[0]);
  data.erase(data.begin());

  // pass the rest of the data to the splitflap
  auto packet = createPacket(data);
  mySerial.write(packet.data(), packet.size());

  server.send(200, "text/plain", "OK");
}

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1); // UART setup

  Serial.println("ESP32 UART Transmitter");

  while (!Serial)
    ;

  if (!LittleFS.begin(true))
  {
    Serial.println("An error occurred while mounting LittleFS");
  }

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

  // server.on("/", handleRoot);
  // server.on("/character", handleCharacter);
  server.on("/api/splitflap", handleSplitflap);
  server.on("/flaps", handleFlaps);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();

  splitFlap.update();
}
