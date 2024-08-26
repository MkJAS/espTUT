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

    while(true)
    {
        led.toggle();
        ESP_LOGD(LOG_TAG, "TOGGLE");
        vTaskDelay(400);
        ESP_LOGD(LOG_TAG, "TOGGLE");
        led.toggle();
        vTaskDelay(400);
    }


}