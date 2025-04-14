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
  packet[data.size() + 1] = calculateCRC(const_cast<uint8_t *>(data.data()), data.size());
  packet[data.size() + 2] = END_BYTE;
  return packet;
}

bool validatePacket(const std::vector<uint8_t> &packet)
{
  if (packet.size() < 3)
  {
    return false;
  }

  if (packet[0] != START_BYTE)
  {
    return false;
  }

  if (packet[packet.size() - 1] != END_BYTE)
  {
    return false;
  }

  // Extract data portion and verify CRC
  std::vector<uint8_t> data(packet.begin() + 1, packet.end() - 2);
  uint8_t receivedCRC = packet[packet.size() - 2];
  uint8_t calculatedCRC = calculateCRC(const_cast<uint8_t *>(data.data()), data.size());

  if (receivedCRC != calculatedCRC)
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
  std::vector<uint8_t> data(packet.begin() + 2, packet.end() - 2); // skip START byte and first data byte, and skip CRC and END bytes
  return createPacket(data);
}
