#include <QApplication>
#include "window.h"
#ifdef TCP
#include "tcpservice.h"
#else
#include "serialportservice.h"
#endif

int main(int argc, char *argv[])
{
#ifdef TCP
    TCPService comserv;
#else
    SerialPortService comserv;
#endif

    // Initialize the Qt application with command-line arguments
    QApplication app(argc, argv);

    // Create the main window of the application and pass a pointer to the TCPService instance
    // This allows the window to interact with the communication service
    Window window(&comserv);

    // Display the main window on the screen
    window.show();

    // Start the Qt event loop, which will keep the application running and responsive
    // The event loop processes user interactions, updates the GUI, and handles events
    return app.exec();
}
