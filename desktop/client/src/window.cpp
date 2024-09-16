#include "window.h"
#include "setting.h"
#include <QTimer>

Window::Window(COMService *comserv, QWidget *parent)
    : QDialog(parent), comserv(comserv), canvas(comserv, this), update_timer(this), blink_timer(this),
      turn_signal_sound(this), audio_output(this)
{
    setWindowTitle("Client");
    setFixedSize(width_, height_);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    canvas.setGeometry(0, 0, width_, height_);

    turn_signal_sound.setAudioOutput(&audio_output);
    turn_signal_sound.setSource(QUrl("qrc:/turn-signals.wav"));
    turn_signal_sound.setLoops(QMediaPlayer::Infinite);

    // repaint the canvas
    connect(&update_timer, SIGNAL(timeout()), &canvas, SLOT(update()));

    // turn signal sound and visual toggles
    connect(&blink_timer, &QTimer::timeout, [this]() {
        manage_blinker_sound();
        canvas.is_visible = !canvas.is_visible;
        canvas.update();
    });

    blink_timer.start(350);
    update_timer.start(Setting::INTERVAL_MS);
}

void Window::manage_blinker_sound()
{
    bool sound = comserv->get_turn_signal_state() != Setting::OFF && comserv->get_connection_state();

    // Manage the sound playback based on the signal state
    if (sound && !turn_signal_sound.isPlaying())
    {
        turn_signal_sound.play();
    }
    else if (!sound && turn_signal_sound.isPlaying())
    {
        turn_signal_sound.stop();
    }
}
