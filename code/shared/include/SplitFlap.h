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
  SplitFlap(int stepPin, int dirPin, int hallPin)
  {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->hallPin = hallPin;
    stepper = AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);

    stepper.setMaxSpeed(STEPS_PER_REVOLUTION * 2);
    stepper.setAcceleration(STEPS_PER_REVOLUTION * 2);
  }

  void init()
  {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(hallPin, INPUT_PULLUP);

    digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  }

  void home()
  {
    // TODO: make this non blocking by adding module states
    Serial.println("Homing");
    stepper.setSpeed(1000); // Steps per second
    while (digitalRead(hallPin) == HIGH)
    {
      stepper.runSpeed();
    }
    stepper.setCurrentPosition(0);

    // TODO: add max steps
  }

  void update()
  {
    // normalized step
    uint16_t normalizedStep = stepper.currentPosition() % STEPS_PER_REVOLUTION;

    // at first flap, 1 deg
    if (normalizedStep == STEPS_PER_REVOLUTION / 360 && digitalRead(hallPin) == HIGH)
    {
      Serial.println("Missed steps");

      stepper.setSpeed(1000); // Steps per second
      while (digitalRead(hallPin) == HIGH)
      {
        stepper.runSpeed();
      }

      // set position to the nearest full rotation (rounded down)
      long offset = stepper.currentPosition() % STEPS_PER_REVOLUTION;
      stepper.setCurrentPosition(stepper.currentPosition() - offset);

      // TODO: fix, this prevents the module from moving to its target position
    }

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

    int currentIndex = getCurrentFlapIndex();
    int currentPosition = stepper.currentPosition();

    int steps = index - currentIndex;

    if (steps < 0)
    {
      steps += FLAP_COUNT;
    }

    stepper.moveTo(currentPosition + steps * FLAP_STEPS);
  }

  /** move x amount of flaps */
  void moveFlaps(int steps)
  {
    // TODO: update to take the closest target position, instead of just adding steps
    Serial.printf("Moving %d flaps to index %d\n", steps, (getCurrentFlapIndex() + steps) % FLAP_COUNT);
    stepper.moveTo(stepper.targetPosition() + steps * FLAP_STEPS);
  }

private:
  int stepPin;
  int dirPin;
  int hallPin;
  AccelStepper stepper;

  // TOOD: update character list
  const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-:,.'%$@?!# █";
  const uint8_t FLAP_COUNT = characters.length();
  const uint16_t FLAP_STEPS = STEPS_PER_REVOLUTION / FLAP_COUNT;

  uint8_t getCharacterIndex(char c)
  {
    int index = characters.indexOf(toUpperCase(c)); // toupper(c) ??
    if (index == -1)
    {
      Serial.printf("Character not found: %c\n", c);
      return characters.indexOf(' ');
    }
    return index;
  }

  // TODO: Get current flap index
  uint8_t getCurrentFlapIndex()
  {
    return stepper.currentPosition() / FLAP_STEPS % FLAP_COUNT;
  }

  uint8_t getTargetFlapIndex()
  {
    return stepper.targetPosition() / FLAP_STEPS % FLAP_COUNT;
  }
};
