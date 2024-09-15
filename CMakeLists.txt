cmake_minimum_required(VERSION 3.22)
project(
  av23tr
  VERSION 1.0
  DESCRIPTION "Project"
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
# Toggle ON/OFF to switch between CAN and TCP/IP
option(USE_CAN "Use the CAN protocol to communicate." OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core SerialPort Multimedia)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (USE_CAN)
  add_executable(
    client
    desktop/client/main.cpp
    desktop/client/src/window.cpp
    desktop/client/src/canvas.cpp
    desktop/client/src/serialportservice.cpp
    desktop/client/src/comservice.cpp)
  target_include_directories(client PUBLIC desktop/client/include shared)
  target_link_libraries(client Qt6::Core Qt6::Widgets Qt6::SerialPort Qt6::Multimedia)

  add_executable(
    server
    desktop/server/main.cpp
    desktop/server/src/window.cpp
    desktop/server/src/serialportservice.cpp
    desktop/server/src/comservice.cpp)
  target_include_directories(server PUBLIC desktop/server/include shared)
  target_link_libraries(server Qt6::Core Qt6::Widgets Qt6::SerialPort Qt6::Multimedia)
 # Add custom targets for PlatformIO (build and upload)
  # Client Microcontroller (ESP32)
add_custom_target(pio_client_build ALL
    COMMAND platformio run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/esp32/client
    COMMENT "Building PlatformIO Client project"
    VERBATIM)
add_custom_target(pio_client_upload
    COMMAND platformio run --target upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/esp32/client
    COMMENT "Uploading Client firmware to microcontroller"
    VERBATIM)
# Server Microcontroller (ESP32)
add_custom_target(pio_server_build ALL
    COMMAND platformio run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/esp32/server
    COMMENT "Building PlatformIO Server project"
    VERBATIM)
add_custom_target(pio_server_upload
    COMMAND platformio run --target upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/esp32/server/
    COMMENT "Uploading Server firmware to microcontroller"
    VERBATIM)
    
add_dependencies(pio_client_upload pio_client_build)
add_dependencies(pio_server_upload pio_server_build)
else()
  add_executable(
    client
    desktop/client/main.cpp
    desktop/client/src/window.cpp
    desktop/client/src/tcpservice.cpp
    desktop/client/src/canvas.cpp
    desktop/client/src/comservice.cpp)
  target_include_directories(client PUBLIC desktop/client/include shared)
  target_link_libraries(client Qt6::Core Qt6::Widgets Qt6::Multimedia)
  target_compile_definitions(client PUBLIC TCP)
  add_executable(
    server
    desktop/server/main.cpp
    desktop/server/src/window.cpp
    desktop/server/src/tcpservice.cpp
    desktop/server/src/comservice.cpp)
  target_include_directories(server PUBLIC desktop/server/include shared)
  target_link_libraries(server Qt6::Core Qt6::Widgets Qt6::Multimedia)
  target_compile_definitions(server PUBLIC TCP)
endif()

add_custom_target(use_can
  COMMAND ${CMAKE_COMMAND} -DUSE_CAN=ON ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
  COMMENT "Switching to CAN"
)

add_custom_target(use_tcp
  COMMAND ${CMAKE_COMMAND} -DUSE_CAN=OFF ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
  COMMENT "Switching to TCP" 
)

function(find_and_install_font FONT_NAME SOURCE_DIR FONT_VARIABLE)
  if(WIN32)
    set(FONT_DIRS "C:/Windows/Fonts")
    set(INSTALL_DIR "C:/Windows/Fonts")
  elseif(APPLE)
    set(FONT_DIRS "/Library/Fonts" "~/Library/Fonts" "/System/Library/Fonts"
                  "/Network/Library/Fonts")
    set(INSTALL_DIR "/Library/Fonts")
  elseif(UNIX)
    set(FONT_DIRS "/usr/share/fonts" "~/.fonts" "/usr/local/share/fonts"
                  "/usr/share/fonts/truetype")
    set(INSTALL_DIR "$ENV{HOME}/.local/share/fonts")
  endif()
  find_file(
    FONT_PATH ${FONT_NAME}
    PATHS ${FONT_DIRS}
    NO_DEFAULT_PATH)
  if(FONT_PATH)
    set(${FONT_VARIABLE}
        ${FONT_PATH}
        PARENT_SCOPE)
  else()
    set(FONT_SOURCE_PATH "${SOURCE_DIR}/${FONT_NAME}")
    if(EXISTS ${FONT_SOURCE_PATH})
      message(
        STATUS
          "Font not found. Installing from ${FONT_SOURCE_PATH} to ${INSTALL_DIR}"
      )
      file(COPY ${FONT_SOURCE_PATH} DESTINATION ${INSTALL_DIR})
      find_file(
        FONT_PATH ${FONT_NAME}
        PATHS ${INSTALL_DIR}
        NO_DEFAULT_PATH)
      if(FONT_PATH)
        set(${FONT_VARIABLE}
            ${FONT_PATH}
            PARENT_SCOPE)
        message(STATUS "Font installed at: ${FONT_PATH}")
      else()
        message(FATAL_ERROR "Failed to install font: ${FONT_NAME}")
      endif()
    else()
      message(
        FATAL_ERROR
          "Font not found and source file does not exist: ${FONT_SOURCE_PATH}")
    endif()
  endif()
endfunction()
# Define the font you are looking for and the source directory
set(FONT_NAME "MaterialIcons.ttf")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/desktop/client/res")
# Call the find_and_install_font function
find_and_install_font(${FONT_NAME} ${SOURCE_DIR} FONT_PATH)
# Check if the font was found or installed
if(FONT_PATH)
  message(STATUS "Font available at: ${FONT_PATH}")
else()
  message(FATAL_ERROR "Font not available: ${FONT_NAME}")
endif()
