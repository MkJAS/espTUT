#include "GpioBase.h"


GpioBase::GpioBase(const gpio_num_t pin, const gpio_config_t& config) : m_pin{pin}, m_cfg{config}
{

}



esp_err_t GpioBase::initalise()
{
    esp_err_t status{ESP_OK};

    status = gpio_config(&m_cfg);

    return status;
}
