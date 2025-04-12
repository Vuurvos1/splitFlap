#include <Arduino.h>
#include "utils.h"

uint8_t calculateCRC(uint8_t *data, uint8_t length);

void setup()
{
  Serial.begin(115200);

  // put your setup code here, to run once:
  int result = add(2, 3);
  Serial.println(result);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

uint8_t calculateCRC(uint8_t *data, uint8_t length)
{
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < length; i++)
  {
    crc ^= data[i];
  }
  return crc;
}
