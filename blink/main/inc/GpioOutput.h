#pragma once

#include "GpioBase.h"


class GpioOutput : public GpioBase
{
public:

    GpioOutput(const gpio_num_t pin);

    esp_err_t initalise();

    esp_err_t set(const bool state);
    
    esp_err_t toggle();

    bool getState();

private:

    bool m_state = false;



};