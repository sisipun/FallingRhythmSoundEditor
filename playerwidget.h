#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
class QAudioOutput;
class QSlider;
class QTimeEdit;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class PlayerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget* parent = nullptr);

    void load(const QString& soundFilePath);
    void play();
    void pause();
    void stop();
    bool isLoaded() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;
    float getPosition() const;
    void setPosition(float seconds);
    QString getSoundName() const;
    QUrl getSource() const;

signals:
    void loaded(bool loaded);
    void positionChanged(float position);

private slots:
    void onLoadSoundButtonClicked();
    void onPauseButtonClicked();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onTimelinePressed();
    void onTimelineReleased();
    void onTimelineMoved(int value);
    void onTimelineValueChanged(int value);

private:
    QMediaPlayer* mediaPlayer = nullptr;
    QAudioOutput* audioOutput = nullptr;

    QLabel* title = nullptr;
    QSlider* timeline = nullptr;
    QTimeEdit* timer = nullptr;
    QPushButton* pauseButton = nullptr;
    QPushButton* loadSoundButton = nullptr;
};

#endif // PLAYERWIDGET_H
