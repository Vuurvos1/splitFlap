#pragma once

#include <vector>

#define START_BYTE 0xAA // 10101010 in binary
#define END_BYTE 0x55   // 01010101 in binary

uint8_t calculateCRC(uint8_t *data, uint8_t length)
{
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < length; i++)
  {
    crc ^= data[i];
  }
  return crc;
}

/**
 * create a packet
 * [START][DATA...][CRC][END]
 */
std::vector<uint8_t> createPacket(const std::vector<uint8_t> &data)
{
  std::vector<uint8_t> packet(data.size() + 3);
  packet[0] = START_BYTE;
  std::copy(data.begin(), data.end(), packet.begin() + 1);
  packet[data.size() + 1] = END_BYTE;
  packet[data.size() + 2] = calculateCRC(const_cast<uint8_t *>(data.data()), data.size());
  return packet;
}
