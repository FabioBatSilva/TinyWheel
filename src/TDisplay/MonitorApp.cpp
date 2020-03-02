#include <Wheel.h>
#include <TFT_eSPI.h>
#include <MonitorApp.h>

template<> void MonitorApp<TFT_eSPI>::setup() {
    log_i("Setup T-Display Monitor");

    MonitorApp<TFT_eSPI>::UI* ui = &MonitorApp<TFT_eSPI>::ui;

    ui->display.init();
    ui->display.setRotation(0);
};

template<> void MonitorApp<TFT_eSPI>::loop() {

};
