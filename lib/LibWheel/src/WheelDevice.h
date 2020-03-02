#pragma once

#include <map>
#include <vector>
#include <string>
#include <BLEDevice.h>

#include <WheelInfo.h>
#include <WheelValue.h>
#include <WheelHandshake.h>
#include <WheelCharacteristic.h>

#ifndef HANDSHAKE_INTERVAL
    #define HANDSHAKE_INTERVAL 20000
#endif

#ifndef WHEEL_SCAN_INTERVAL
    #define WHEEL_SCAN_INTERVAL 100
#endif

#ifndef WHEEL_SCAN_DURATION
    #define WHEEL_SCAN_DURATION 4
#endif

#ifndef WHEEL_SCAN_CHECK_DURATION
    #define WHEEL_SCAN_CHECK_DURATION 2
#endif

#ifndef WHEEL_SCAN_WINDOW
    #define WHEEL_SCAN_WINDOW 99
#endif

class WheelDevice
{
    public: enum class State {
        NEW,
        CONNECTED,
        DISCONNECTED,
        READY,
        ERROR
    };

    private:
        WheelInfo info;

        long lastHandshake = 0;

        BLEClient* client = nullptr;

        WheelHandshake wheelHandshake;

        WheelDevice::State state = WheelDevice::State::NEW;

    public:
        static std::string toString(WheelDevice::State state) {
            switch (state) {
                case WheelDevice::State::NEW: return "NEW";
                case WheelDevice::State::CONNECTED: return "CONNECTED";
                case WheelDevice::State::DISCONNECTED: return "DISCONNECTED";
                case WheelDevice::State::READY: return "READY";
                case WheelDevice::State::ERROR: return "ERROR";
            }

            return std::string();
        }

        WheelDevice(): info(WheelInfo()) {}

        WheelDevice(WheelInfo info): info(info) {}

        void reset();

        bool handshake();

        bool isReachable();

        bool connect(bool reachable = false);

        std::string readKey();

        WheelDevice::State currentState();

        bool empty() {
            return this->info.empty();
        }

        bool hasKey() {
            return this->info.hasKey();
        }

        bool isConnected() {
            if (this->client == nullptr) {
                return false;
            }

            return this->client->isConnected();
        }

        WheelInfo* getInfo() {
            return &this->info;
        }

        WheelValue read(std::string uuid) {
            if (this->client == nullptr) {
                return std::string();
            }

            return WheelCharacteristic::read(this->client, uuid);
        }

        bool registerListener(std::string uuid, notify_callback callback) {
            if (this->client == nullptr) {
                return false;
            }

            return WheelCharacteristic::registerListener(this->client, uuid, callback);
        }

        bool removeListener(std::string uuid) {
            if (this->client == nullptr) {
                return false;
            }

            return WheelCharacteristic::removeListener(this->client, uuid);
        }
};

typedef void (*WheelScanResultCallback)(std::vector<WheelInfo> devices);

class WheelDeviceScanner
{
    public:
        size_t scan(WheelScanResultCallback callback);
};
