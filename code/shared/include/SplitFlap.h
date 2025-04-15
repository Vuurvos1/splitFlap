#include <Arduino.h>
#include <AccelStepper.h>

// 200 steps per revolution
// 1 revolution = 360 degrees
// 1 step = 1.8 degrees

// 1/16 microstepping
// 1 revolution = 200 * 16 steps = 3200 steps

#define STEPS_PER_REVOLUTION 3200

class SplitFlap
{
public:
  SplitFlap(int stepPin, int dirPin)
  {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    stepper = AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);

    // Set the maximum speed and acceleration
    stepper.setMaxSpeed(STEPS_PER_REVOLUTION * 8); // Set the maximum speed in steps per second
    stepper.setAcceleration(STEPS_PER_REVOLUTION); // Set the acceleration in steps per second squared
  }

  void init()
  {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction

    this->home();
  }

  void home()
  {
    // TODO: implement homing sequence
  }

  void update()
  {
    // if (current pos % flapSteps == 0) {
    //   stepper.stop(); // reset to prevent overflow
    // }

    stepper.run();
  }

  void setCharacter(char c)
  {
    int targetIndex = getCharacterIndex(c);
    setFlap(targetIndex);
  }

  void setFlap(int index)
  {
    if (index < 0 || index >= FLAP_COUNT)
    {
      Serial.printf("Invalid index: %d\n", index);
      return;
    }

    int currentIndex = getFlapIndex();
    int currentPosition = stepper.currentPosition();

    int steps = index - currentIndex;

    if (steps < 0)
    {
      steps += FLAP_COUNT;
    }

    stepper.moveTo(currentPosition + steps * FLAP_STEPS);
  }

  void moveFlaps(int steps)
  {
    stepper.moveTo(stepper.currentPosition() + steps * FLAP_STEPS);
  }

private:
  int stepPin;
  int dirPin;
  AccelStepper stepper;

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
  // | is full block
  // ?!@-=+&%$#.,:'

  // "+-*!.#%@?,&': █"
  // "?!@&()+-%$█*= "
  // " █?!@%'+-=.,#:"

  // +-:,.'%$@?!# █

  const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-:,.'%$@?!# |";
  // | is full block??
  const int FLAP_COUNT = characters.length();
  const int FLAP_STEPS = STEPS_PER_REVOLUTION / characters.length();

  int getCharacterIndex(char c)
  {
    int index = characters.indexOf(toUpperCase(c)); // toupper(c) ??
    if (index == -1)
    {
      Serial.printf("Character not found: %c\n", c);
      return characters.indexOf(' ');
    }
    return index;
  }

  int getFlapIndex()
  {
    return stepper.currentPosition() / FLAP_STEPS % FLAP_COUNT;
  }
};
