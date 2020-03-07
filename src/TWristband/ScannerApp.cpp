#include <Wheel.h>
#include <TFT_eSPI.h>
#include <BoardUtils.h>
#include <ScannerApp.h>
#include <EasyButton.h>

EasyButton scannerBtn(TP_PIN_PIN, 80, true, false);

template<> void ScannerApp<TFT_eSPI>::setup() {
    log_i("Setup T-Wristband Scanner");

    BoardUtils::setup<TFT_eSPI>(&ui.display);

    scannerBtn.begin();

    scannerBtn.onPressedFor(3000, []() {
        ScannerApp<TFT_eSPI>::sleep();
    });
};

template<> void ScannerApp<TFT_eSPI>::startDeepSleep() {
    BoardUtils::deepSleep(&ui.display);
};

template<> void ScannerApp<TFT_eSPI>::preLoop() {
    scannerBtn.read();
};