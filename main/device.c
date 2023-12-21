#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>
#include <hal/adc_types.h>
#include <soc/adc_channel.h>
#include <math.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <esp_mac.h>
#include <esp_system.h>
#include <esp_ota_ops.h>

#include "device.h"

struct device_state state = {
    .mac = "unknown",
    .version = "unknown",
    .network_connected = false
};

static const char *TAG = "device";

struct device_state* get_device_state(void) 
{
    return &state;
}

void device_network_connected(void) 
{
    state.network_connected = true;
} 

void device_network_disconnected(void) 
{
    state.network_connected = false;
} 

void device_init(void)
{
    ESP_LOGI(TAG, "Device configuration");

    uint8_t base_mac_addr[6];

    const esp_app_desc_t *app_desc = esp_app_get_description();
    esp_err_t ret = esp_read_mac(base_mac_addr, ESP_MAC_BASE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK0. (%s)", esp_err_to_name(ret));
        ESP_LOGE(TAG, "Aborting");
        abort();
    } else {
        ESP_LOGI(TAG, "Base MAC Address read from EFUSE BLK0");
    }

	size_t mac_len = snprintf(NULL, 0, "%02x:%02x:%02x:%02x:%02x:%02x", 
        base_mac_addr[0], 
        base_mac_addr[1], 
        base_mac_addr[2], 
        base_mac_addr[3], 
        base_mac_addr[4], 
        base_mac_addr[5]);

	char *mac = malloc(mac_len + 1);

    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
        base_mac_addr[0], 
        base_mac_addr[1], 
        base_mac_addr[2], 
        base_mac_addr[3], 
        base_mac_addr[4], 
        base_mac_addr[5]); 

    state.mac = mac;
    state.version = (char *) app_desc->version;
}