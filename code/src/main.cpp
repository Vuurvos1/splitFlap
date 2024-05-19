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

// 26 ABCDEFGHIJKLMNOPQRSTUVWXYZ
// 10 0123456789
// 20 !@#$%^&*()_+{}|:<>?/.,;[]\\=-`~
// 1 █ (full block)
// 1 space

// "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █
// ",.?!=/-+:$%()"

// ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+{}|:<>?/.,;[]\\=-`~"
// █

// TODO: maybe reuse 0 and o and 1 and i
// const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █";
const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-:,.'%$@?!# |";
// | is full block
// ?!@-=+&%$#.,:'

// "+-*!.#%@?,&': █"
// "?!@&()+-%$█*= "
// " █?!@%'+-=.,#:"

// +-:,.'%$@?!# █

int characterIndex = 0;

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

int getCharacterIndex(char c)
{
  int index = characters.indexOf(c);
  if (index == -1)
  {
    return characters.indexOf(' ');
  }
  return index;
}

void setCharacter(char c)
{
  int targetIndex = getCharacterIndex(c);

  int steps = targetIndex - characterIndex;

  if (steps < 0)
  {
    steps += FLAP_COUNT;
  }
  // int steps = newCharacterIndex < characterIndex ? characters.length() - characterIndex + newCharacterIndex : newCharacterIndex - characterIndex;

  Serial.printf("%d -> %d: %d\n", characterIndex, targetIndex, steps);
  Serial.println(characters.length());

  characterIndex = targetIndex;

  step(steps * (STEPS_PER_REVOLUTION / FLAP_COUNT), 150);

  // TODO: maybe add slight ramp up and ramp down
  // step(steps * (STEPS_PER_REVOLUTION / FLAP_COUNT), 100);
}

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
    setCharacter(c);
    server.send(200, "text/plain", "OK");
  }
  else
  {
    server.send(400, "text/plain", "Bad request");
  }
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

  // TODO: home sensor

  server.on("/", handleRoot);
  server.on("/character", handleCharacter);
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
