#define WHEEL_DEVICE_TYPE WHEEL_DEVICE_TYPE_PINT

#include <unity.h>

#include <WheelValue.cpp>
#include <WheelCharacteristic.cpp>

namespace WheelValueTest
{
    void test_get_uuid(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 19, 26 };
        WheelValue value = WheelValue::from(WHEEL_CHARACTERISTIC_HARDWARE_REVISION_UUID, bytes, length);

        TEST_ASSERT_EQUAL_STRING(WHEEL_CHARACTERISTIC_HARDWARE_REVISION_UUID, value.getUuid().c_str());
    }

    void test_get_value(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 19, 26 };
        WheelValue value = WheelValue::from(WHEEL_CHARACTERISTIC_HARDWARE_REVISION_UUID, bytes, length);

        TEST_ASSERT_EQUAL(4890, value.getValue());
    }

    void test_get_tuple(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 25, 26 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(25, std::get<0>(value.getTuple()));
        TEST_ASSERT_EQUAL(26, std::get<1>(value.getTuple()));
    }

    void test_to_string(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 12, 34 };
        WheelValue value = WheelValue::from("e659f3fd", bytes, length);
        char expected[] = "WheelValue {uuid=e659f3fd, length=2, bytes=[12, 34] }";

        TEST_ASSERT_EQUAL_STRING(expected, value.toString().c_str());
    }

    void test_formatter_temperature(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 27, 25 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(27, value.format(&WheelValue::FORMATTER_TEMPERATURE_CONTROLLER));
        TEST_ASSERT_EQUAL(25, value.format(&WheelValue::FORMATTER_TEMPERATURE_MOTOR));
    }

    void test_formatter_tilt_angle_pitch(void)
    {
        uint8_t length = 2;
        uint8_t bytes_negative[] = { 7, 213 };
        uint8_t bytes_positive[] = { 6, 234 };
        WheelValue value_negative = WheelValue::from("", bytes_negative, length);
        WheelValue value_positive = WheelValue::from("", bytes_positive, length);

        TEST_ASSERT_EQUAL_FLOAT(-20.5, value_negative.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
        TEST_ASSERT_EQUAL_FLOAT(3.0, value_positive.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
    }

    void test_formatter_tilt_angle_roll(void)
    {
        uint8_t length = 2;
        uint8_t bytes_negative[] = { 7, 215 };
        uint8_t bytes_positive[] = { 6, 236 };
        WheelValue value_negative = WheelValue::from("", bytes_negative, length);
        WheelValue value_positive = WheelValue::from("", bytes_positive, length);

        TEST_ASSERT_EQUAL_FLOAT(-20.7, value_negative.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
        TEST_ASSERT_EQUAL_FLOAT(2.8, value_positive.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
    }

    void test_formatter_tilt_angle_yaw(void)
    {
        uint8_t length = 2;
        uint8_t bytes_negative[] = { 7, 217 };
        uint8_t bytes_positive[] = { 6, 238 };
        WheelValue value_negative = WheelValue::from("", bytes_negative, length);
        WheelValue value_positive = WheelValue::from("", bytes_positive, length);

        TEST_ASSERT_EQUAL_FLOAT(-20.9, value_negative.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
        TEST_ASSERT_EQUAL_FLOAT(2.6, value_positive.format(&WheelValue::FORMATTER_TILT_ANGLE_PITCH));
    }

    void test_formatter_riding_mode(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 0, 5 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(5, value.format(&WheelValue::FORMATTER_RIDING_MODE));
    }

    void test_formatter_battery_remaining(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 0, 99 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(99, value.format(&WheelValue::FORMATTER_BATTERY_REMAINING));
    }

    void test_formatter_battery_low(void)
    {
        uint8_t length = 2;
        uint8_t bytes_on[] = { 0, 1 };
        uint8_t bytes_off[] = { 0, 0 };
        WheelValue value_on = WheelValue::from("", bytes_on, length);
        WheelValue value_off = WheelValue::from("", bytes_off, length);

        TEST_ASSERT_TRUE(value_on.format(&WheelValue::FORMATTER_BATTERY_LOW5));
        TEST_ASSERT_TRUE(value_on.format(&WheelValue::FORMATTER_BATTERY_LOW20));

        TEST_ASSERT_FALSE(value_off.format(&WheelValue::FORMATTER_BATTERY_LOW5));
        TEST_ASSERT_FALSE(value_off.format(&WheelValue::FORMATTER_BATTERY_LOW20));
    }

    void test_formatter_status_error(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 24, 0 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(6144, value.format(&WheelValue::FORMATTER_STATUS_ERROR));
    }

    void test_formatter_meters_per_second(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 2, 248 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(837, WHEEL_TYRE_CIRCUMFERENCE);
        TEST_ASSERT_EQUAL_FLOAT(10.602, value.format(&WheelValue::FORMATTER_METERS_PER_SECOND));
    }

    void test_formatter_kilometers_per_hour(void)
    {
        uint8_t length = 2;
        uint8_t bytes[] = { 1, 135 };
        WheelValue value = WheelValue::from("", bytes, length);

        TEST_ASSERT_EQUAL(837, WHEEL_TYRE_CIRCUMFERENCE);
        TEST_ASSERT_EQUAL_FLOAT(19.63602, value.format(&WheelValue::FORMATTER_KILOMETERS_PER_HOUR));
    }

    void run_tests() {
        RUN_TEST(WheelValueTest::test_get_uuid);
        RUN_TEST(WheelValueTest::test_get_value);
        RUN_TEST(WheelValueTest::test_get_tuple);
        RUN_TEST(WheelValueTest::test_to_string);

        RUN_TEST(WheelValueTest::test_formatter_battery_low);
        RUN_TEST(WheelValueTest::test_formatter_battery_remaining);
        RUN_TEST(WheelValueTest::test_formatter_temperature);

        RUN_TEST(WheelValueTest::test_formatter_tilt_angle_pitch);
        RUN_TEST(WheelValueTest::test_formatter_tilt_angle_roll);
        RUN_TEST(WheelValueTest::test_formatter_tilt_angle_yaw);

        RUN_TEST(WheelValueTest::test_formatter_meters_per_second);
        RUN_TEST(WheelValueTest::test_formatter_kilometers_per_hour);

        RUN_TEST(WheelValueTest::test_formatter_riding_mode);
        RUN_TEST(WheelValueTest::test_formatter_status_error);
    }
}