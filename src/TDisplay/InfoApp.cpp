#include <Wheel.h>
#include <TFT_eSPI.h>
#include <ScannerApp.h>

template<> void ScannerApp<TFT_eSPI>::setup() {
    log_i("Setup T-Display Scanner");

    ScannerApp<TFT_eSPI>::UI* ui = &ScannerApp<TFT_eSPI>::ui;

    ui->display.init();
    ui->display.setRotation(0);
};
