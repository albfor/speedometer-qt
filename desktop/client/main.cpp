#ifdef TCP
#include "tcpservice.h"
#else
#include "serialportservice.h"
#endif
#include "window.h"
#include <QApplication>
#include <comservice.h>
#include <tcpservice.h>

// Main function: entry point of the application
int main(int argc, char **argv)
{
    // Create a QApplication object, which is necessary for any Qt application
    QApplication app(argc, argv);
#ifdef TCP
    // Create an instance of TCPService, which manages communication with the server
    TCPService comserv;
#else
    SerialPortService comserv;
#endif

    // Create the main window and pass a pointer to the TCPService instance
    // This allows the Window class to use the communication service
    Window window(&comserv);

    // Show the main window on the screen
    window.show();

    // Execute the application’s event loop, keeping the application running
    return app.exec();
}
