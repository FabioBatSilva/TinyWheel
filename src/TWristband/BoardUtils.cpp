#include <TFT_eSPI.h>
#include <BoardUtils.h>

BoardUtilsBattery BoardUtils::battery(BATT_ADC_PIN);

template<> void BoardUtils::setup<TFT_eSPI>(TFT_eSPI* display) {
    display->init();
    display->setRotation(4);
    display->setSwapBytes(true);
    display->fillScreen(TFT_BLACK);

    pinMode(TP_PWR_PIN, PULLUP);
    digitalWrite(TP_PWR_PIN, HIGH);
};

template<> void BoardUtils::deepSleep<TFT_eSPI>(TFT_eSPI* display) {
    pinMode(39, GPIO_MODE_INPUT);

    display->writecommand(ST7735_SWRESET);
    display->writecommand(ST7735_SLPIN);
    display->writecommand(ST7735_DISPOFF);

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
};
