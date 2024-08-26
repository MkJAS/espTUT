set(SOURCES ./src/main.cpp
            ./src/GpioBase.cpp
            ./src/GpioOutput.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common driver)