#include <Arduino.h>

const int flaps = 50;
// ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+{}|:<>?/.,;[]\\=-`~"
// █
const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!@#$%^&*()+:/.,=-`\" █";
const int stepCount = 400 / flaps;

int currentChar = 0;

void setup()
{
  // home module
  // rotate till sensor is triggered
}

void loop()
{
  // put your main code here, to run repeatedly:

  // get current char
  // get preferred char
  // get char difference (always forrward)
  // convert to stepper steps

  int currentIndex = getCharIndex(characters, currentChar);
  int preferredIndex = getCharIndex(characters, 'A');
  int difference = preferredIndex - currentIndex;
  int steps = difference * stepCount;
}

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
