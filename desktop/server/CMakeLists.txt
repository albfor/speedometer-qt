if (USE_CAN)
	add_executable(
		server
		main.cpp
		src/window.cpp
		src/serialportservice.cpp
		src/comservice.cpp
	)

	target_link_libraries(
		server
		Qt6::SerialPort
	)
else()
	add_executable(
		server
		main.cpp
		src/window.cpp
		src/comservice.cpp
		src/tcpservice.cpp
	)

	target_compile_definitions(
		server
		PUBLIC
		TCP
	)
endif()

target_include_directories(
	server
	PUBLIC
	include
	"${PROJECT_SOURCE_DIR}/shared"
)

target_link_libraries(
	server
	Qt6::Core
	Qt6::Widgets
	Qt6::Multimedia
)
