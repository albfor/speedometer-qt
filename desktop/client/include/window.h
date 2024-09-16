#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include "comservice.h"

#include <QAudioOutput>
#include <QDialog>
#include <QMediaPlayer>
#include <QTimer>

class Window : public QDialog
{
public:
    explicit Window(COMService *comserv, QWidget *parent = nullptr);

private:
    COMService *comserv;
    Canvas canvas;
    QTimer update_timer;
    QTimer blink_timer;
    QMediaPlayer turn_signal_sound;
    QAudioOutput audio_output;
    constexpr static int width_ = 800;
    constexpr static int height_ = 560;

    void manage_blinker_sound();
};

#endif // WINDOW_H
