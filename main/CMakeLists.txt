idf_component_register(
    SRCS "main.c" "device.c" "ota.c" "wifi.c"
    INCLUDE_DIRS "include"
    EMBED_TXTFILES ${project_dir}/certs/ca_cert.pem)
