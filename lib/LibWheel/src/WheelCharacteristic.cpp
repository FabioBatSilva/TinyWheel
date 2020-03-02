#include <Arduino.h>
#include <WheelValue.h>
#include <WheelCharacteristic.h>

BLERemoteService* WheelCharacteristic::getRemoteService(BLEClient* client) {
    if (client == nullptr) {
        return nullptr;
    }

    return client->getService({WHEEL_SERVICE_UUID});
};

BLERemoteCharacteristic* WheelCharacteristic::getRemoteCharacteristic(BLEClient* client, std::string uuid) {
    BLERemoteService* remote = getRemoteService(client);

    if (remote == nullptr) {
        return nullptr;
    }

    return remote->getCharacteristic({uuid});
};

WheelValue WheelCharacteristic::read(BLEClient* client, std::string uuid) {
    BLERemoteCharacteristic* characteristic = getRemoteCharacteristic(client, uuid);

    if (characteristic == nullptr) {
        log_e("Failed to find characteristic : %s", uuid.c_str());

        return WheelValue(uuid);
    }

    std::string value = characteristic->readValue();
    uint8_t length = value.length();
    uint8_t bytes[length];

    memcpy(bytes, value.data(), value.length());

    return WheelValue::from(uuid, bytes, length);
};

bool WheelCharacteristic::write(BLEClient* client, std::string uuid, uint8_t* bytes, size_t length) {
    BLERemoteCharacteristic* characteristic = getRemoteCharacteristic(client, uuid);

    if (characteristic == nullptr) {
        log_e("Failed to find characteristic : %s", uuid.c_str());

        return false;
    }

    characteristic->writeValue(bytes, length, true);

    return true;
};

bool WheelCharacteristic::registerListener(BLEClient* client, std::string uuid, notify_callback callback) {
    BLERemoteCharacteristic* characteristic = getRemoteCharacteristic(client, uuid);

    if (characteristic == nullptr) {
        log_e("Unable to register listener for characteristic : %s", uuid.c_str());

        return false;
    }

    log_d("Registering listener for characteristic : %s", uuid.c_str());
    characteristic->registerForNotify(callback, true);

    return true;
};

bool WheelCharacteristic::removeListener(BLEClient* client, std::string uuid) {
    BLERemoteCharacteristic* characteristic = getRemoteCharacteristic(client, uuid);

    if (characteristic == nullptr) {
        log_e("Unable to remove listener for characteristic : %s", uuid.c_str());

        return false;
    }

    log_d("Removing listener for characteristic : %s", uuid.c_str());
    characteristic->registerForNotify(nullptr, true);

    return true;
};
