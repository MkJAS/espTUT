#pragma once

#include "GpioBase.h"

#include "led_strip.h"
#include "sdkconfig.h"


class GpioOutput : public GpioBase
{
public:

    GpioOutput(const gpio_num_t pin);

    esp_err_t initalise();

    esp_err_t set(const bool state);
    
    esp_err_t toggle();

    bool getState();

    void setColour(uint32_t red, uint32_t green, uint32_t blue);

private:

    bool m_state = false;

    led_strip_handle_t m_led_strip;

    // RGB values
    uint32_t m_RED = 16;
    uint32_t m_GREEN = 16;
    uint32_t m_BLUE = 16;
    



};