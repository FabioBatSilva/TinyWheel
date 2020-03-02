#include <WheelUtils.cpp>
#include <unity.h>

namespace WheelUtilsTest
{
    void test_bytesToUint16(void)
    {
        uint8_t bytes[] = { 19, 26 };
        uint16_t result = WheelUtils::bytesToUint16(bytes, 2);

        TEST_ASSERT_EQUAL(4890, result);
    }

    void test_hexStringToBytes(void)
    {
        std::string hexString = "7465787420746F206865780D0A";
        std::vector<uint8_t> bytes = WheelUtils::hexStringToBytes(&hexString);
        std::vector<uint8_t> expected = {116, 101, 120, 116, 32, 116, 111, 32, 104, 101, 120, 13, 10};

        TEST_ASSERT_EQUAL(expected.size(), bytes.size());

        for (int i = 0; i < bytes.size(); i++)
        {
            TEST_ASSERT_EQUAL(expected[i], bytes[i]);
        }
    }

    void test_bytesToHexString(void)
    {
        std::vector<uint8_t> bytes = {116, 101, 120, 116, 32, 116, 111, 32, 104, 101, 120, 13, 10};
        std::string hexString1 = WheelUtils::bytesToHexString(bytes.data(), bytes.size());
        std::string hexString2 = WheelUtils::bytesToHexString(&bytes);
        std::string expected = "7465787420746F206865780D0A";

        TEST_ASSERT_EQUAL_STRING(expected.c_str(), hexString1.c_str());
        TEST_ASSERT_EQUAL_STRING(expected.c_str(), hexString2.c_str());
    }

    void run_tests() {
        RUN_TEST(WheelUtilsTest::test_bytesToUint16);
        RUN_TEST(WheelUtilsTest::test_hexStringToBytes);
        RUN_TEST(WheelUtilsTest::test_bytesToHexString);
    }
}