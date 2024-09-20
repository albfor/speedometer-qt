# Build ESP32 server
add_custom_target(
	server_esp
	ALL
	COMMAND platformio run
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/esp32/server"
	COMMENT "Building PlatformIO Server"
	VERBATIM
)

# Upload ESP32 server
add_custom_target(
	server_upload
	COMMAND platformio run --target upload
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/esp32/server/"
	COMMENT "Uploading Server firmware to microcontroller"
	VERBATIM
)
