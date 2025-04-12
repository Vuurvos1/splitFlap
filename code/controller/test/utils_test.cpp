#include <unity.h>
#include "../src/utils.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_add_function(void)
{
  TEST_ASSERT_EQUAL(3, add(1, 2));
  TEST_ASSERT_EQUAL(0, add(-1, 1));
  TEST_ASSERT_EQUAL(0, add(0, 0));
  TEST_ASSERT_EQUAL(300, add(100, 200));
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_add_function);
  return UNITY_END();
}
