#include <Wheel.h>
#include <TFT_eSPI.h>
#include <BoardUtils.h>
#include <ScannerApp.h>
#include <EasyButton.h>

EasyButton scannerBtn2(BUTTON_2, 80, true, true);

template<> void ScannerApp<TFT_eSPI>::setup() {
    log_i("Setup T-Display Scanner");

    BoardUtils::setup<TFT_eSPI>(&ui.display);

    scannerBtn2.begin();

    scannerBtn2.onPressedFor(3000, []() {
        ScannerApp<TFT_eSPI>::sleep();
    });
};

template<> void ScannerApp<TFT_eSPI>::startDeepSleep() {
    BoardUtils::deepSleep(&ui.display);
};

template<> void ScannerApp<TFT_eSPI>::preLoop() {
    scannerBtn2.read();
};