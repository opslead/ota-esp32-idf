#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_http_client.h>
#include <esp_https_ota.h>
#include <string.h>
#include <esp_mac.h>

#include "ota.h"
#include "device.h"

#define FIRMWARE_UPGRADE_URL "https://ota.yourdomain.com/v1/ota/6566a5ef-b8b6-4243-8781-5e91384eb371"
#define FIRMWARE_UPGRADE_CHECK_INTERVAL 3600000

static const char *TAG = "ota";

extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

esp_err_t ota_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }

    return ESP_OK;
}

void ota_task(void *pvParameter)
{
    ESP_LOGI(TAG, "start ota_task");

    struct device_state *state = get_device_state();

    size_t url_len = snprintf(
        NULL, 0, "%s/%s/%s", FIRMWARE_UPGRADE_URL, state->mac, state->version);

	char *url = malloc(url_len + 1);

    sprintf(url, "%s/%s/%s", FIRMWARE_UPGRADE_URL, state->mac, state->version);

    while (1) {
        if (!state->network_connected) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        
        esp_http_client_config_t config = {
            .url = url,
            .cert_pem = (char *)server_cert_pem_start,
            .event_handler = ota_event_handler,
            .keep_alive_enable = true,
            .skip_cert_common_name_check = false,
            .timeout_ms = 6000
        };

        esp_https_ota_config_t ota_config = {
            .http_config = &config
        };

        ESP_LOGI(TAG, "Attempting to download update from %s", config.url);

        esp_err_t ret = esp_https_ota(&ota_config);
        
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
            esp_restart();
        } else {
            ESP_LOGE(TAG, "Firmware upgrade failed");
        }

        vTaskDelay(FIRMWARE_UPGRADE_CHECK_INTERVAL / portTICK_PERIOD_MS);
    }
}
