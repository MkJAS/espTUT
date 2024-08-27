#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"
#include "nvs_flash.h"


#include "WifiHandler.h"
#include "TcpHandler.h"



extern "C" void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Hello");




    WifiHandler wifi;
    TcpHandler tcpCon;

    esp_err_t status = WIFI_FAILURE;

	//initialize storage
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    status = wifi.connectWifi();
	if (WIFI_SUCCESS != status)
	{
		ESP_LOGI(LOG_TAG, "Failed to associate to AP, dying...");
		return;
	}
    
    tcpCon.createSocket();
    tcpCon.acceptClient();

    GpioOutput led(GPIO_NUM_48);
    led.initalise();

    char buff[] = "Hello Client";
    tcpCon.write(*buff, sizeof(buff));

    while (true)
    {
        std::string readBuff = tcpCon.readSocket();

        ESP_LOGI(LOG_TAG, "readBuff came out as: %s", readBuff.c_str());
        ESP_LOGI(LOG_TAG, "readBuff size: %i", readBuff.length());

        if (readBuff.find("ON") != std::string::npos)
        {
            led.set(true);
        }
        else if (readBuff.find("OFF") != std::string::npos)
        {
            ESP_LOGI(LOG_TAG, "Turning off LED");
            //led.set(false);
            led.toggle();
        }
        else
        {
            ESP_LOGI(LOG_TAG, "Bad buffer read");
        }
    }
    



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