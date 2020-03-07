#include <Arduino.h>
#include <macros.h>

#include <Wheel.h>
#include <ScannerApp.h>
#include <WheelCharacteristic.h>

#if DEVICE_UI == DEVICE_UI_TFT
    #include <TFT_eSPI.h>

    #define D_TYPE TFT_eSPI

    template<> TFT_eSPI ScannerApp<D_TYPE>::UI::display = TFT_eSPI();
    template<> View<D_TYPE> ScannerApp<D_TYPE>::UI::view = View<D_TYPE>(&ScannerApp<D_TYPE>::UI::display);
#else
    #error "Unknown DEVICE_UI."
#endif

template<> unsigned long ScannerApp<D_TYPE>::scanCount = 0;
template<> WheelDeviceScanner ScannerApp<D_TYPE>::scanner = WheelDeviceScanner();

template<> ViewPageHome<D_TYPE> ScannerApp<D_TYPE>::UI::viewPageHome = ViewPageHome<D_TYPE>();
template<> ViewPageSleep<D_TYPE> ScannerApp<D_TYPE>::UI::viewPageSleep = ViewPageSleep<D_TYPE>();
template<> ViewPageConnecting<D_TYPE> ScannerApp<D_TYPE>::UI::viewPageConnecting = ViewPageConnecting<D_TYPE>();

template<> void ScannerApp<D_TYPE>::onScanResult(std::vector<WheelInfo> result) {
    log_i("Found %d wheels", result.size());

    if (result.empty()) {
        return;
    }

    for (size_t i = 0; i < result.size(); i++) {
        WheelInfo info = result.at(i);
        WheelDevice wheel = WheelDevice(info);

        log_d("WheelDevice#name    : %s", info.name.c_str());
        log_d("WheelDevice#address : %s", info.address.c_str());

        if (!wheel.connect()) {
            log_e("Failed to connect to %s", info.name.c_str());

            return;
        }

        std::string deviceName = info.name;
        std::string deviceAddress = info.address;
        std::string deviceKey = wheel.readKey();

        Serial.printf("WHEEL_DEVICE_NAME=%s\n", deviceName.c_str());
        Serial.printf("WHEEL_DEVICE_ADDRESS=%s\n", deviceAddress.c_str());
        Serial.printf("WHEEL_DEVICE_API_KEY=%s\n", deviceKey.c_str());
    }
}

template<> void ScannerApp<D_TYPE>::init() {
    log_i("Init TinyWheel Scanner");

    BLEDevice::init("TinyWheel Scanner");

    ui.view.draw(&ui.viewPageHome);
};

template<> void ScannerApp<D_TYPE>::sleep() {
    log_i("Going to sleep");

    ui.view.draw(&ui.viewPageSleep);
    delay(2000);

    startDeepSleep();
};

template<> void ScannerApp<D_TYPE>::loop() {
    preLoop();

    log_i("Scanning...");
    ui.view.draw(&ui.viewPageConnecting);

    if (scanCount++ >= WHEEL_MAX_SCANS) {
        log_e("Failed to find a wheel after %d scans.", scanCount - 1);
        ScannerApp<D_TYPE>::sleep();
    }

    // continue until we find a wheel
    if (scanner.scan(onScanResult) == 0) {
        return;
    }

    log_i("Done...");
    ui.view.draw(&ui.viewPageHome);

    delay(10000);

    ScannerApp<D_TYPE>::sleep();
};
