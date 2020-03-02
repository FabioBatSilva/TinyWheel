#include <WheelUtils.h>
#include <WheelValue.h>

#include <Arduino.h>
#include <stdio.h>

uint16_t WheelValue::getValue() {
    return WheelUtils::bytesToUint16(this->bytes(), this->size());
}

std::tuple<uint8_t, uint8_t> WheelValue::getTuple() {
    if (this->size() < 2) {
        return std::make_tuple(0, 0);
    }

    return std::make_tuple(
        WheelUtils::byteToUint8(this->bytes()[0]),
        WheelUtils::byteToUint8(this->bytes()[1])
    );
}

std::string WheelValue::toString() {
    std::string result("WheelValue {");
    char buffer[6];

    sprintf(buffer, "%d", this->size());

    result = result + "uuid=" + this->uuid.c_str() + ", ";
    result = result + "length=" + buffer + ", ";
    result = result + "bytes=" + "[";

    for (int i = 0; i < this->size(); i++) {
        sprintf(buffer, "%d", this->bytes()[i]);

        result.append(buffer);

        if (i + 1 < this->size()) {
            result.append(", ");
        }
    }

    result.append("] }");

    return result;
}

float WheelValue::format(WheelValueFormatter<float>* formatter) {
    return formatter->format(this->bytes(), this->size());
}

uint8_t WheelValue::format(WheelValueFormatter<uint8_t>* formatter) {
    return formatter->format(this->bytes(), this->size());
}

uint16_t WheelValue::format(WheelValueFormatter<uint16_t>* formatter) {
    return formatter->format(this->bytes(), this->size());
}

bool WheelValue::format(WheelValueFormatter<bool>* formatter) {
    return formatter->format(this->bytes(), this->size());
}

bool WheelValueFormatterBool::format(uint8_t* data, size_t length) {
    return WheelUtils::bytesToUint16(data, length);
}

uint8_t WheelValueFormatterUInt8::format(uint8_t* data, size_t length) {
    if (length < this->part) {
        return 0;
    }

    return WheelUtils::byteToUint8(data[this->part]);
}

uint16_t WheelValueFormatterUInt16::format(uint8_t* data, size_t length) {
    return WheelUtils::bytesToUint16(data, length);
}

float WheelValueFormatterAngle::format(uint8_t* data, size_t length) {
    return 0.1f * (1800 - WheelUtils::bytesToUint16(data, length));
}

float WheelValueFormatterFloat::format(uint8_t* data, size_t length) {
    return 0.1f * WheelUtils::bytesToUint16(data, length);
}

float WheelValueFormatterMetersPerSecond::format(uint8_t* data, size_t length) {
    const uint16_t rpm = WheelUtils::bytesToUint16(data, length);
    const float circumference = WHEEL_TYRE_CIRCUMFERENCE; //mm
    const float radius = circumference / TWO_PI / 1000; // In meters
    const float radPerSecond = (TWO_PI / 60) * rpm;
    const float metersPerSecond = radius * radPerSecond;

    // log_d("-------------------");
    // log_d("rpm             : %d", rpm);
    // log_d("circumference   : %.2f", circumference);
    // log_d("radius          : %.2f", radius);
    // log_d("radPerSecond    : %.2f", radPerSecond);
    // log_d("metersPerSecond : %.2f", metersPerSecond);

    return metersPerSecond;
}

float WheelValueFormatterKilometersPerHour::format(uint8_t* data, size_t length) {
    const float metersPerSecond = metersPerSecondFormatter.format(data, length);
    const float kilometersPerHour = metersPerSecond * 3.6f; // kmph

    return kilometersPerHour;
}

WheelValueFormatterUInt16 WheelValue::FORMATTER_RIDING_MODE;

WheelValueFormatterUInt16 WheelValue::FORMATTER_BATTERY_REMAINING;

WheelValueFormatterBool WheelValue::FORMATTER_BATTERY_LOW5;

WheelValueFormatterBool WheelValue::FORMATTER_BATTERY_LOW20;

WheelValueFormatterAngle WheelValue::FORMATTER_TILT_ANGLE_PITCH;

WheelValueFormatterAngle WheelValue::FORMATTER_TILT_ANGLE_ROLL;

WheelValueFormatterAngle WheelValue::FORMATTER_TILT_ANGLE_YAW;

WheelValueFormatterUInt8 WheelValue::FORMATTER_TEMPERATURE_CONTROLLER = {0};

WheelValueFormatterUInt8 WheelValue::FORMATTER_TEMPERATURE_MOTOR = {1};

WheelValueFormatterUInt8 WheelValue::FORMATTER_TEMPERATURE_BATTERY = {0};

WheelValueFormatterUInt16 WheelValue::FORMATTER_STATUS_ERROR;

WheelValueFormatterUInt16 WheelValue::FORMATTER_FIRMWARE_REVISION;

WheelValueFormatterUInt16 WheelValue::FORMATTER_HARDWARE_REVISION;

WheelValueFormatterUInt16 WheelValue::FORMATTER_LAST_ERROR_CODE;

WheelValueFormatterUInt16 WheelValue::FORMATTER_LIGHTING_MODE;

WheelValueFormatterUInt16 WheelValue::FORMATTER_LIGHTS_BACK;

WheelValueFormatterUInt16 WheelValue::FORMATTER_LIGHTS_FRONT;

WheelValueFormatterUInt16 WheelValue::FORMATTER_ODOMETER;

WheelValueFormatterUInt16 WheelValue::FORMATTER_SAFETY_HEADROOM;

WheelValueFormatterUInt16 WheelValue::FORMATTER_SPEED_RPM;

WheelValueFormatterMetersPerSecond WheelValue::FORMATTER_METERS_PER_SECOND;

WheelValueFormatterKilometersPerHour WheelValue::FORMATTER_KILOMETERS_PER_HOUR;
