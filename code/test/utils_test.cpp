#include <unity.h>
#include <vector>

#include "../shared/include/utils.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_calculateCRC_function(void)
{
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  uint8_t length = sizeof(data) / sizeof(data[0]);
  uint8_t crc = calculateCRC(data, length);
  TEST_ASSERT_EQUAL(0x01, crc);
}

void test_createPacket_function(void)
{
  std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0x05};
  std::vector<uint8_t> packet = createPacket(data);

  uint8_t length = data.size();

  // Check data
  for (size_t i = 0; i < length; i++)
  {
    TEST_ASSERT_EQUAL(data[i], packet[i]);
  }

  // Check END_BYTE
  TEST_ASSERT_EQUAL(packet[length], END_BYTE);
}

void test_validatePacket_function(void)
{
  // valid packet
  std::vector<uint8_t> packet = {0xAA, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0xFF};
  TEST_ASSERT_TRUE(validatePacket(packet));

  // missing end byte
  std::vector<uint8_t> packet2 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x01};
  TEST_ASSERT_FALSE(validatePacket(packet2));
}

void test_createForwardPacket_function(void)
{
  std::vector<uint8_t> packet = createPacket({0x01, 0x02, 0x03});
  std::vector<uint8_t> forwardPacket = createForwardPacket(packet);
  TEST_ASSERT_EQUAL(packet.size() - 1, forwardPacket.size());

  TEST_ASSERT_EQUAL(forwardPacket[0], 0x02);
  TEST_ASSERT_EQUAL(forwardPacket[1], 0x03);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_calculateCRC_function);
  RUN_TEST(test_createPacket_function);
  RUN_TEST(test_validatePacket_function);
  RUN_TEST(test_createForwardPacket_function);
  return UNITY_END();
}
