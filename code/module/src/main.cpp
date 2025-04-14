#include <Arduino.h>
#include "utils.h"
#include <vector>

#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

bool readingPacket = false;

void handleMyByte(uint8_t byte)
{
  Serial.println("Received byte: " + String(byte));
  // TODO: set the flap number
}

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1); // UART setup

  Serial.println("Module started");
}

void loop()
{
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
