#include <Wheel.h>
#include <TFT_eSPI.h>
#include <MonitorApp.h>
#include <EasyButton.h>

EasyButton button(TP_PIN_PIN, 80, true, false);

template<> void MonitorApp<TFT_eSPI>::setup() {
    log_i("Setup T-Wristband Monitor");

    MonitorApp<TFT_eSPI>::UI* ui = &MonitorApp<TFT_eSPI>::ui;

    ui->display.init();
    ui->display.setRotation(4);
    ui->display.setSwapBytes(true);

    pinMode(TP_PWR_PIN, PULLUP);
    digitalWrite(TP_PWR_PIN, HIGH);

    button.begin();
    button.onPressed([]() {
        log_d("onClick");

        MonitorApp<TFT_eSPI>::nextPage();
    });

    button.onPressedFor(3000, []() {
        log_d("onLongClick");

        MonitorApp<TFT_eSPI>::sleep();
    });
};

template<> void MonitorApp<TFT_eSPI>::loop() {
    button.read();

    Wheel::Status status = Wheel::loop();
    Wheel::Values* values = Wheel::values();
    uint8_t page = MonitorApp<TFT_eSPI>::page;
    MonitorApp<TFT_eSPI>::UI* ui = &MonitorApp<TFT_eSPI>::ui;

    if (status != Wheel::Status::CONNECTED && button.isPressed()) {
        return;
    }

    if (Wheel::connectFails() > WHEEL_MAX_CONNECT_FAILURE) {
        log_w("Failed to connect %d times.", WHEEL_MAX_CONNECT_FAILURE);
        sleep();
    }

    if (status == Wheel::Status::CONNECTING || status == Wheel::Status::DISCONNECTED) {
        log_d("Not ready yet : %s", Wheel::toString(status).c_str());

        ui->view.draw(&ui->viewPageConnecting);

        return;
    }

    if (status != Wheel::Status::CONNECTED) {
        log_e("Invalid status %s.", Wheel::toString(status).c_str());

        return;
    }

    if (page == VIEW_PAGE_SPEED) {
        ui->view.update(&ui->viewPageSpeed, values);

        return;
    }

    if (page == VIEW_PAGE_BATTERY) {
        ui->view.update(&ui->viewPageBattery, values);

        return;
    }

    if (page == VIEW_PAGE_HOME) {
        ui->view.draw(&ui->viewPageHome);
        return;
    }
};
