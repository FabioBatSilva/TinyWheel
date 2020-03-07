
#include <Arduino.h>
#include <BoardUtils.h>

uint32_t BoardUtilsBattery::getVref() {
    if (this->vref > 0) {
        log_d("Reusing existing Vref:%u mV", this->vref);
        return this->vref;
    }

    esp_adc_cal_characteristics_t adcChars;
    esp_adc_cal_value_t valType = esp_adc_cal_characterize(
        this->adcUnit,
        this->attenuation,
        this->bitWidth,
        this->defaultVref,
        &adcChars
    );

    //Check type of calibration value used to characterize ADC
    switch (valType)
    {
        case ESP_ADC_CAL_VAL_EFUSE_VREF:
            log_d("eFuse Vref:%u mV", adcChars.vref);
            break;
        case ESP_ADC_CAL_VAL_EFUSE_TP:
            log_d("Two Point --> coeff_a:%umV coeff_b:%umV", adcChars.coeff_a, adcChars.coeff_b);
            break;
        default:
            log_d("Default Vref:%u mV", this->defaultVref);
            break;
    }

    log_d("Read Vref:%u mV", adcChars.vref);

    return this->vref = adcChars.vref;
};

float BoardUtilsBattery::getVoltage() {
    if (this->voltage > 0 && millis() - this->lastRead < 10000) {
        log_d("Reusing existing voltage:%f", this->voltage);
        return this->voltage;
    }

    const uint32_t vref = this->getVref();
    const uint16_t read = analogRead(this->batteryAdcPin);
    const float value = ((float)read / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);

    this->lastRead = millis();
    this->voltage = value;

    log_d("Read voltage:%f", this->voltage);

    return this->voltage;
};
