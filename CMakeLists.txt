cmake_minimum_required(VERSION 3.22)

project(
    speedometer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Toggle ON/OFF to switch between CAN and TCP/IP
option(USE_CAN "Use the CAN protocol to communicate." OFF)

find_package(
    Qt6
    REQUIRED COMPONENTS
    Widgets
    Core
    SerialPort
    Multimedia
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Desktop projects
add_subdirectory(desktop/client)
add_subdirectory(desktop/server)

if (USE_CAN)
    # ESP32 projects
    add_subdirectory(esp32/client)
    add_subdirectory(esp32/server)
endif()

# Switch to UART/CAN communication
add_custom_target(
    use_can
    COMMAND ${CMAKE_COMMAND} -DUSE_CAN=ON ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build .
    COMMENT "Switching to CAN"
)

# Switch to TCP communication
add_custom_target(
    use_tcp
    COMMAND ${CMAKE_COMMAND} -DUSE_CAN=OFF ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build .
    COMMENT "Switching to TCP" 
)

# Ensure MaterialsIcon font is installed
function(find_and_install_font FONT_NAME SOURCE_DIR FONT_VARIABLE)
    set(
        FONT_DIRS
        "/usr/share/fonts"
        "~/.fonts"
        "/usr/local/share/fonts"
        "/usr/share/fonts/truetype"
    )

    set(INSTALL_DIR "$ENV{HOME}/.local/share/fonts")

    find_file(
        FONT_PATH ${FONT_NAME}
        PATHS ${FONT_DIRS}
        NO_DEFAULT_PATH
    )

    if(FONT_PATH)
        set(
            ${FONT_VARIABLE}
            ${FONT_PATH}
            PARENT_SCOPE
        )
    else()
        set(FONT_SOURCE_PATH "${SOURCE_DIR}/${FONT_NAME}")

        if(EXISTS ${FONT_SOURCE_PATH})
            message(STATUS "Font not found. Installing from ${FONT_SOURCE_PATH} to ${INSTALL_DIR}")

            file(COPY ${FONT_SOURCE_PATH} DESTINATION ${INSTALL_DIR})

            find_file(
                FONT_PATH ${FONT_NAME}
                PATHS ${INSTALL_DIR}
                NO_DEFAULT_PATH
            )

            if(FONT_PATH)
                set( ${FONT_VARIABLE} ${FONT_PATH} PARENT_SCOPE)

                message(STATUS "Font installed at: ${FONT_PATH}")
            else()
                message(FATAL_ERROR "Failed to install font: ${FONT_NAME}")
            endif()
        else()
            message(FATAL_ERROR "Font not found and source file does not exist: ${FONT_SOURCE_PATH}")
        endif()
    endif()
endfunction()
set(FONT_NAME "MaterialIcons.ttf")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/desktop/client/res")
find_and_install_font(${FONT_NAME} ${SOURCE_DIR} FONT_PATH)
if(FONT_PATH)
    message(STATUS "Font available at: ${FONT_PATH}")
else()
    message(FATAL_ERROR "Font not available: ${FONT_NAME}")
endif()
