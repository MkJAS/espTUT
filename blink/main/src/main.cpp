#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"
#include "nvs_flash.h"

#include <algorithm>
#include <cctype> 

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
    led.set(false);
    tcpCon.write(buff, sizeof(buff));

    while (true)
    {
        std::string readBuff = tcpCon.readSocket();

        std::string loweredBuff = readBuff;

        std::transform(loweredBuff.begin(), loweredBuff.end(), loweredBuff.begin(),
                   [](unsigned char c){ return std::tolower(c); });

        ESP_LOGI(LOG_TAG, "readBuff came out as: %s", loweredBuff.c_str());
        ESP_LOGI(LOG_TAG, "readBuff size: %i", loweredBuff.length());



        if (loweredBuff.find("on") != std::string::npos)
        {
            led.set(true);
        }
        else if (loweredBuff.find("off") != std::string::npos)
        {
            ESP_LOGI(LOG_TAG, "Turning off LED");
            //led.set(false);
            led.set(false);
        }
        else if (loweredBuff.find("colour") != std::string::npos)
        {
            //COLOR:R,G,B
            int index = loweredBuff.find("colour");
            index = index + 7;
            

            std::string colourValues(loweredBuff,index);

            int redIndex = colourValues.find(",");
            int redValue = std::stoi(colourValues.substr(0,redIndex));
            ESP_LOGI(LOG_TAG, "red value: %i", redValue);

            int greenIndex = colourValues.find(",",redIndex+1);
            //ESP_LOGI(LOG_TAG, "green index: %i", greenIndex);
            int greenValue = std::stoi(colourValues.substr(redIndex+1,greenIndex));
            ESP_LOGI(LOG_TAG, "green value: %i", greenValue);

            ESP_LOGI(LOG_TAG, "string: %s", colourValues.substr(greenIndex+1).c_str());
            int blueValue = std::stoi(colourValues.substr(greenIndex+1));
            ESP_LOGI(LOG_TAG, "blue value: %i", blueValue);

            led.setColour(redValue,greenValue,blueValue);

            if (led.getState())
            {
                led.toggle();
                led.toggle();
            }
            else
            {
                led.toggle();
            }
            
        }
        else
        {
            ESP_LOGI(LOG_TAG, "Bad buffer read");
        }
    }
    


}