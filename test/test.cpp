#include <Arduino.h>
#include <unity.h>

#include "test_wheel.h"
#include "test_view.h"

#define RUN_TEST_GROUP(TEST) \
    if (!std::getenv("TEST_GROUP") || (strcmp(#TEST, std::getenv("TEST_GROUP")) == 0)) { \
        TEST::run_tests(); \
    }

void setup() {
    delay(1000);

    UNITY_BEGIN();

    RUN_TEST_GROUP(WheelTest);
    RUN_TEST_GROUP(ViewTest);

    UNITY_END();
}

void loop() {
}
