#include <unity.h>

#include <Wheel.cpp>

namespace WheelTest
{
    void test_values(void)
    {
        WheelValue v1("c1", { 0, 1 });
        WheelValue v2("c1", { 0, 2 });

        long t1 = 1000;
        long t2 = 2000;

        TEST_ASSERT_TRUE(Wheel::values()->empty());
        TEST_ASSERT_EQUAL(0, Wheel::values()->millis("c1"));
        TEST_ASSERT_FALSE(Wheel::values()->changed("c1", t1));
        TEST_ASSERT_TRUE(Wheel::values()->get("c1")->empty());

        Wheel::values()->put("c1", v1, t1);

        TEST_ASSERT_FALSE(Wheel::values()->empty());
        TEST_ASSERT_FALSE(Wheel::values()->get("c1")->empty());
        TEST_ASSERT_FALSE(Wheel::values()->changed("c1", t1));
        TEST_ASSERT_EQUAL(t1, Wheel::values()->millis("c1"));
        TEST_ASSERT_EQUAL_INT8_ARRAY(v1.bytes(), Wheel::values()->get("c1")->bytes(), 2);

        Wheel::values()->put("c1", v2, t2);

        TEST_ASSERT_FALSE(Wheel::values()->empty());
        TEST_ASSERT_FALSE(Wheel::values()->get("c1")->empty());
        TEST_ASSERT_TRUE(Wheel::values()->changed("c1", t1));
        TEST_ASSERT_FALSE(Wheel::values()->changed("c1", t2));
        TEST_ASSERT_EQUAL(t2, Wheel::values()->millis("c1"));
        TEST_ASSERT_EQUAL_INT8_ARRAY(v2.bytes(), Wheel::values()->get("c1")->bytes(), 2);

        Wheel::values()->clear();
        TEST_ASSERT_TRUE(Wheel::values()->empty());
    }

    void run_tests() {
        RUN_TEST(WheelTest::test_values);
    }
}