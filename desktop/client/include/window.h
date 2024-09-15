#ifndef WINDOW_H
#define WINDOW_H

// Include the necessary Qt header for using QDialog
#include <QDialog>
// Include headers for COMService and Canvas classes
#include "canvas.h"
#include "comservice.h"
#include <QTimer>

class Window : public QDialog
{
public:
    // Constructor to initialize the Window class, taking a COMService pointer and an optional parent widget
    explicit Window(COMService *comserv, QWidget *parent = nullptr);

private:
    COMService *comserv; // Pointer to the communication service for interacting with external systems
    Canvas canvas;       // Pointer to the Canvas instance used for custom drawing
    QTimer timer;
    QTimer BlinkTimer;
    constexpr static int width_ = 800;
    constexpr static int height_ = 560;
};

#endif // WINDOW_H
