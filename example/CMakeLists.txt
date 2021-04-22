cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)


project(project1
        LANGUAGES ASM C CXX
        VERSION 1.0
        DESCRIPTION "Example"
)


# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)


set(FILE_ELF ${CMAKE_PROJECT_NAME}.elf)
set(FILE_HEX ${CMAKE_PROJECT_NAME}.hex)


# Initialize the SDK
pico_sdk_init()


add_executable(main
    example.cpp
)

target_sources(main
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../SSD1306.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../GFX.cpp
)

target_include_directories(main
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../
)


target_link_libraries(main
  pico_stdlib
  hardware_i2c
)

pico_add_extra_outputs(main)
