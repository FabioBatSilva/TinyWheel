#include <vector>

#include <Arduino.h>

#include <WheelDevice.h>
#include <WheelCharacteristic.h>

#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>

class WheelAdvertisedCallback: public BLEAdvertisedDeviceCallbacks {
    public:
        std::vector<WheelInfo> wheels;

        void onResult(BLEAdvertisedDevice device) {
            if (!device.isAdvertisingService({WHEEL_SERVICE_UUID})) {
                return;
            }

            this->wheels.push_back(WheelInfo(
                device.getName(),
                device.getAddress().toString()
            ));
        }
};

class WheelCheckCallback: public BLEAdvertisedDeviceCallbacks {
    public:
        WheelInfo* info;

        bool found;

        WheelCheckCallback(WheelInfo* info):
            info(info),
            found(false) {}

        void onResult(BLEAdvertisedDevice device) {
            std::string address = device.getAddress().toString();

            if (address == info->address) {
                log_d("Found Wheel, stopping scanner.");
                device.getScan()->stop();

                this->found = true;
            }
        }
};

void scanWheelDevices(BLEAdvertisedDeviceCallbacks* callback, uint32_t duration) {
    log_d("Creating scanner reference");
    BLEScan* scan = BLEDevice::getScan();

    log_d("Configuring scanner");
    scan->setAdvertisedDeviceCallbacks(callback);
    scan->setInterval(WHEEL_SCAN_INTERVAL);
    scan->setWindow(WHEEL_SCAN_WINDOW);
    scan->setActiveScan(true);

    log_d("Starting scan");
    scan->start(duration);

    log_d("Clearing results");
    scan->clearResults();

    log_d("Stoping scanner");
    scan->stop();
}

WheelDevice::State WheelDevice::currentState() {
    if (this->client == nullptr) {
        return this->state = WheelDevice::State::NEW;
    }

    if (!this->client->isConnected()) {
        this->client->disconnect();

        return this->state = WheelDevice::State::DISCONNECTED;
    }

    const WheelHandshake::State handshakeStatus = this->wheelHandshake.currentState();

    if (handshakeStatus == WheelHandshake::State::PENDING) {
        return this->state = WheelDevice::State::CONNECTED;
    }

    if (handshakeStatus == WheelHandshake::State::COMPLETED) {
        return this->state = WheelDevice::State::READY;
    }

    return WheelDevice::State::ERROR;
};

void WheelDevice::reset() {
    log_d("Resetting connection with '%s'", this->info.name.c_str());

    if (this->isConnected()) {
        this->client->disconnect();
    }

    this->client = nullptr;
    this->state = WheelDevice::State::NEW;
}

bool WheelDevice::isReachable() {
    log_d("Scanning for wheel at '%s'", this->info.address.c_str());
    WheelCheckCallback callback(&this->info);

    scanWheelDevices(&callback, WHEEL_SCAN_CHECK_DURATION);

    return callback.found;
}

bool WheelDevice::connect(bool reachable) {
    if (this->isConnected()) {
        log_w("Already connected to device");

        return true;
    }

    if (!reachable && !this->isReachable()) {
        log_w("Device not reachable.");
        return false;
    }

    log_d("Starting connection with '%s'", this->info.address.c_str());

    this->client = BLEDevice::createClient();

    // Connect to the remove BLE Server.
    if (!this->client->connect(BLEAddress(this->info.address))) {
        log_e("Failed to connect to '%s'", this->info.address.c_str());

        this->client = nullptr;

        return false;
    }

    log_d("Connected to '%s'", this->info.address.c_str());

    return this->isConnected();
}

std::string WheelDevice::readKey() {
    if (!this->isConnected()) {
        log_e("Device '%s' not connected", this->info.address.c_str());

        return std::string();
    }

    return WheelHandshake::readKey(this->client, &this->info);
}

bool WheelDevice::handshake() {
    if (!this->isConnected()) {
        log_e("Device '%s' not connected", this->info.address.c_str());

        return false;
    }

    // Last handshake is still valid. skip it
    if (lastHandshake > 0 && millis() - lastHandshake < HANDSHAKE_INTERVAL) {
        return true;
    }

    log_d("Starting handshake with '%s'", this->info.address.c_str());

    if (this->wheelHandshake.run(this->client, &this->info) != WheelHandshake::State::COMPLETED) {
        log_e("Failed to shake hands with '%s'", this->info.address.c_str());

        return false;
    }

    log_d("Handshake completed");

    lastHandshake = millis();

    return true;
}

size_t WheelDeviceScanner::scan(WheelScanResultCallback callback) {
    log_d("Scanning for wheels");
    WheelAdvertisedCallback deviceCallback;

    scanWheelDevices(&deviceCallback, WHEEL_SCAN_DURATION);

    log_d("Invoking wheel devices callback");
    callback(deviceCallback.wheels);

    return deviceCallback.wheels.size();
};
