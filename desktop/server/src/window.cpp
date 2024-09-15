#include "window.h"
#include "setting.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Window::Window(COMService *comserv, QWidget *parent)
    : QDialog(parent), comserv(comserv), speedLabel(this), speedLabel2(this), temperatureLabel(this),
      temperatureLabel2(this), batteryLabel(this), batteryLabel2(this), speedSlider(Qt::Horizontal, this),
      temperatureSlider(Qt::Horizontal, this), batterySlider(Qt::Horizontal, this), leftCheckBox(this),
      rightCheckBox(this), warningCheckBox(this), layout(this), checkBoxLayout()
{
    Setting::Signal &signal{Setting::Signal::instance()};

    setWindowTitle("Server");
    setFixedSize(Setting::Server_GUI::Window_Size::width, Setting::Server_GUI::Window_Size::height);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    speedLabel.setText("Speed");
    speedLabel2.setText("0 kph");
    speedSlider.setRange(signal["speed"].min, signal["speed"].max);
    QObject::connect(&speedSlider, &QSlider::valueChanged, [this, comserv](int value) {
        speedLabel2.setText(QString("%1 kph").arg(value));
        comserv->set_speed(value);
    });
    speedLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&speedLabel, 0, 0);
    layout.addWidget(&speedSlider, 0, 1);
    layout.addWidget(&speedLabel2, 0, 2);

    temperatureLabel.setText("Temperature");
    temperatureLabel2.setText("0°C");
    temperatureSlider.setRange(signal["temperature"].min, signal["temperature"].max);
    QObject::connect(&temperatureSlider, &QSlider::valueChanged, [this, comserv](int value) {
        temperatureLabel2.setText(QString("%1°C").arg(value));
        comserv->set_temperature(value);
    });
    temperatureLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&temperatureLabel, 1, 0);
    layout.addWidget(&temperatureSlider, 1, 1);
    layout.addWidget(&temperatureLabel2, 1, 2);

    batteryLabel.setText("Battery Level");
    batteryLabel2.setText("0%");
    batterySlider.setRange(signal["battery_level"].min, signal["battery_level"].max);
    QObject::connect(&batterySlider, &QSlider::valueChanged, [this, comserv](int value) {
        batteryLabel2.setText(QString("%1%").arg(value));
        comserv->set_battery_level(value);
    });
    batteryLabel2.setFixedWidth(Setting::Server_GUI::Label_Width::width);
    layout.addWidget(&batteryLabel, 2, 0);
    layout.addWidget(&batterySlider, 2, 1);
    layout.addWidget(&batteryLabel2, 2, 2);

    leftCheckBox.setText("Left Signal");
    rightCheckBox.setText("Right Signal");
    warningCheckBox.setText("Warning Signal");

    // Add the checkboxes to the horizontal layout

    checkBoxLayout.addWidget(&leftCheckBox);
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(&warningCheckBox);
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(&rightCheckBox);

    // Add the horizontal layout to the grid layout
    layout.addLayout(&checkBoxLayout, 3, 0, 1, 3); // Span across 3 columns

    rightCheckBox.setStyleSheet("QCheckBox:disabled { color: rgba(0, 0, 0, 0.5); }");
    leftCheckBox.setStyleSheet("QCheckBox:disabled { color: rgba(0, 0, 0, 0.5); }");

    QObject::connect(&leftCheckBox, &QCheckBox::stateChanged, [this, comserv]() {
        if (leftCheckBox.isChecked())
        {
            comserv->set_turn_signal(warningCheckBox.isChecked() ? Setting::WARNING : Setting::LEFT);
            rightCheckBox.setDisabled(true);
        }
        else
        {
            comserv->set_turn_signal(warningCheckBox.isChecked() ? Setting::WARNING : Setting::OFF);
            rightCheckBox.setEnabled(true);
        }
    });

    QObject::connect(&rightCheckBox, &QCheckBox::stateChanged, [this, comserv]() {
        if (rightCheckBox.isChecked())
        {
            comserv->set_turn_signal(warningCheckBox.isChecked() ? Setting::WARNING : Setting::RIGHT);
            leftCheckBox.setDisabled(true);
        }
        else
        {
            comserv->set_turn_signal(warningCheckBox.isChecked() ? Setting::WARNING : Setting::OFF);
            leftCheckBox.setEnabled(true);
        }
    });

    QObject::connect(&warningCheckBox, &QCheckBox::stateChanged, [this, comserv]() {
        if (warningCheckBox.isChecked())
        {
            comserv->set_turn_signal(Setting::WARNING);
        }
        else if (leftCheckBox.isChecked())
        {
            comserv->set_turn_signal(Setting::LEFT);
        }
        else if (rightCheckBox.isChecked())
        {
            comserv->set_turn_signal(Setting::RIGHT);
        }
        else
        {
            comserv->set_turn_signal(Setting::OFF);
        }
    });
    setLayout(&layout);
}
