#include "app_button.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "esp_timer.h"
#include "esp_log.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ADC Channels
#define ADC1_EXAMPLE_CHAN0 ADC_CHANNEL_0
// ADC Attenuation
#define ADC_EXAMPLE_ATTEN ADC_ATTEN_DB_11
// ADC Calibration
#if CONFIG_IDF_TARGET_ESP32
#define ADC_EXAMPLE_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_VREF
#elif CONFIG_IDF_TARGET_ESP32S2
#define ADC_EXAMPLE_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32C3
#define ADC_EXAMPLE_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP
#elif CONFIG_IDF_TARGET_ESP32S3
#define ADC_EXAMPLE_CALI_SCHEME ESP_ADC_CAL_VAL_EFUSE_TP_FIT
#endif

static adc_oneshot_unit_handle_t adc1_handle = NULL;

#define PRESS_INTERVAL 500000

static const char *TAG = "App/Button";

AppButton::AppButton() : key_configs({{BUTTON_MENU, 2800, 3000}, {BUTTON_PLAY, 2250, 2450}, {BUTTON_UP, 300, 500}, {BUTTON_DOWN, 850, 1050}}),
                         pressed(BUTTON_IDLE), menu(0)
{
    if (adc1_handle){
        ESP_LOGE(TAG, "Button adc has been initialized");
    }

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    // ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    // ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_EXAMPLE_CHAN0, &config));
}

static void task(AppButton *self)
{
    int64_t backup_time = esp_timer_get_time();
    int64_t last_time = esp_timer_get_time();

    uint8_t menu_count = 0;

    while (true)
    {
        // int voltage = 0;
        // ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_EXAMPLE_CHAN0, &voltage));
        // backup_time = esp_timer_get_time();
        // for (auto key_config : self->key_configs)
        // {
        //     if ((voltage >= key_config.min) && (voltage <= key_config.max))
        //     {
        //         if (((backup_time - last_time) > PRESS_INTERVAL))
        //         {
        //             self->pressed = key_config.key;
        //             ESP_LOGI(TAG, "Button[%d] is clicked", self->pressed);

        //             if (self->pressed == BUTTON_MENU)
        //             {
        //                 self->menu++;
        //                 self->menu %= (MENU_MOTION_DETECTION + 1);
        //             }

        //             last_time = backup_time;
        //             self->notify();

        //             self->pressed = BUTTON_IDLE;
        //             break;
        //         }
        //     }
        // }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void AppButton::run()
{
    xTaskCreatePinnedToCore((TaskFunction_t)task, TAG, 3 * 1024, this, 5, NULL, 0);
}
