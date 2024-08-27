set(SOURCES ./src/main.cpp
            ./src/GpioBase.cpp
            ./src/GpioOutput.cpp
            ./src/WifiHandler.cpp
            ./src/TcpHandler.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common driver esp_wifi nvs_flash)