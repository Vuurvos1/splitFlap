#include <Arduino.h>

const int flaps = 50;
// ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+{}|:<>?/.,;[]\\=-`~"
// █
const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █";
// "+-*!.#%@?,&': █"
// "?!@&()+-%$█*= "
// " █?!@%'+-=.,#:"

const int DIR = 12;
const int STEP = 13;
const int stepsPerRevolution = 200 * 16;

const int stepCount = stepsPerRevolution / flaps;

int currentChar = 0;

// get current char index
int getCharIndex(String str, char c)
{
  int index = str.indexOf(c);
  if (index == -1)
  {
    return 0;
  }
  return index;

  // if not found, goto empty space
}

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  Serial.println("Starting up");

  // home module
  // rotate till sensor is triggered
}

void loop()
{
  Serial.println("Looping");

  // put your main code here, to run repeatedly:

  // get current char
  // get preferred char
  // get char difference (always forrward)
  // convert to stepper steps

  // int currentIndex = getCharIndex(characters, currentChar);
  // int preferredIndex = getCharIndex(characters, 'A');
  // int difference = preferredIndex - currentIndex;
  // int steps = difference * stepCount;

  // rotate stepper

  digitalWrite(DIR, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(200); // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP, LOW);
    delayMicroseconds(200);
  }
  delay(1000); // One second delay

  digitalWrite(DIR, LOW); // Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < stepsPerRevolution * 2; x++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP, LOW);
    delayMicroseconds(100);
  }
  delay(1000);
}

// when/while rotating double check if sensor is triggered to ensure correct positioning
// if not, home module

// if snesor is triggered but not at char || expected to pass sensor but not triggered
// every x steps update current char

// default char is space/filled?
