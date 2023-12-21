#pragma once

struct device_state {
    char *mac;
    char *version;
    
    bool network_connected;

    // Other state data
};

struct device_state* get_device_state(void);

void device_init(void);
void device_network_connected(void);
void device_network_disconnected(void);
