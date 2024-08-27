#pragma once

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10




class WifiHandler
{
public:

    WifiHandler();

    esp_err_t connectWifi();



private:


    static void wifiEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

    static void ipEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

    static EventGroupHandle_t m_wifiEventGroup;
    static int m_retryCount;






};