#pragma once

#include <map>
#include <string>
#include <vector>

#include <WheelValue.h>
#include <WheelDevice.h>

#ifndef WHEEL_CONNECT_FAILURE_DELAY
    #define WHEEL_CONNECT_FAILURE_DELAY 1000
#endif


#include <Arduino.h>

class Wheel
{
    public:
        enum class Status {
            DISCONNECTED,
            CONNECTING,
            CONNECTED,
            ERROR
        };

        class Values {
            protected:
                std::map<std::string, std::tuple<unsigned long, WheelValue>> values;

            public:
                void clear() {
                    values.clear();
                };

                bool empty() {
                    return values.empty();
                };

                WheelValue* get(std::string uuid) {
                    if (values.count(uuid) == 0) {
                        put(uuid, {uuid}, 0);
                    }

                    return &std::get<1>(values.at(uuid));
                };

                void put(std::string uuid, WheelValue value, unsigned long millis) {
                    std::tuple<unsigned long, WheelValue> tuple = std::make_tuple(millis, value);

                    if (values.count(uuid) == 0) {
                        values.insert(std::make_pair(uuid, tuple));

                        return;
                    }

                    values.at(uuid) = tuple;
                };

                unsigned long millis(std::string uuid) {
                    if (values.count(uuid) == 0) {
                        return 0;
                    }

                    return std::get<0>(values.at(uuid));
                };

                bool changed(std::string uuid, unsigned long since) {
                    return this->millis(uuid) > since;
                };
        };

    protected:
        static Values mValues;

        static WheelDevice device;

        static unsigned long failureCount;

        static std::vector<std::string> characteristics;

        static void onDataResult(BLERemoteCharacteristic* remote, uint8_t* data, size_t length, bool notify);

        static bool registerListeners();

        static void reset();

    public:
        static void init(WheelInfo info, std::vector<std::string> characteristics);

        static Values* values() {
            return &mValues;
        };

        static unsigned long connectFails() {
            return failureCount;
        };

        static std::string toString(Wheel::Status state) {
            switch (state) {
                case Wheel::Status::DISCONNECTED: return "DISCONNECTED";
                case Wheel::Status::CONNECTING: return "CONNECTING";
                case Wheel::Status::CONNECTED: return "CONNECTED";
                case Wheel::Status::ERROR: return "ERROR";
            }

            return std::string();
        }

        static Wheel::Status loop();
};
