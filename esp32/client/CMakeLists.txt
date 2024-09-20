# Build ESP32 client
add_custom_target(
	client_esp
	ALL
	COMMAND platformio run
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/esp32/client"
	COMMENT "Building PlatformIO Client"
	VERBATIM
)

# Upload ESP32 client
add_custom_target(
	client_upload
	COMMAND platformio run --target upload
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/esp32/client"
	COMMENT "Uploading Client firmware to microcontroller"
	VERBATIM
)
