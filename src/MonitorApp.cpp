#include <Arduino.h>
#include <macros.h>

#include <View.h>
#include <Wheel.h>
#include <MonitorApp.h>
#include <BoardUtils.h>
#include <WheelCharacteristic.h>

#if DEVICE_UI == DEVICE_UI_TFT
    #include <TFT_eSPI.h>

    #define D_TYPE TFT_eSPI

    template<> TFT_eSPI MonitorApp<D_TYPE>::UI::display = TFT_eSPI();
    template<> View<D_TYPE> MonitorApp<D_TYPE>::UI::view = View<D_TYPE>(&MonitorApp<D_TYPE>::UI::display);
#else
    #error "Unknown DEVICE_UI."
#endif

template<> uint8_t MonitorApp<TFT_eSPI>::page = VIEW_PAGE_SPEED;

template<> ViewPageHome<D_TYPE> MonitorApp<D_TYPE>::UI::viewPageHome = ViewPageHome<D_TYPE>();
template<> ViewPageSleep<D_TYPE> MonitorApp<D_TYPE>::UI::viewPageSleep = ViewPageSleep<D_TYPE>();
template<> ViewPageSpeed<D_TYPE> MonitorApp<D_TYPE>::UI::viewPageSpeed = ViewPageSpeed<D_TYPE>();
template<> ViewPageBattery<D_TYPE> MonitorApp<D_TYPE>::UI::viewPageBattery = ViewPageBattery<D_TYPE>();
template<> ViewPageConnecting<D_TYPE> MonitorApp<D_TYPE>::UI::viewPageConnecting = ViewPageConnecting<D_TYPE>();

template<> void MonitorApp<D_TYPE>::init() {
    log_i("Init TinyWheel Monitor");

    ui.view.draw(&ui.viewPageHome);

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

template<> void MonitorApp<D_TYPE>::sleep() {
    log_i("Going to sleep");

    ui.view.draw(&ui.viewPageSleep);
    delay(2000);

    startDeepSleep();
};

template<> void MonitorApp<D_TYPE>::loop() {
    preLoop();

    Wheel::Status status = Wheel::loop();
    Wheel::Values* values = Wheel::values();

    if (Wheel::connectFails() > WHEEL_MAX_CONNECT_FAILURE) {
        log_e("Failed to connect %d times.", WHEEL_MAX_CONNECT_FAILURE);
        sleep();
    }

    if (status == Wheel::Status::CONNECTING || status == Wheel::Status::DISCONNECTED) {
        log_d("Not ready yet : %s", Wheel::toString(status).c_str());

        ui.view.draw(&ui.viewPageConnecting);

        return;
    }

    if (status != Wheel::Status::CONNECTED) {
        log_e("Invalid status %s.", Wheel::toString(status).c_str());

        return;
    }

    switch (page)
    {
        case VIEW_PAGE_SPEED:
            ui.view.update(&ui.viewPageSpeed, values);
            break;
        case VIEW_PAGE_BATTERY:
            ui.view.update(&ui.viewPageBattery, values);
            break;
        case VIEW_PAGE_HOME:
            ui.view.draw(&ui.viewPageHome);
            break;
        default:
            log_e("Invalid page %d.", page);
            break;
    }

    delay(100);
};