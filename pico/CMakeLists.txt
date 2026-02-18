cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
set(PICO_SDK_FETCH_FROM_GIT ON)
include(pico_sdk_import.cmake)

project(divisor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(PROJECT_SOURCES
    src/main.c
    src/gamepad/gamepad.c
    src/usb/usb_descriptors.c
    src/usb/usb_callbacks.c
    src/led/led.c
    src/usb/dinput.c
    src/usb/xinput.c
    src/gamepad/debounce.c
)

add_executable(divisor
    ${PROJECT_SOURCES}
)

# Make sure TinyUSB can find tusb_config.h + your headers
target_include_directories(divisor PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/gamepad
    ${CMAKE_CURRENT_LIST_DIR}/src/usb
    ${CMAKE_CURRENT_LIST_DIR}/src/led
)

target_link_libraries(divisor PUBLIC
    pico_stdlib
    pico_unique_id
    tinyusb_device
    tinyusb_board
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(divisor PUBLIC pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(divisor)
