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

    lastHallState = digitalRead(hallPin);
    lastPosition = 0;
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
    stepper.run();

    // Check for home position passing
    bool currentHallState = digitalRead(hallPin);
    if (currentHallState != lastHallState)
    {
      if (currentHallState == LOW) // Hall sensor just triggered
      {
        int currentPosition = stepper.currentPosition();
        int stepsSinceLastHome = currentPosition - lastPosition;

        // Calculate how far we are from the nearest flap boundary
        int positionError = currentPosition % FLAP_STEPS;
        // If we're more than halfway through a flap, consider it as being close to the next boundary
        if (positionError > FLAP_STEPS / 2)
        {
          positionError = FLAP_STEPS - positionError;
        }

        // Allow for more lenient tolerance (1/4 of a flap)
        const int MAX_ALLOWED_ERROR = FLAP_STEPS / 4;

        if (positionError > MAX_ALLOWED_ERROR)
        {
          Serial.printf("Warning: Position error detected! Off by %d steps (max allowed: %d)\n",
                        positionError, MAX_ALLOWED_ERROR);
          // Try to correct the position to the nearest flap boundary
          int correctedPosition = (currentPosition / FLAP_STEPS) * FLAP_STEPS;
          if (positionError > FLAP_STEPS / 2)
          {
            correctedPosition += FLAP_STEPS;
          }
          stepper.setCurrentPosition(correctedPosition);
        }

        lastPosition = currentPosition;
      }
      lastHallState = currentHallState;
    }
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

  /** move x amount of flaps */
  void moveFlaps(int steps)
  {
    // TODO: update to take the closest target position, instead of just adding steps
    Serial.printf("Moving %d flaps to index %d\n", steps, (getFlapIndex() + steps) % FLAP_COUNT);
    stepper.moveTo(stepper.targetPosition() + steps * FLAP_STEPS);
  }

private:
  int stepPin;
  int dirPin;
  int hallPin;
  AccelStepper stepper;

  bool lastHallState;
  int lastPosition;

  // TOOD: update character list
  const String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-:,.'%$@?!# █";
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

  // TODO: Get current flap index
  int getFlapIndex()
  {
    return stepper.currentPosition() / FLAP_STEPS % FLAP_COUNT;
  }

  // TODO: get target flap index
};
