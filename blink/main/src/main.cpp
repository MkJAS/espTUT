#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"


extern "C" void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Hello");
}