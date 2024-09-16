#ifndef CANVAS_H
#define CANVAS_H

#include "comservice.h"

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QWidget>

namespace speedometer
{
constexpr QRect arc_rectangle(70, 50, 600, 600); // rectangle defining the boundary for the arc
constexpr int start_angle = 330 * 16;            // starting angle of the arc in 1/16th of a degree
constexpr int span_angle = 240 * 16;             // span angle of the arc in 1/16th of a degree

namespace tick
{
constexpr int small_width = 2;
constexpr int medium_width = 4;
constexpr int large_width = 6;
constexpr int start = 250;
constexpr int small_end = 270;
constexpr int medium_end = 275;
constexpr int large_end = 280;
constexpr int label = 21;
constexpr int label_offset_x = -15;
constexpr int label_offset_y = 9;
constexpr int number_of_large_ticks = 13;
constexpr int total_ticks = (number_of_large_ticks - 1) * 4;
constexpr int angle_increment = 240 / total_ticks;
constexpr int initial_angle = 330;
constexpr int large_interval = 4;
constexpr int medium_interval = 2;
} // namespace tick

namespace icons
{

struct Icon
{
    const QPoint position;
    const QChar icon;
    const QFont font;
};

struct IconWithColoredStates : Icon
{
    const int low, high;
    const QColor lowc, medc, highc;
    const QPoint label;
    const QFont label_font;
};

const QString icon_font("MaterialIcons");
const QString text_font("Helvetica");
const Icon left_turn{QPoint(50, 75), QChar(0xe5c4), QFont(icon_font, 50)};
const Icon right_turn{QPoint(600, 75), QChar(0xe5c8), QFont(icon_font, 50)};
const Icon connection_ok{QPoint(345, 430), QChar(0xe9e4), QFont(icon_font, 40)};
const Icon connection_error{QPoint(345, 430), QChar(0xe628), QFont(icon_font, 40)};
constexpr QPoint speed_label(345, 450);
constexpr QPoint errror_label(300, 450);
const IconWithColoredStates battery{{QPoint(675, 380), QChar(0xebdc), QFont(icon_font, 100)},
                                    25,
                                    50,
                                    Qt::red,
                                    Qt::yellow,
                                    Qt::green,
                                    QPoint(730, 385),
                                    QFont(text_font, 10)};
constexpr QPoint battery_level(725, 360);
constexpr QSize battery_level_size(35, -80);
const IconWithColoredStates temperature{{QPoint(710, 480), QChar(0xe1ff), QFont(icon_font, 50)},
                                        5,
                                        40,
                                        Qt::white,
                                        Qt::blue,
                                        Qt::red,
                                        QPoint(730, 490),
                                        QFont(text_font, 10)};

} // namespace icons

namespace needle
{
constexpr int length = 220;
constexpr int offset = 210;
constexpr QPoint attach_point = arc_rectangle.center();
constexpr int attach_point_size = 18; // radius of the central ellipse in pixels
} // namespace needle
} // namespace speedometer

class Canvas : public QWidget
{
public:
    explicit Canvas(COMService *comserv, QWidget *parent = nullptr);
    bool is_visible = true;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    COMService *comserv;
    QColor backgroundColor;

    void paint_battery(QPainter &painter);
    void paint_turn_signals(QPainter &painter);
    void paint_ticks(QPainter &painter, QPen &pen);
    void paint_temperature(QPainter &painter);
    QColor determine_color(const speedometer::icons::IconWithColoredStates &icon, int value);
    void paint_icon(QPainter &painter, const speedometer::icons::Icon &icon);
};

#endif // CANVAS_H
