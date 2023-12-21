#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "ota.h"
#include "device.h"
#include "wifi.h"

static const char *TAG = "main";

void app_main(void)
{
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGE(TAG, "Failed to initialize nvs flash");
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    device_init();

    xTaskCreate(ota_task, "ota_task", 8192, NULL, 12, NULL);

    initialize_wifi();
}
