#include <Wheel.h>
#include <TFT_eSPI.h>
#include <ScannerApp.h>
#include <EasyButton.h>

template<> void ScannerApp<TFT_eSPI>::setup() {
    log_i("Setup T-Wristband Scanner");

    ScannerApp<TFT_eSPI>::UI* ui = &ScannerApp<TFT_eSPI>::ui;
    static EasyButton button(TP_PIN_PIN, 80, true, false);

    ui->display.init();
    ui->display.setRotation(4);
    ui->display.setSwapBytes(true);

    pinMode(TP_PWR_PIN, PULLUP);
    digitalWrite(TP_PWR_PIN, HIGH);

    button.begin();

    button.onPressedFor(3000, []() {
        ScannerApp<TFT_eSPI>::sleep();
    });
};
