#ifndef DEVICE_UI
    #error "DEVICE_UI not defined."
#endif

#ifndef APP
    #error "APP not defined."
#endif

#if DEVICE_UI == DEVICE_UI_TFT
    #include <TFT_eSPI.h>
    #define UI_DISPLAY TFT_eSPI
#else
    #error "Unknown DEVICE_UI."
#endif

#if APP == APP_MONITOR
    #include <MonitorApp.h>
    #define APP_TYPE MonitorApp
#elif APP == APP_INFO
    #include <InfoApp.h>
    #define APP_TYPE InfoApp
#else
    #error "Unknown APP."
#endif

#include <Arduino.h>

void setup() {
    log_i("Starting ...");

    Serial.begin(115200);

    log_i("Setup ...");

    APP_TYPE<UI_DISPLAY>::setup();

    log_i("init ...");
    APP_TYPE<UI_DISPLAY>::init();
}

void loop() {
    APP_TYPE<UI_DISPLAY>::loop();
}
