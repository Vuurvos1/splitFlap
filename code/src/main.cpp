#include <Arduino.h>
#include <config.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

// 200 steps per revolution
// 1 revolution = 360 degrees
// 1 step = 1.8 degrees

// 1/16 microstepping
// 1 revolution = 200 * 16 steps = 3200 steps
#define STEPS_PER_REVOLUTION 3200

#define FLAP_COUNT 50

// defines pins
#define stepPin 13
#define dirPin 12

void step(int steps, int delayTime)
{
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}

void turnDeg(int degrees, int delayTime = 500)
{
  // don't do negative angles
  const int steps = (STEPS_PER_REVOLUTION / 360) * degrees;
  step(steps, delayTime);
}

void nextFlap(int delayTime = 500)
{
  const int steps = STEPS_PER_REVOLUTION / FLAP_COUNT;
  step(steps, delayTime);
}

void handleRoot()
{
  server.send(200, "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"></head><body><h1>Hey there!</h1></body></html>");
}

void handleNotFound()
{
  server.send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);

  while (!Serial)
    ;

  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

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
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
}

void loop()
{
  server.handleClient();

  // turnDeg(90, 500);

  // delay(1500);

  // turnDeg(90, 300);

  // delay(1500);

  // turnDeg(180, 500);

  // for (int i = 0; i < 12; i++)
  // {
  //   nextFlap(300);
  //   delay(1000);
  // }

  // turnDeg(360 * 3, 300);

  // delay(3000);

  // digitalWrite(dirPin, LOW); // Changes the rotations direction
  // // Makes 400 pulses for making two full cycle rotation
  // for (int x = 0; x < 1600; x++)
  // {
  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(500);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(500);
  // }
  // delay(1000);
}
