#include <Arduino.h>

#include <Wheel.h>

WheelDevice Wheel::device;
Wheel::Values Wheel::mValues = {};
unsigned long  Wheel::failureCount = 0;
std::vector<std::string> Wheel::characteristics = {};

void Wheel::onDataResult(BLERemoteCharacteristic* remote, uint8_t* data, size_t length, bool notify) {
    std::string uuid = remote->getUUID().toString();
    WheelValue value = WheelValue::from(uuid, data, length);

    if (value.empty()) {
        log_e("Invalid wheel value : %s", value.toString().c_str());

        return;
    }

    Wheel::mValues.put(uuid, value, millis());

    log_d("%s", value.toString().c_str());
}

void Wheel::reset() {
    log_w("Wheel reset");

    Wheel::device.reset();
    Wheel::mValues.clear();
}

bool Wheel::registerListeners() {

    for (std::string uuid : Wheel::characteristics) {
        unsigned long timestamp = millis();
        WheelValue value = Wheel::device.read(uuid);

        log_d("%s", value.toString().c_str());

        Wheel::mValues.put(uuid, value, timestamp);

        Wheel::device.registerListener(uuid, Wheel::onDataResult);
    }

    return true;
};

void Wheel::init(WheelInfo info, std::vector<std::string> characteristics) {
    BLEDevice::init("TinyWheel monitor");

    Wheel::characteristics = characteristics;
    Wheel::device = WheelDevice(info);

    Wheel::mValues.clear();
};

Wheel::Status Wheel::loop() {

    if (device.empty()) {
        log_e("Invalid wheel info");
        Wheel::failureCount++;

        return Wheel::Status::ERROR;
    }

    WheelDevice::State deviceState = device.currentState();

    if (deviceState == WheelDevice::State::NEW) {
        log_d("State : %s", WheelDevice::toString(deviceState).c_str());

        if (!device.connect()) {
            log_w("Failed to connect.");

            Wheel::failureCount++;
            delay(WHEEL_CONNECT_FAILURE_DELAY);

            return Wheel::Status::CONNECTING;
        }

        return Wheel::Status::CONNECTED;
    }

    if (deviceState == WheelDevice::State::DISCONNECTED) {
        log_d("State : %s ", WheelDevice::toString(deviceState).c_str());

        reset();

        return Wheel::Status::DISCONNECTED;
    }

    if (deviceState == WheelDevice::State::CONNECTED) {
        log_d("State : %s ", WheelDevice::toString(deviceState).c_str());

        if (!device.hasKey()) {
            log_d("Key not set for wheel. Skipping handshake.");

            return Wheel::Status::CONNECTED;
        }

        if (!device.handshake()) {
            log_w("Failed to perform initial handshake.");

            reset();

            return Wheel::Status::CONNECTING;
        }

        // Setup listeners
        Wheel::registerListeners();

        return Wheel::Status::CONNECTED;
    }

    if (deviceState == WheelDevice::State::ERROR) {
        log_d("State : %s ", WheelDevice::toString(deviceState).c_str());

        reset();

        return Wheel::Status::ERROR;
    }

    if (deviceState == WheelDevice::State::READY) {
        // Refresh handshake
        if (!device.handshake()) {
            log_w("Failed to perform handshake.");

            return Wheel::Status::CONNECTING;
        }

        Wheel::failureCount = 0;

        return Wheel::Status::CONNECTED;
    }

    log_w("Unknown state %s.", WheelDevice::toString(deviceState).c_str());

    return Wheel::Status::ERROR;
};
