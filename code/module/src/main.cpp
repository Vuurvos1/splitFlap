#include <Arduino.h>
#include "utils.h"

#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

// Buffer for serial data
const int BUFFER_SIZE = 64;
uint8_t serialBuffer[BUFFER_SIZE];

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
    int bytesRead = mySerial.readBytes(serialBuffer, BUFFER_SIZE);
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
  }
}
