#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "comservice.h"

class Window : public QDialog
{
public:
    Window(COMService *comserv, QWidget *parent = nullptr);

private:
    COMService *comserv;
    QLabel speedLabel;
    QLabel speedLabel2;
    QLabel temperatureLabel;
    QLabel temperatureLabel2;
    QLabel batteryLabel;
    QLabel batteryLabel2;
    QSlider speedSlider;
    QSlider temperatureSlider;
    QSlider batterySlider;
    QCheckBox leftCheckBox;
    QCheckBox rightCheckBox;
    QCheckBox warningCheckBox;
    QGridLayout layout;
    QHBoxLayout checkBoxLayout;

    void send_it();
};

#endif // WINDOW_H
