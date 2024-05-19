#include <Arduino.h>
#include <config.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <AccelStepper.h>

WebServer server(80);

// 200 steps per revolution
// 1 revolution = 360 degrees
// 1 step = 1.8 degrees

// 1/16 microstepping
// 1 revolution = 200 * 16 steps = 3200 steps
#define STEPS_PER_REVOLUTION 3200

#define FLAP_COUNT 50

// defines pins
#define STEP_PIN 13
#define DIR_PIN 12

const int flapSteps = STEPS_PER_REVOLUTION / FLAP_COUNT;

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// 26 ABCDEFGHIJKLMNOPQRSTUVWXYZ
// 10 0123456789
// 20 !@#$%^&*()_+{}|:<>?/.,;[]\\=-`~
// 1 █ (full block)
// 1 space

// "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █
// ",.?!=/-+:$%()"

// ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+{}|:<>?/.,;[]\\=-`~"
// █

// const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █";
// "+-*!.#%@?,&': █"
// "?!@&()+-%$█*= "
// " █?!@%'+-=.,#:"

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

void nextFlap(int delayTime = 500)
{
  stepper.moveTo(stepper.currentPosition() + flapSteps);
}

int getFlapIndex()
{
  return stepper.currentPosition() / flapSteps % FLAP_COUNT;
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
  int currentIndex = getFlapIndex();
  int currentPosition = stepper.currentPosition();

  int steps = targetIndex - currentIndex;

  if (steps < 0)
  {
    steps += FLAP_COUNT;
  }

  Serial.printf("%d -> %d: %d\n", currentIndex, targetIndex, steps);
  stepper.moveTo(currentPosition + steps * flapSteps);
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
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

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

  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(STEPS_PER_REVOLUTION * 3); // Set the maximum speed in steps per second
  stepper.setAcceleration(15000);                // Set the acceleration in steps per second squared

  // Move the motor to the initial position
  // TODO: home sensor
  // stepper.moveTo(1000); // Move to position 1000

  server.on("/", handleRoot);
  server.on("/character", handleCharacter);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(DIR_PIN, HIGH); // Enables the motor to move in a particular direction
}

void loop()
{
  server.handleClient();

  // if (current pos % flapSteps == 0) {
  //   stepper.stop(); // reset to prevent overflow
  // }

  stepper.run();
}
