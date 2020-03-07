#include <Wheel.h>
#include <TFT_eSPI.h>
#include <BoardUtils.h>
#include <MonitorApp.h>
#include <EasyButton.h>

EasyButton monitorBtn2(BUTTON_2, 80, true, true);

template<> void MonitorApp<TFT_eSPI>::setup() {
    log_i("Setup T-Display Monitor");

    BoardUtils::setup<TFT_eSPI>(&ui.display);

    monitorBtn2.begin();
    monitorBtn2.onPressed([]() {
        log_d("onClick");

        MonitorApp<TFT_eSPI>::nextPage();
    });

    monitorBtn2.onPressedFor(3000, []() {
        log_d("onLongClick");

        MonitorApp<TFT_eSPI>::sleep();
    });
};

template<> void MonitorApp<TFT_eSPI>::startDeepSleep() {
    BoardUtils::deepSleep(&ui.display);
};

template<> void MonitorApp<TFT_eSPI>::preLoop() {
    monitorBtn2.read();
};
