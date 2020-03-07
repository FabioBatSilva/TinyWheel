#pragma once

#include <esp_adc_cal.h>

#ifndef BOARD_BATTERY_MIN_V
    #define BOARD_BATTERY_MIN_V 3.2
#endif

#ifndef BOARD_BATTERY_MAX_V
    #define BOARD_BATTERY_MAX_V 4.1
#endif

#ifndef BOARD_BATTERY_DEFAULT_VREF
    #define BOARD_BATTERY_DEFAULT_VREF 1100
#endif

class BoardUtilsBattery
{
    protected:
        uint32_t vref;

        float voltage;

        adc_unit_t adcUnit;

        adc_atten_t attenuation;

        adc_bits_width_t bitWidth;

        uint8_t batteryAdcPin;

        uint32_t defaultVref;

        unsigned long lastRead = 0;

    public:
        BoardUtilsBattery(
            adc_unit_t adcUnit,
            adc_atten_t attenuation,
            adc_bits_width_t bitWidth,
            uint8_t batteryAdcPin
        ):
            vref(0),
            voltage(0),
            adcUnit(adcUnit),
            attenuation(attenuation),
            bitWidth(bitWidth),
            batteryAdcPin(batteryAdcPin),
            defaultVref(BOARD_BATTERY_DEFAULT_VREF) { }

        BoardUtilsBattery(uint8_t batteryAdcPin):
            vref(0),
            voltage(0),
            adcUnit((adc_unit_t)ADC_UNIT_1),
            attenuation((adc_atten_t)ADC1_CHANNEL_6),
            bitWidth((adc_bits_width_t)ADC_WIDTH_BIT_12),
            batteryAdcPin(batteryAdcPin),
            defaultVref(BOARD_BATTERY_DEFAULT_VREF) { }

        uint32_t getVref();

        float getVoltage();
};

class BoardUtils
{
    protected:
        static BoardUtilsBattery battery;

    public:
        template<class D>
        static void setup(D* display);

        template<class D>
        static void deepSleep(D* display);

        static float getBatteryVoltage() {
            return battery.getVoltage();
        }

        static uint8_t getBatteryPercentage() {
            const float volts = getBatteryVoltage();
            const float percentage = (volts - BOARD_BATTERY_MIN_V) * 100 / (BOARD_BATTERY_MAX_V - BOARD_BATTERY_MIN_V);

            if (percentage > 100) {
                return 100;
            }

            if (percentage < 0) {
                return 0;
            }

            return (uint8_t)percentage;
        }
};
