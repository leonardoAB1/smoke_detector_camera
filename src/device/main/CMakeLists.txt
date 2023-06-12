idf_component_register(SRCS
    src/main.c
    src/connect_wifi/connect_wifi.c
    src/base64/base64_utils.c
    src/camera/camera_utils.c
    src/http_handlers/http_handlers.c
    src/logging/logging_utils.c
    src/user_roles/user_roles.c
    src/web_server/web_server.c
    src/gpio_utils/gpio_utils.c
    src/http_handlers/http_handlers.c
    src/motor_control/motor_control.c
    INCLUDE_DIRS
    "src"
)

# Set the paths for the configuration and header files
set(CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../../config/wifi.config")
set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h")

# Read the SSID and password from the configuration file
file(STRINGS ${CONFIG_FILE} CONFIG_CONTENTS)

foreach(CONFIG_LINE ${CONFIG_CONTENTS})
    string(REGEX MATCH "^SSID=(.*)" SSID_MATCH ${CONFIG_LINE})
    string(REGEX MATCH "^PASSWORD=(.*)" PASSWORD_MATCH ${CONFIG_LINE})

    if (SSID_MATCH)
        string(REGEX REPLACE "^SSID=(.*)" "\\1" SSID ${CONFIG_LINE})
    elseif (PASSWORD_MATCH)
        string(REGEX REPLACE "^PASSWORD=(.*)" "\\1" PASSWORD ${CONFIG_LINE})
    endif()
endforeach()

# Generate the header file with preprocessor directives
file(WRITE ${HEADER_FILE} "#ifndef CONFIG_H_\n")
file(APPEND ${HEADER_FILE} "#define CONFIG_H_\n")
file(APPEND ${HEADER_FILE} "\n")
file(APPEND ${HEADER_FILE} "#define WIFI_SSID \"${SSID}\"\n")
file(APPEND ${HEADER_FILE} "#define WIFI_PASSWORD \"${PASSWORD}\"\n")
file(APPEND ${HEADER_FILE} "\n")
file(APPEND ${HEADER_FILE} "#endif\n")
