#include <Wheel.h>
#include <TFT_eSPI.h>
#include <InfoApp.h>

template<> void InfoApp<TFT_eSPI>::setup() {
    log_i("Setup T-Display Info");

    InfoApp<TFT_eSPI>::UI* ui = &InfoApp<TFT_eSPI>::ui;

    ui->display.init();
    ui->display.setRotation(0);
};
