cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)



project(ATIVIDADE-02 C CXX ASM) 
pico_sdk_init()




add_executable(${PROJECT_NAME}  
        ATIVIDADE-02.c # Código principal em C
        libs/ssd1306.c # Código do display
        libs/definir_cores.c # Código de função que define as cores dos resistores
        )

target_link_libraries(${PROJECT_NAME} 
        pico_stdlib
        hardware_i2c
        hardware_adc
        hardware_pio
        hardware_clocks
        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_generate_pio_header(ATIVIDADE-02 ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)
pico_add_extra_outputs(${PROJECT_NAME})
