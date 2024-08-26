#pragma once

#include "driver/gpio.h"

class GpioBase
{

public:

    GpioBase(const gpio_num_t pin, const gpio_config_t& config);

    esp_err_t initalise();

    virtual bool getState() = 0;

    virtual esp_err_t set(const bool state) = 0;
    
    //esp_err_t toggle();


protected:

    const gpio_num_t m_pin;
    const gpio_config_t m_cfg;









};







