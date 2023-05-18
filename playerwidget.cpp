#include "playerwidget.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QtWidgets>
#include <QtMath>

PlayerWidget::PlayerWidget(QWidget* parent)
    : QWidget { parent }
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerWidget::onDurationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerWidget::onPositionChanged);

    title = new QLabel(this);
    title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    pauseButton = new QPushButton();
    pauseButton->setText(tr(">"));
    pauseButton->setDisabled(true);
    connect(pauseButton, &QPushButton::clicked, this, &PlayerWidget::onPauseButtonClicked);

    timeline = new QSlider(Qt::Horizontal, this);
    timeline->setDisabled(true);
    connect(timeline, &QSlider::sliderPressed, this, &PlayerWidget::onTimelinePressed);
    connect(timeline, &QSlider::sliderReleased, this, &PlayerWidget::onTimelineReleased);
    connect(timeline, &QSlider::sliderMoved, this, &PlayerWidget::onTimelineMoved);
    connect(timeline, &QSlider::valueChanged, this, &PlayerWidget::onTimelineValueChanged);

    timer = new QTimeEdit(this);
    timer->setDisplayFormat("HH:mm:ss");
    timer->setTime(QTime(0, 0));
    timer->setDisabled(true);
    timer->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);

    loadSoundButton = new QPushButton(this);
    loadSoundButton->setText(tr("Load sound"));
    connect(loadSoundButton, &QPushButton::clicked, this, &PlayerWidget::onLoadSoundButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    QBoxLayout* playerLayout = new QHBoxLayout;
    playerLayout->addWidget(pauseButton);
    playerLayout->addWidget(timeline);
    playerLayout->addWidget(timer);

    QBoxLayout* timingsLayout = new QHBoxLayout;
    timingsLayout->addWidget(loadSoundButton);

    layout->addLayout(titleLayout);
    layout->addLayout(playerLayout);
    layout->addLayout(timingsLayout);

    setLayout(layout);
}

void PlayerWidget::load(const QString& soundFilePath)
{
    title->setText(soundFilePath);
    timeline->setDisabled(false);
    pauseButton->setDisabled(false);
    mediaPlayer->setSource(QUrl::fromLocalFile(soundFilePath));
    emit loaded(true);
}

void PlayerWidget::play()
{
    if (isLoaded()) {
        mediaPlayer->play();
        pauseButton->setText(tr("||"));
    }
}

void PlayerWidget::pause()
{
    if (isLoaded()) {
        mediaPlayer->pause();
        pauseButton->setText(tr(">"));
    }
}

void PlayerWidget::stop()
{
    if (isLoaded()) {
        mediaPlayer->stop();
        timeline->setValue(0);
        pauseButton->setText(tr(">"));
    }
}

bool PlayerWidget::isLoaded() const
{
    return timeline->isEnabled();
}

bool PlayerWidget::isPlaying() const
{
    return mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::PlayingState;
}

bool PlayerWidget::isPaused() const
{
    return mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::PausedState;
}

bool PlayerWidget::isStopped() const
{
    return mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::StoppedState;
}

float PlayerWidget::getPosition() const
{
    return timeline->value() / 1000.0;
}

void PlayerWidget::setPosition(float seconds)
{
    mediaPlayer->setPosition(seconds * 1000);
}

QString PlayerWidget::getSoundName() const
{
    return title->text();
}

QUrl PlayerWidget::getSource() const
{
    return mediaPlayer->source();
}

void PlayerWidget::onLoadSoundButtonClicked()
{
    pause();
    QString soundFilePath = QFileDialog::getOpenFileName(
        this, tr("Open sound"), QString(), tr("Sound Files (*.wav *.mp3 *.ogg)"));
    if (soundFilePath.isEmpty()) {
        return;
    }

    load(soundFilePath);
    play();
}

void PlayerWidget::onPauseButtonClicked()
{
    if (isPlaying()) {
        pause();
    } else {
        play();
    }
}

void PlayerWidget::onDurationChanged(qint64 duration)
{
    timeline->setMaximum(duration);
}

void PlayerWidget::onPositionChanged(qint64 position)
{
    timeline->setValue(position);
    emit positionChanged(getPosition());
}

void PlayerWidget::onTimelinePressed()
{
    mediaPlayer->pause();
}

void PlayerWidget::onTimelineReleased()
{
    if (timeline->maximum() != timeline->value()) {
        play();
    } else {
        stop();
    }
}

void PlayerWidget::onTimelineMoved(int value)
{
    mediaPlayer->setPosition(value);
}

void PlayerWidget::onTimelineValueChanged(int value)
{
    timer->setTime(QTime::fromMSecsSinceStartOfDay(value));

    if (timeline->maximum() == value && !timeline->isSliderDown()) {
        stop();
    }
}
