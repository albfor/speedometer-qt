qt_add_resources(RESOURCES res/resources.qrc)

if (USE_CAN)
	# Build for UART/CAN
	add_executable(
		client
		main.cpp
		src/window.cpp
		src/canvas.cpp
		src/comservice.cpp
		src/serialportservice.cpp
	)
	target_link_libraries(
		client
		Qt6::SerialPort
	)
else()
	# Build for TCP
	add_executable(
		client
		main.cpp
		src/window.cpp
		src/canvas.cpp
		src/comservice.cpp
		src/tcpservice.cpp
	)

	target_compile_definitions(
		client
		PRIVATE
		TCP
	)
endif()

target_include_directories(
	client
	PUBLIC
	include
	"${PROJECT_SOURCE_DIR}/shared"
)

target_link_libraries(
	client
	Qt6::Core
	Qt6::Widgets
	Qt6::Multimedia
)

target_sources(
	client 
	PRIVATE 
	${RESOURCES}
)
