#include <TFT_eSPI.h>
#include <BoardUtils.h>

BoardUtilsBattery BoardUtils::battery(ADC_PIN);

template<> void BoardUtils::setup<TFT_eSPI>(TFT_eSPI* display) {
    display->init();
    display->setRotation(0);
    display->setSwapBytes(true);
    display->fillScreen(TFT_BLACK);
};

template<> void BoardUtils::deepSleep<TFT_eSPI>(TFT_eSPI* display) {
    display->writecommand(TFT_DISPOFF);
    display->writecommand(TFT_SLPIN);

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
};
