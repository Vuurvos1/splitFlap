#include <Arduino.h>
#include "utils.h"
#include <vector>
#include "SplitFlap.h"

#define TXD1 19
#define RXD1 21

// defines pins
#define STEP_PIN 22
#define DIR_PIN 23
#define HALL_PIN 15

SplitFlap splitFlap(STEP_PIN, DIR_PIN, HALL_PIN);

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

bool readingPacket = false;

void handleMyByte(uint8_t byte)
{
  Serial.println("Received byte: " + String(byte));
  splitFlap.setFlap(byte);
}

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1); // UART setup

  splitFlap.init();
  splitFlap.home();

  Serial.println("Module started");
}

void loop()
{
  splitFlap.update();

  // move flap every 200ms
  static unsigned long lastMove = 0;
  if (millis() - lastMove > 5000)
  {
    splitFlap.moveFlaps(49); // TODO: make sure this also works with > 50
    lastMove = millis();
  }

  // Check if data is available to read
  if (mySerial.available())
  {
    uint8_t incoming = mySerial.read();

    if (!readingPacket && incoming != END_BYTE)
    {
      readingPacket = true;
      handleMyByte(incoming);
    }
    else
    {
      // forward the data
      Serial.print(String(incoming) + " ");
      mySerial.write(incoming);
    }

    if (incoming == END_BYTE)
    {
      Serial.println("End of packet");
      readingPacket = false;
    }
  }
}
