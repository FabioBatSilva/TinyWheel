#include <Wheel.h>
#include <TFT_eSPI.h>
#include <BoardUtils.h>
#include <MonitorApp.h>
#include <EasyButton.h>

EasyButton monitorBtn(TP_PIN_PIN, 80, true, false);

template<> void MonitorApp<TFT_eSPI>::setup() {
    log_i("Setup T-Wristband Monitor");

    BoardUtils::setup<TFT_eSPI>(&ui.display);

    monitorBtn.begin();
    monitorBtn.onPressed([]() {
        log_d("onClick");

        MonitorApp<TFT_eSPI>::nextPage();
    });

    monitorBtn.onPressedFor(3000, []() {
        log_d("onLongClick");

        MonitorApp<TFT_eSPI>::sleep();
    });
};

template<> void MonitorApp<TFT_eSPI>::startDeepSleep() {
    BoardUtils::deepSleep(&ui.display);
};

template<> void MonitorApp<TFT_eSPI>::preLoop() {
    monitorBtn.read();
};
