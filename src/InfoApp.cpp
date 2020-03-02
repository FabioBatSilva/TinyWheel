#include <Arduino.h>
#include <macros.h>

#include <Wheel.h>
#include <InfoApp.h>
#include <WheelCharacteristic.h>

#if DEVICE_UI == DEVICE_UI_TFT

#include <TFT_eSPI.h>

template<> std::string InfoApp<TFT_eSPI>::key = std::string();
template<> TFT_eSPI InfoApp<TFT_eSPI>::UI::display = TFT_eSPI();
template<> WheelDeviceScanner InfoApp<TFT_eSPI>::scanner = WheelDeviceScanner();
template<> View<TFT_eSPI> InfoApp<TFT_eSPI>::UI::view = View<TFT_eSPI>(&InfoApp<TFT_eSPI>::UI::display);

template<> ViewPageHome<TFT_eSPI> InfoApp<TFT_eSPI>::UI::viewPageHome = ViewPageHome<TFT_eSPI>();
template<> ViewPageSleep<TFT_eSPI> InfoApp<TFT_eSPI>::UI::viewPageSleep = ViewPageSleep<TFT_eSPI>();
template<> ViewPageConnecting<TFT_eSPI> InfoApp<TFT_eSPI>::UI::viewPageConnecting = ViewPageConnecting<TFT_eSPI>();

template<> void InfoApp<TFT_eSPI>::onScanResult(std::vector<WheelInfo> result) {
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

template<> void InfoApp<TFT_eSPI>::init() {
    log_i("Init TinyWheel Info");

    BLEDevice::init("TinyWheel info");

    InfoApp<TFT_eSPI>::UI* ui = &InfoApp<TFT_eSPI>::ui;

    ui->view.draw(&ui->viewPageHome);
    delay(2000);

    log_i("Scanning...");
    ui->view.draw(&ui->viewPageConnecting);

    scanner.scan(InfoApp<TFT_eSPI>::onScanResult);

    log_i("Done...");
    ui->view.draw(&ui->viewPageHome);
};

template<> void InfoApp<TFT_eSPI>::sleep() {
    log_i("Going to sleep");

    InfoApp<TFT_eSPI>::UI* ui = &InfoApp<TFT_eSPI>::ui;

    ui->view.draw(&ui->viewPageSleep);
    delay(2000);

    pinMode(39, GPIO_MODE_INPUT);

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
};

template<> void InfoApp<TFT_eSPI>::loop() {
    delay(10000);

    InfoApp<TFT_eSPI>::sleep();
};

#endif