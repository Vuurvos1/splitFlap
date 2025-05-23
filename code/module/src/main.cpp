#include <Arduino.h>
#include "utils.h"
#include <vector>
#include "SplitFlap.h"

#define TXD1 19
#define RXD1 21

// defines pins
#define STEP_PIN 16
#define DIR_PIN 17
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

uint8_t flapIndex = 40;

void loop()
{
  splitFlap.update();

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
