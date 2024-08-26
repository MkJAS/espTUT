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

    ESP_LOGI(LOG_TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = m_pin,
        .max_leds = 1, // at least one LED on board
    };
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags{.with_dma = false},
    };
    return led_strip_new_rmt_device(&strip_config, &rmt_config, &m_led_strip);
#elif CONFIG_BLINK_LED_STRIP_BACKEND_SPI
    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };
    return led_strip_new_spi_device(&strip_config, &spi_config, &m_led_strip);
#else
#error "unsupported LED strip backend"
#endif
    /* Set all LED off to clear all pixels */
    led_strip_clear(m_led_strip);
}


esp_err_t GpioOutput::set(const bool state)
{
    m_state = state;

    ESP_LOGI(LOG_TAG, "%i",state);

    return gpio_set_level(m_pin,state);
}


esp_err_t GpioOutput::toggle()
{
    /* If the addressable LED is enabled */
    if (m_state) 
    {
        m_state = false;
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(m_led_strip, 0, m_RED, m_GREEN, m_BLUE);
        /* Refresh the strip to send data */
        return led_strip_refresh(m_led_strip);
    } 
    else 
    {
        m_state = true;
        /* Set all LED off to clear all pixels */
        return led_strip_clear(m_led_strip);
    }
}


bool GpioOutput::getState()
{
    return m_state;
}


void GpioOutput::setColour(uint32_t red, uint32_t green, uint32_t blue) 
{
    m_RED = red;
    m_GREEN = green;
    m_BLUE = blue;
}
