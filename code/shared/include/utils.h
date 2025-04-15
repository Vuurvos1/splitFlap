#pragma once

#include <vector>

// #define START_BYTE 0xAA // 10101010 in binary
#define END_BYTE 0xFF // 01010101 in binary

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
 * [DATA...][END]
 */
std::vector<uint8_t> createPacket(const std::vector<uint8_t> &data)
{
  std::vector<uint8_t> packet(data.size() + 1);        // +1 for END byte only
  std::copy(data.begin(), data.end(), packet.begin()); // Copy data at the start
  packet[data.size()] = END_BYTE;                      // Add END_BYTE at the end
  return packet;
}

bool validatePacket(const std::vector<uint8_t> &packet)
{
  if (packet.size() < 2)
  {
    return false;
  }

  if (packet[packet.size() - 1] != END_BYTE)
  {
    return false;
  }

  return true;
}

/**
 * remove first data byte and create a new packet with the remaining data
 */
std::vector<uint8_t> createForwardPacket(const std::vector<uint8_t> &packet)
{
  std::vector<uint8_t> data(packet.begin() + 1, packet.end() - 1);
  return createPacket(data);
}
