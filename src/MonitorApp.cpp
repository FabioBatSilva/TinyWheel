#include <Arduino.h>
#include <macros.h>

#include <View.h>
#include <Wheel.h>
#include <MonitorApp.h>
#include <WheelCharacteristic.h>

#if DEVICE_UI == DEVICE_UI_TFT

#include <TFT_eSPI.h>

template<> uint8_t MonitorApp<TFT_eSPI>::page = VIEW_PAGE_SPEED;

template<> TFT_eSPI MonitorApp<TFT_eSPI>::UI::display = TFT_eSPI();
template<> View<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::view = View<TFT_eSPI>(&MonitorApp<TFT_eSPI>::UI::display);

template<> ViewPageHome<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::viewPageHome = ViewPageHome<TFT_eSPI>();
template<> ViewPageSleep<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::viewPageSleep = ViewPageSleep<TFT_eSPI>();
template<> ViewPageSpeed<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::viewPageSpeed = ViewPageSpeed<TFT_eSPI>();
template<> ViewPageBattery<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::viewPageBattery = ViewPageBattery<TFT_eSPI>();
template<> ViewPageConnecting<TFT_eSPI> MonitorApp<TFT_eSPI>::UI::viewPageConnecting = ViewPageConnecting<TFT_eSPI>();

template<> void MonitorApp<TFT_eSPI>::init() {
    log_i("Init TinyWheel Monitor");

    MonitorApp<TFT_eSPI>::UI* ui = &MonitorApp<TFT_eSPI>::ui;

    ui->view.draw(&ui->viewPageHome);

    Wheel::init(
        WheelInfo(
            TOSTRING(WHEEL_DEVICE_NAME),
            TOSTRING(WHEEL_DEVICE_ADDRESS),
            TOSTRING(WHEEL_DEVICE_KEY)
        ),
        {
            WHEEL_CHARACTERISTIC_BATTERY_REMAINING_UUID,
            WHEEL_CHARACTERISTIC_SPEED_RPM_UUID
        }
    );
};

template<> void MonitorApp<TFT_eSPI>::sleep() {
    log_i("Going to sleep");

    MonitorApp<TFT_eSPI>::UI* ui = &MonitorApp<TFT_eSPI>::ui;

    ui->view.draw(&ui->viewPageSleep);
    delay(2000);

    pinMode(39, GPIO_MODE_INPUT);

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
};

#endif