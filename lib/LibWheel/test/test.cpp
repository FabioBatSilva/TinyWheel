#include <Arduino.h>
#include <unity.h>

#include "test_wheel_utils.h"
#include "test_wheel_value.h"

#define RUN_TEST_GROUP(TEST) \
    if (!std::getenv("TEST_GROUP") || (strcmp(#TEST, std::getenv("TEST_GROUP")) == 0)) { \
        TEST::run_tests(); \
    }

void setup() {
    delay(1000);

    UNITY_BEGIN();

    RUN_TEST_GROUP(WheelUtilsTest);
    RUN_TEST_GROUP(WheelValueTest);

    UNITY_END();
}

void loop() {
}
