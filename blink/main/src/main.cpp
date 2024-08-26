#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

#define LED_NUM GPIO_NUM_48

extern "C" void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Hello");


    GpioOutput led(GPIO_NUM_48);

    led.initalise();

    ESP_LOGI(LOG_TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(LED_NUM);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_NUM, GPIO_MODE_OUTPUT);

    while(true)
    {
        led.set(true);
        ESP_LOGI(LOG_TAG, "ON");
        vTaskDelay(400);
        ESP_LOGI(LOG_TAG, "OFF");
        led.set(false);
        vTaskDelay(400);
    }


}