#include "window.h"
#include "setting.h"
#include <QTimer>

Window::Window(COMService *comserv, QWidget *parent)
    : QDialog(parent), comserv(comserv), canvas(comserv, this), timer(this), BlinkTimer(this)
{
    setWindowTitle("Client");
    setFixedSize(width_, height_);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    canvas.setGeometry(0, 0, width_, height_);

    // Create a QTimer to trigger updates periodically
    connect(&timer, SIGNAL(timeout()), &canvas, SLOT(update()));

    connect(&BlinkTimer, &QTimer::timeout, [this]() {
        canvas.isIconGreen = !canvas.isIconGreen; // Toggle icon color state
        canvas.update();                          // Trigger a repaint
    });

    BlinkTimer.start(350); // Blink interval (350 ms)

    timer.start(Setting::INTERVAL_MS);
}
