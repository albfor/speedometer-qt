#include "canvas.h"
#include "setting.h"

Canvas::Canvas(COMService *comserv, QWidget *parent) : QWidget(parent), comserv(comserv), backgroundColor(Qt::black)
{
}

void Canvas::paintEvent(QPaintEvent *event)
{
    using namespace speedometer;
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), backgroundColor); // set background

    // Create a QPen object to define the style of the lines
    QPen pen(Qt::white, 10, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(pen);

    // outer edge speedometer
    painter.drawArc(arc_rectangle, start_angle, span_angle);

    // Set brush color to red and draw a circle at the center of the arc
    painter.setBrush(Qt::red);
    painter.drawEllipse(needle::attach_point, needle::attach_point_size, needle::attach_point_size);

    paint_ticks(painter, pen);
    paint_temperature(painter);
    paint_battery(painter);

    // paint connection status icon and text based on the connection status
    auto paint_connection = [&painter, this](icons::Icon icon, QString message, QPoint label_position) {
        paint_icon(painter, icon);
        painter.setFont(QFont(speedometer::icons::text_font, 14));
        painter.drawText(label_position, message);
    };
    if (comserv->is_connected())
        paint_connection(icons::connection_ok, QString("%1 km/h").arg(comserv->get_speed()), icons::speed_label);
    else
        paint_connection(icons::connection_error, "Connection Error!", icons::errror_label);

    // paint the needle
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(10);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    qreal needle_radians = qDegreesToRadians(needle::offset - comserv->get_speed());
    painter.drawLine(needle::attach_point, QPointF(needle::attach_point.x() + needle::length * qCos(needle_radians),
                                                   needle::attach_point.y() - needle::length * qSin(needle_radians)));

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
        paint_icon(painter, speedometer::icons::left_turn);
        painter.setPen(right);
        paint_icon(painter, speedometer::icons::right_turn);
    }
}

void Canvas::paint_ticks(QPainter &painter, QPen &pen)
{
    using namespace speedometer::tick;
    using speedometer::needle::attach_point;

    for (int tick = 0; tick <= total_ticks; tick++)
    {
        // calculate the angle for the tick mark
        int angle = initial_angle + tick * angle_increment;
        qreal radians = qDegreesToRadians(angle); // convert angle to radians
        qreal cos = qCos(radians);
        qreal sin = qSin(radians);
        QPoint tick_start(attach_point.x() + start * cos, attach_point.y() - start * sin);
        QPoint tick_end;

        auto set_tick_width_and_endpoint = [&pen, &tick_end, cos, sin](int width, int distance_from_center) {
            pen.setWidth(width);
            tick_end.setX(attach_point.x() + distance_from_center * cos);
            tick_end.setY(attach_point.y() - distance_from_center * sin);
        };

        // determine tick size
        if (tick % large_interval == 0)
        {
            set_tick_width_and_endpoint(large_width, large_end);
            // calculate and display the speed label
            int speed_label = (total_ticks - tick) / 4 * 20;
            QPoint speed_label_position(tick_start.x() - label * cos + label_offset_x,
                                        tick_start.y() + label * sin + label_offset_y);
            painter.drawText(speed_label_position, QString::number(speed_label));
        }
        else if (tick % medium_interval == 0)
        {
            set_tick_width_and_endpoint(medium_width, medium_end);
        }
        else
        {
            set_tick_width_and_endpoint(small_width, small_end);
        }

        painter.setPen(pen);
        painter.drawLine(tick_start, tick_end);
    }
}

void Canvas::paint_temperature(QPainter &painter)
{
    using speedometer::icons::temperature;
    int temp = comserv->get_temperature();
    painter.setPen(determine_color(temperature, temp));
    paint_icon(painter, temperature);
    painter.setPen(Qt::white);
    painter.setFont(temperature.label_font);
    painter.drawText(temperature.label, QString("%1°C").arg(temp, 3));
}

void Canvas::paint_battery(QPainter &painter)
{
    using namespace ::speedometer::icons;

    unsigned battery_state = comserv->get_battery_level();
    QColor battery_color = determine_color(battery, battery_state);
    painter.setPen(battery_color);
    painter.setBrush(battery_color);
    paint_icon(painter, battery);
    // bar inside icon showing percentage
    QRect battery_level_bar(battery_level, battery_level_size);

    // map battery_level to gui
    battery_level_bar.setHeight((battery_level_size.height() * static_cast<int>(battery_state)) / 100);
    painter.drawRect(battery_level_bar);
    painter.setPen(Qt::white);
    painter.setFont(battery.label_font);
    painter.drawText(battery.label, QString("%1%").arg(battery_state, 3)); // 3 is max digits
}

void Canvas::paint_icon(QPainter &painter, const speedometer::icons::Icon &icon)
{
    painter.setFont(icon.font);
    painter.drawText(icon.position, icon.icon);
}

QColor Canvas::determine_color(const speedometer::icons::IconWithColoredStates &icon, int value)
{
    return value < icon.low ? icon.lowc : value < icon.high ? icon.medc : icon.highc;
}
