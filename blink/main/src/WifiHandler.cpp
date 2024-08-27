#include "WifiHandler.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "WIFI"



EventGroupHandle_t WifiHandler::m_wifiEventGroup = nullptr;
int WifiHandler::m_retryCount = 0;


WifiHandler::WifiHandler()
{
    m_retryCount = 0;
}

esp_err_t WifiHandler::connectWifi()
{
    int status = WIFI_FAILURE;

	/** INITIALIZE ALL THE THINGS **/
	//initialize the esp network interface
	ESP_ERROR_CHECK(esp_netif_init());

	//initialize default esp event loop
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	//create wifi station in the wifi driver
	esp_netif_create_default_wifi_sta();

	//setup wifi station with the default wifi configuration
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /** EVENT LOOP CRAZINESS **/
	m_wifiEventGroup = xEventGroupCreate();

    esp_event_handler_instance_t wifi_handler_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &WifiHandler::wifiEventHandler,
                                                        NULL,
                                                        &wifi_handler_event_instance));

    esp_event_handler_instance_t got_ip_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &WifiHandler::ipEventHandler,
                                                        NULL,
                                                        &got_ip_event_instance));

    /** START THE WIFI DRIVER **/
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "SeklawyWiFi",
            .password = "Swanjs21",
	     .threshold = {.authmode = WIFI_AUTH_WPA2_PSK},
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    // set the wifi controller to be a station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );

    // set the wifi config
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );

    // start the wifi driver
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(LOG_TAG, "STA initialization complete");

    /** NOW WE WAIT **/
    EventBits_t bits = xEventGroupWaitBits(m_wifiEventGroup,
            WIFI_SUCCESS | WIFI_FAILURE,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_SUCCESS) {
        ESP_LOGI(LOG_TAG, "Connected to ap");
        status = WIFI_SUCCESS;
    } else if (bits & WIFI_FAILURE) {
        ESP_LOGI(LOG_TAG, "Failed to connect to ap");
        status = WIFI_FAILURE;
    } else {
        ESP_LOGE(LOG_TAG, "UNEXPECTED EVENT");
        status = WIFI_FAILURE;
    }

    /* The event will not be processed after unregister */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, got_ip_event_instance));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler_event_instance));
    vEventGroupDelete(m_wifiEventGroup);

    return status;
}



void WifiHandler::wifiEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
	{
		ESP_LOGI(LOG_TAG, "Connecting to AP...");
		esp_wifi_connect();
	} 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		if (m_retryCount < MAX_FAILURES)
		{
			ESP_LOGI(LOG_TAG, "Reconnecting to AP...");
			esp_wifi_connect();
			m_retryCount++;
		} 
        else 
        {
			xEventGroupSetBits(m_wifiEventGroup, WIFI_FAILURE);
		}
	}

}



void WifiHandler::ipEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	{
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(LOG_TAG, "STA IP: " IPSTR, IP2STR(&event->ip_info.ip));
        m_retryCount = 0;
        xEventGroupSetBits(m_wifiEventGroup, WIFI_SUCCESS);
    }
}
