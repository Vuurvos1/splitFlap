#include <Arduino.h>
#include "utils.h"
#include <vector>

#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

// Buffer for serial data
std::vector<uint8_t> serialBuffer;

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
    // Resize vector to available bytes
    serialBuffer.resize(mySerial.available());
    int bytesRead = mySerial.readBytes(serialBuffer.data(), serialBuffer.size());
    Serial.print("Received ");
    Serial.print(bytesRead);
    Serial.println(" bytes");

    // print as hex
    Serial.print("Data: ");
    for (int i = 0; i < bytesRead; i++)
    {
      Serial.print("0x");
      Serial.print(serialBuffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // validate the packet
    if (!validatePacket(serialBuffer))
    {
      Serial.println("Invalid packet");
      return;
    }

    // get first byte
    uint8_t command = serialBuffer[1];
    Serial.print("Command: ");
    Serial.println(command);

    // create packet to forward from remaining bytes
    auto forwardPacket = createForwardPacket(serialBuffer);
    if (forwardPacket.size() <= 3)
    {
      Serial.println("Forward packet is too small");
      return;
    }

    // forward the data
    mySerial.write(forwardPacket.data(), forwardPacket.size());
  }
}
