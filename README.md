# ESP32 Firmware with SmartConfig and OTA

This is example project skeleton with Wifi SmartConfig for network configuration and
OTA for firmware updates.

During startup `device_init()` initialize device state structure which is include device MAC address, firmware version
and network connection state. Then start `ota_task` that manage firmware updates.

Once device connected to network, will be called `device_network_connected()` and update device state

Project use custom partitions table to enable OTA, and include LetEncrypt CA cert. If you use different SSL certificate for your OTA service installation please update `certs/ce_certs.pem` with your vendor CA certificate. 

# Build project

To build project please run next command:
```bash
. ~/.idf/export.sh

idf.py build
```

If you want to specify firmware version, just set `VERSION` env variable

```bash
. ~/.idf/export.sh

VERSION="1.1.1"

idf.py build
```

# Contributing
We'd love for you to contribute to this project. You can request new features by creating an [issue](https://github.com/opslead/ota-esp32-idf/issues), or submit a [pull request](https://github.com/opslead/ota-esp32-idf/pulls) with your contribution.

# Issues
If you encountered a problem running this project, you can file an [issue](https://github.com/opslead/ota-esp32-idf/issues). For us to provide better support, be sure to include the following information in your issue:

- Host OS and version
- SDK version
- Device logs
