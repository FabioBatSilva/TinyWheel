#pragma once

#include <tuple>
#include <string>
#include <vector>

#ifndef WHEEL_DEVICE_TYPE_XR
    #define WHEEL_DEVICE_TYPE_XR 1
#endif

#ifndef WHEEL_DEVICE_TYPE_PINT
    #define WHEEL_DEVICE_TYPE_PINT 2
#endif

#ifndef WHEEL_TYRE_CIRCUMFERENCE
    #if WHEEL_DEVICE_TYPE == WHEEL_DEVICE_TYPE_XR
        #define WHEEL_TYRE_CIRCUMFERENCE 910
    #else
        #define WHEEL_TYRE_CIRCUMFERENCE 837
    #endif
#endif

template <class T>
class WheelValueFormatter
{
    public:
        virtual T format(uint8_t* data, size_t length);
};

class WheelValueFormatterBool : public WheelValueFormatter<bool>
{
    public:
        bool format(uint8_t* data, size_t length);
};

class WheelValueFormatterUInt8 : public WheelValueFormatter<uint8_t>
{
    private:
        uint8_t part;

    public:
        WheelValueFormatterUInt8(uint8_t part) {
            this->part = part;
        }

        uint8_t format(uint8_t* data, size_t length);
};

class WheelValueFormatterUInt16 : public WheelValueFormatter<uint16_t>
{
    public:
        uint16_t format(uint8_t* data, size_t length);
};

class WheelValueFormatterAngle : public WheelValueFormatter<float>
{
    public:
        float format(uint8_t* data, size_t length);
};

class WheelValueFormatterFloat : public WheelValueFormatter<float>
{
    public:
        float format(uint8_t* data, size_t length);
};

class WheelValueFormatterMetersPerSecond : public WheelValueFormatter<float>
{
    public:
        float format(uint8_t* data, size_t length);
};

class WheelValueFormatterKilometersPerHour : public WheelValueFormatter<float>
{
    protected:
        WheelValueFormatterMetersPerSecond metersPerSecondFormatter = {};

    public:
        float format(uint8_t* data, size_t length);
};

class WheelValue
{
    private:
        std::string uuid;
        std::vector<uint8_t> data;

    public:
        static WheelValueFormatterUInt16 FORMATTER_RIDING_MODE;

        static WheelValueFormatterUInt16 FORMATTER_BATTERY_REMAINING;

        static WheelValueFormatterBool FORMATTER_BATTERY_LOW5;

        static WheelValueFormatterBool FORMATTER_BATTERY_LOW20;

        static WheelValueFormatterAngle FORMATTER_TILT_ANGLE_PITCH;

        static WheelValueFormatterAngle FORMATTER_TILT_ANGLE_ROLL;

        static WheelValueFormatterAngle FORMATTER_TILT_ANGLE_YAW;

        static WheelValueFormatterUInt8 FORMATTER_TEMPERATURE_CONTROLLER;

        static WheelValueFormatterUInt8 FORMATTER_TEMPERATURE_MOTOR;

        static WheelValueFormatterUInt8 FORMATTER_TEMPERATURE_BATTERY;

        static WheelValueFormatterUInt16 FORMATTER_STATUS_ERROR;

        static WheelValueFormatterUInt16 FORMATTER_FIRMWARE_REVISION;

        static WheelValueFormatterUInt16 FORMATTER_HARDWARE_REVISION;

        static WheelValueFormatterUInt16 FORMATTER_LAST_ERROR_CODE;

        static WheelValueFormatterUInt16 FORMATTER_LIGHTING_MODE;

        static WheelValueFormatterUInt16 FORMATTER_LIGHTS_BACK;

        static WheelValueFormatterUInt16 FORMATTER_LIGHTS_FRONT;

        static WheelValueFormatterUInt16 FORMATTER_ODOMETER;

        static WheelValueFormatterUInt16 FORMATTER_SAFETY_HEADROOM;

        static WheelValueFormatterUInt16 FORMATTER_SPEED_RPM;

        static WheelValueFormatterMetersPerSecond FORMATTER_METERS_PER_SECOND;

        static WheelValueFormatterKilometersPerHour FORMATTER_KILOMETERS_PER_HOUR;

        static WheelValue from(std::string uuid, uint8_t* bytes, size_t length) {
            std::vector<uint8_t> data(length);

            for (size_t i = 0; i < length; i++)
            {
                data[i] = bytes[i];
            }

            return WheelValue(uuid, data);
        }

        WheelValue(std::string uuid):
            uuid(uuid),
            data(std::vector<uint8_t>(0)) {}

        WheelValue(std::string uuid, std::vector<uint8_t> data):
            uuid(uuid),
            data(data) {}

        std::string getUuid() {
            return this->uuid;
        }

        uint8_t* bytes() {
            return this->data.data();
        }

        size_t size() {
            return this->data.size();
        }

        bool empty() {
            return this->data.empty();
        }

        uint16_t getValue();

        std::tuple<uint8_t, uint8_t> getTuple();

        std::string toString();

        bool format(WheelValueFormatter<bool>* formatter);

        float format(WheelValueFormatter<float>* formatter);

        uint8_t format(WheelValueFormatter<uint8_t>* formatter);

        uint16_t format(WheelValueFormatter<uint16_t>* formatter);
};
