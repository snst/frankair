#include <unity.h>
#include "fa_calc.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_convertRelativeToAbsoluteHumidity(void)
{
  TEST_ASSERT_EQUAL_FLOAT(9.207461, convertRelativeToAbsoluteHumidity(25, 40));
  TEST_ASSERT_EQUAL_FLOAT(13.81119, convertRelativeToAbsoluteHumidity(25, 60));
  TEST_ASSERT_EQUAL_FLOAT(18.41492, convertRelativeToAbsoluteHumidity(25, 80));
  TEST_ASSERT_EQUAL_FLOAT(2.717554, convertRelativeToAbsoluteHumidity(5, 40));
  TEST_ASSERT_EQUAL_FLOAT(4.076331, convertRelativeToAbsoluteHumidity(5, 60));
  TEST_ASSERT_EQUAL_FLOAT(5.435108, convertRelativeToAbsoluteHumidity(5, 80));
  TEST_ASSERT_EQUAL_FLOAT(3.878644, convertRelativeToAbsoluteHumidity(0, 80));
  TEST_ASSERT_EQUAL_FLOAT(0.0001727296, convertRelativeToAbsoluteHumidity(20, 0.001));
  TEST_ASSERT_EQUAL_FLOAT(0, convertRelativeToAbsoluteHumidity(20, 0));

}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_convertRelativeToAbsoluteHumidity);
  UNITY_END();
  return 0;
}
