#include <Arduino.h>

#include <WheelUtils.h>
#include <WheelHandshake.h>
#include <WheelCharacteristic.h>

std::vector<uint8_t> WheelHandshake::buffer = {};

bool WheelHandshake::waitForBuffer(size_t expectedSize, uint32_t timeout) {
    int currentSize = 0;
    long timeElapsed = 0;
    long startTime = millis();

    do {
        timeElapsed = millis() - startTime;
        currentSize = WheelHandshake::buffer.size();

        log_d(
            "Received %d of %d bytes after %d ms",
            currentSize,
            expectedSize,
            timeElapsed
        );

        if (currentSize >= expectedSize) {
            break;
        }

        if (timeElapsed > timeout) {
            log_w("Timed out after %d ms", timeElapsed);

            break;
        }

        delay(WHEEL_HANDSHAKE_DELAY_INTERVAL);

    } while (currentSize < expectedSize);

    if (currentSize < expectedSize) {
        log_e("Unable to read %d bytes after %d ms", expectedSize, timeElapsed);

        return false;
    }

    return true;
};

void WheelHandshake::onDataResult(BLERemoteCharacteristic* remote, uint8_t* data, size_t length, bool notify) {
    for (size_t i = 0; i < length; i++) {
        WheelHandshake::buffer.push_back(data[i]);
    }
}

std::string WheelHandshake::readKey(BLEClient* client, WheelInfo* info, uint32_t timeout) {
    if (!client->isConnected()) {
        log_e("Device %s not connected", client->getPeerAddress().toString().c_str());

        return std::string();
    }

    log_d("Clearing buffer");
    WheelHandshake::buffer.clear();
    WheelCharacteristic::removeListener(client, WHEEL_CHARACTERISTIC_UART_SERIAL_READ_UUID);

    log_d("Registering serial read listener");
    WheelCharacteristic::registerListener(client, WHEEL_CHARACTERISTIC_UART_SERIAL_READ_UUID, WheelHandshake::onDataResult);

    log_d("Reading firmware revision");
    WheelValue firmwareRevision = WheelCharacteristic::read(client, WHEEL_CHARACTERISTIC_FIRMWARE_REVISION_UUID);

    if (firmwareRevision.empty()) {
        log_e("Invalid firmware revision");

        return std::string();
    }

    log_d("Writing firmware revision back to wheel");
    if (!WheelCharacteristic::write(client, WHEEL_CHARACTERISTIC_FIRMWARE_REVISION_UUID, firmwareRevision.bytes(), firmwareRevision.size())) {
        log_e("Failed to write firmware revision");

        return std::string();
    }

    if (!WheelHandshake::waitForBuffer(WHEEL_HANDSHAKE_KEY_SIZE, WHEEL_HANDSHAKE_KEY_TIMEOUT)) {
        log_e("Failed to receive serial read data");

        return std::string();
    }

    if (WheelHandshake::buffer.size() < WHEEL_HANDSHAKE_KEY_SIZE) {
        log_e("Failed to read key from buffer.");

        return std::string();
    }

    // Get bytes 3 through 19
    std::vector<uint8_t> buffer = WheelHandshake::buffer;
    std::vector<uint8_t> apiKeyBytes = std::vector<uint8_t>({buffer.begin() + 3, buffer.end() - 1});
    std::string apiKey = WheelUtils::bytesToHexString(&apiKeyBytes);

    log_d("Removing serial read listener.");
    WheelCharacteristic::removeListener(client, WHEEL_CHARACTERISTIC_UART_SERIAL_READ_UUID);

    log_d("Cleaning buffer.");
    WheelHandshake::buffer.clear();

    // log_i("API Key     : %s", apiKey.c_str());
    // log_i("Device Name : %s", info->name.c_str());
    // log_i("Firmware    : %d", firmwareRevision.format(&WheelValue::FORMATTER_FIRMWARE_REVISION));

    return apiKey;
}

WheelHandshake::State WheelHandshake::run(BLEClient* client, WheelInfo* info) {
    if (!client->isConnected()) {
        log_e("Device %s not connected", client->getPeerAddress().toString().c_str());

        return this->state = State::FAILED;
    }

    std::string key = info->key;
    std::vector<uint8_t> bytes = WheelUtils::hexStringToBytes(&key);

    log_d("Writing api key %s to wheel", apiKey.c_str());
    if (!WheelCharacteristic::write(client, WHEEL_CHARACTERISTIC_UART_SERIAL_WRITE_UUID, bytes.data(), bytes.size())) {
        log_e("Failed to write api");

        return this->state = State::FAILED;
    }

    return this->state = State::COMPLETED;
}