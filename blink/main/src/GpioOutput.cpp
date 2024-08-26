#include "GpioOutput.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "Output"

GpioOutput::GpioOutput(const gpio_num_t pin) :
    GpioBase{pin,
                gpio_config_t{.pin_bit_mask = static_cast<uint64_t>(1) << pin,
                              .mode         = GPIO_MODE_OUTPUT,
                              .pull_up_en   = GPIO_PULLUP_DISABLE,
                              .pull_down_en = GPIO_PULLDOWN_ENABLE,
                              .intr_type    = GPIO_INTR_DISABLE}}
{
}


esp_err_t GpioOutput::initalise()
{

    esp_err_t status{GpioBase::initalise()};

    if ( status == ESP_OK)
    {
        status = set(false);
    }   

    return status;
}


esp_err_t GpioOutput::set(const bool state)
{
    m_state = state;

    ESP_LOGI(LOG_TAG, "%i",state);

    return gpio_set_level(m_pin,state);
}


esp_err_t GpioOutput::toggle()
{
    bool state = getState();
    m_state = !state;
    return gpio_set_level(m_pin,m_state);
}


bool GpioOutput::getState()
{
    return m_state;
}
