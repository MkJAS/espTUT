#include "esp_log.h"
#define LOG_TAG "I2S"

#include "Mic.h"

Mic::Mic()
{
    m_i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 11025, // or 44100 if you like
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // Ground the L/R pin on the INMP441.
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0,
    };

    m_i2sPins = {
        .bck_io_num = 12,   // Bit Clock.
        .ws_io_num = 11,    // Word Select aka left/right clock aka LRCL.
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = 10,  // Data-out of the mic. 
};

}

void Mic::init()
{
    if (ESP_OK != i2s_driver_install(I2S_NUM_0, &m_i2sConfig, 0, NULL)) 
    {
        ESP_LOGI(LOG_TAG, "i2s_driver_install: error");
    }

    if (ESP_OK != i2s_set_pin(I2S_NUM_0, &m_i2sPins)) 
    {
        ESP_LOGI(LOG_TAG,"i2s_set_pin: error");
    }
}
