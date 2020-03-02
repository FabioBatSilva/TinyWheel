#pragma once

#include <string>

#include <BLEDevice.h>

#include <WheelInfo.h>
#include <WheelHandshake.h>

#define WHEEL_HANDSHAKE_KEY_SIZE 20
#define WHEEL_HANDSHAKE_KEY_TIMEOUT 2000
#define WHEEL_HANDSHAKE_DELAY_INTERVAL 20

class WheelHandshake {
    public: enum class State {
        PENDING,
        COMPLETED,
        FAILED
    };

    private:
        State state = State::PENDING;

    protected:
        static std::vector<uint8_t> buffer;

        static bool waitForBuffer(size_t length, uint32_t timeout);

        static void onDataResult(BLERemoteCharacteristic* remote, uint8_t* data, size_t length, bool notify);

    public:
        static std::string readKey(BLEClient* client, WheelInfo* info, uint32_t timeout = WHEEL_HANDSHAKE_KEY_TIMEOUT);

        State run(BLEClient* client, WheelInfo* info);

        State currentState() {
            return this->state;
        }
};
