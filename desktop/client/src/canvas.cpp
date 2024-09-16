#include "canvas.h"
#include "setting.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

Canvas::Canvas(COMService *comserv, QWidget *parent) : QWidget(parent), comserv(comserv), backgroundColor(Qt::black)
{
}

const QString Canvas::text_font("Helvetica");
const QString Canvas::icon_font("MaterialIcons");
const QFont Canvas::IconWithColoredStates::label_font(text_font, 10);

// Override of the paintEvent method to perform custom painting
void Canvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), backgroundColor); // set background

    // Create a QPen object to define the style of the lines
    QPen pen(Qt::white, 10, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(pen);

    // Draw an arc using the defined pen and constants
    painter.drawArc(ARC_RECTANGLE, START_ANGLE, SPAN_ANGLE);

    // Calculate the center of the arc rectangle
    QPoint center = ARC_RECTANGLE.center();

    // Set brush color to red and draw a circle at the center of the arc
    painter.setBrush(Qt::red);
    painter.drawEllipse(center, ELLIPSE_RADIUS, ELLIPSE_RADIUS);

    paint_ticks(painter, pen);
    paint_temperature(painter);
    paint_battery(painter);

    // Draw connection status icon and text based on the connection status
    auto paint_connection = [&painter, this](Icon icon, QString message, QPoint label_position) {
        paint_icon(painter, icon);
        painter.setFont(HELVETICA_FONT_14);
        painter.drawText(label_position, message);
    };
    if (comserv->is_connected())
        paint_connection(connection_ok, QString::number(comserv->get_speed()) + " km/h", SPEED_LABEL_POSITION);
    else
        paint_connection(connection_error, "Connection Error!", ERROR_LABEL_POSITION);

    // Draw the needle
    painter.setFont(HELVETICA_FONT_22);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(10);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    qreal needle_radians = qDegreesToRadians(NEEDLE_OFFSET - comserv->get_speed());
    painter.drawLine(center, QPointF(center.x() + NEEDLE_LENGTH * qCos(needle_radians),
                                     center.y() - NEEDLE_LENGTH * qSin(needle_radians)));

    paint_turn_signals(painter);
}

void Canvas::paint_turn_signals(QPainter &painter)
{
    if (comserv->is_connected())
    {
        QColor left, right;
        QColor visible(Qt::green);

        if (!is_visible)
        {
            left = backgroundColor;
            right = backgroundColor;
        }
        else
        {
            Setting::turn_signal_state signal = comserv->get_turn_signal_state();
            switch (signal)
            {
            case Setting::LEFT:
                left = visible;
                break;
            case Setting::RIGHT:
                right = visible;
                break;
            case Setting::WARNING:
                left = visible;
                right = visible;
                break;
            case Setting::OFF:
            default:
                left = backgroundColor;
                right = backgroundColor;
            }
        }

        // draw the icons
        painter.setPen(left);
        paint_icon(painter, left_turn_signal);
        painter.setPen(right);
        paint_icon(painter, right_turn_signal);
    }
}

void Canvas::paint_ticks(QPainter &painter, QPen &pen)
{
    for (int i = 0; i <= TOTAL_TICKS; i++)
    {
        // Calculate the angle for the tick mark
        int angle = INITIAL_ANGLE + i * ANGLE_INCREMENT;
        qreal radians = qDegreesToRadians(angle); // Convert angle to radians
        qreal cosRadians = qCos(radians);
        qreal sinRadians = qSin(radians);
        QPoint center = ARC_RECTANGLE.center();
        QPoint tick_start(center.x() + tick_to_center_distance * cosRadians,
                          center.y() - tick_to_center_distance * sinRadians);
        QPoint tick_end;

        auto set_tick_width_and_end = [&pen, &tick_end, &center, cosRadians, sinRadians](int width, int length) {
            pen.setWidth(width);
            tick_end.setX(center.x() + length * cosRadians);
            tick_end.setY(center.y() - length * sinRadians);
        };

        if (i % 4 == 0) // large ticks
        {
            set_tick_width_and_end(tick_settings.large_width, tick_settings.large_len);

            // Calculate and display the speed label
            int speed_label = (TOTAL_TICKS - i) / 4 * 20;
            QPoint speed_label_position(tick_start.x() - LABEL_TO_TICK_DISTANCE * cosRadians + LABEL_TO_TICK_OFFSET_X,
                                        tick_start.y() + LABEL_TO_TICK_DISTANCE * sinRadians + LABEL_TO_TICK_OFFSET_Y);
            painter.drawText(speed_label_position, QString::number(speed_label));
        }
        else if (i % 4 == 2) // medium ticks
        {
            set_tick_width_and_end(tick_settings.medium_width, tick_settings.medium_len);
        }
        else // small ticks
        {
            set_tick_width_and_end(tick_settings.small_width, tick_settings.small_len);
        }
        painter.setPen(pen);
        painter.drawLine(tick_start, tick_end);
    }
}

void Canvas::paint_temperature(QPainter &painter)
{
    int temp = comserv->get_temperature();
    painter.setPen(determine_color(temperature, temp));
    paint_icon(painter, temperature);
    painter.setPen(Qt::white);
    painter.setFont(temperature.label_font);
    painter.drawText(temperature.label, QString("%1°C").arg(temp, 3));
}

void Canvas::paint_battery(QPainter &painter)
{
    unsigned battery_level = comserv->get_battery_level();
    QColor battery_color = determine_color(battery, battery_level);
    painter.setPen(battery_color);
    painter.setBrush(battery_color);
    paint_icon(painter, battery);
    QRect battery_level_bar(BATTERY_LEVEL_POSITION, BATTERY_LEVEL_SIZE); // bar inside icon showing percentage
    // map battery_level to gui
    battery_level_bar.setHeight((BATTERY_LEVEL_SIZE.height() * static_cast<int>(battery_level)) / 100);
    painter.drawRect(battery_level_bar);
    painter.setPen(Qt::white);
    painter.setFont(battery.label_font);
    painter.drawText(battery.label, QString("%1%").arg(battery_level, 3)); // 3 is max digits
}

void Canvas::paint_icon(QPainter &painter, const Icon &icon)
{
    painter.setFont(icon.font);
    painter.drawText(icon.position, icon.icon);
}

QColor Canvas::determine_color(IconWithColoredStates icon, int value)
{
    return value < icon.low ? icon.lowc : value < icon.high ? icon.medc : icon.highc;
}
