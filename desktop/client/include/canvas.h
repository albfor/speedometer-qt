#ifndef CANVAS_H
#define CANVAS_H

#include "comservice.h"

#include <QColor>
#include <QPainter>
#include <QWidget>

// Arc rectangle dimensions and angles used for drawing an arc
constexpr QRect ARC_RECTANGLE(70, 50, 600, 600); // Rectangle defining the boundary for the arc
constexpr int START_ANGLE = 330 * 16;            // Starting angle of the arc in 1/16th of a degree
constexpr int SPAN_ANGLE = 240 * 16;             // Span angle of the arc in 1/16th of a degree

// Tick marks on the arc
constexpr int NUM_LARGE_TICKS = 13;                    // Number of large tick marks (major divisions) on the arc
constexpr int TOTAL_TICKS = (NUM_LARGE_TICKS - 1) * 4; // Total number of ticks, including smaller ones
constexpr int ANGLE_INCREMENT = 240 / TOTAL_TICKS;     // Angle between each tick mark

// Initial angle and tick mark lengths
constexpr int INITIAL_ANGLE = 330; // Starting angle for the first tick mark in degrees

// Text positioning relative to tick marks
constexpr int LABEL_TO_TICK_DISTANCE = 21;  // Offset distance for the text from the tick marks in pixels
constexpr int LABEL_TO_TICK_OFFSET_X = -15; // X-axis offset for text positioning relative to tick marks
constexpr int LABEL_TO_TICK_OFFSET_Y = 9;   // Y-axis offset for text positioning relative to tick marks
constexpr int NEEDLE_LENGTH = 220;          // Length of the needle (pointer) in pixels

// Needle value and offset
constexpr int NEEDLE_VALUE = 240 - 50; // Value that determines where the needle points (subtracting 50 to adjust)
constexpr int NEEDLE_OFFSET = 210;     // Offset value used in needle positioning

// Magic numbers turned into constants for clarity
constexpr int ELLIPSE_RADIUS = 18; // Radius of the central ellipse in pixels
const QPoint SPEED_LABEL_POSITION(345, 450);
const QPoint ERROR_LABEL_POSITION(300, 450);

// Battery
const QPoint BATTERY_LEVEL_POSITION(725, 360);
const QSize BATTERY_LEVEL_SIZE(35, -80);

const QFont HELVETICA_FONT_14("Helvetica", 14);
const QFont HELVETICA_FONT_22("Helvetica", 22);
const QFont HELVETICA_FONT_10("Helvetica", 10);

// Declaration of the Canvas class, which inherits from QWidget
class Canvas : public QWidget
{

public:
    // Constructor declaration for Canvas class
    explicit Canvas(COMService *comserv, QWidget *parent = nullptr);
    bool is_visible = true;

protected:
    // Override of the paintEvent function to handle custom painting
    void paintEvent(QPaintEvent *event) override;

private:
    struct Icon
    {
        const QPoint position;
        const QChar icon;
        const QFont font;
    };

    struct IconWithColoredStates : Icon
    {
        int low, high;
        QColor lowc, medc, highc;
        QPoint label;
    };

    struct TickSettings
    {
        int small_width, small_len, medium_width, medium_len, large_width, large_len;
    };

    const QString icon_font = "MaterialIcons";
    const IconWithColoredStates battery{{QPoint(675, 380), QChar(0xebdc), QFont(icon_font, 100)},
                                        25,
                                        50,
                                        Qt::red,
                                        Qt::yellow,
                                        Qt::green,
                                        QPoint(730, 385)};
    const IconWithColoredStates temperature{
        {QPoint(710, 480), QChar(0xe1ff), QFont(icon_font, 50)}, 5, 40, Qt::white, Qt::blue, Qt::red, QPoint(730, 490)};
    const Icon left_turn_signal{QPoint(50, 75), QChar(0xe5c4), QFont(icon_font, 50)};
    const Icon right_turn_signal{QPoint(600, 75), QChar(0xe5c8), QFont(icon_font, 50)};

    const Icon connection_ok{QPoint(345, 430), QChar(0xe9e4), QFont(icon_font, 40)};
    const Icon connection_error{QPoint(345, 430), QChar(0xe628), QFont(icon_font, 40)};

    static constexpr TickSettings tick_settings{2, 270, 4, 275, 6, 280};
    static constexpr int tick_to_center_distance = 250;

    COMService *comserv;
    QColor backgroundColor;

    void paint_battery(QPainter &painter);
    void paint_turn_signals(QPainter &painter);
    void paint_ticks(QPainter &painter, QPen &pen);
    void paint_temperature(QPainter &painter);
    QColor determine_color(IconWithColoredStates icon, int value);
    void manage_blinker_sound();
    void paint_icon(QPainter &painter, const Icon &icon);
};

#endif // CANVAS_H
