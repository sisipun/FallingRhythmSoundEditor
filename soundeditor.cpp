#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "playerwidget.h"
#include "audiodecoderwidget.h"
#include "timingwidget.h"
#include "soundspectrumarea.h"

#include <QKeyEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QSlider>

SoundEditor::SoundEditor(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new PlayerWidget(this);
    soundSpectrum = new SoundSpectrumArea(this);
    QSlider* soundSpectrumZoom = new QSlider(Qt::Vertical, this);
    soundSpectrumZoom->setMinimum(1.0f);
    soundSpectrumZoom->setMaximum(100.0f);
    soundSpectrumZoom->setDisabled(true);
    audioDecoder = new AudioDecoderWidget(player, this);
    timing = new TimingWidget(player, this);

    connect(player, &PlayerWidget::loaded, soundSpectrum, &SoundSpectrumArea::onPlayerLoaded);
    connect(player, &PlayerWidget::positionChanged, soundSpectrum, &SoundSpectrumArea::onPlayerPositionChanged);
    connect(audioDecoder, &AudioDecoderWidget::decoded, soundSpectrum, &SoundSpectrumArea::onAudioDecoderDecoded);
    connect(timing, &TimingWidget::timingsChanged, soundSpectrum, &SoundSpectrumArea::onTimingTimingsChanged);
    connect(soundSpectrumZoom, &QSlider::valueChanged, soundSpectrum, &SoundSpectrumArea::onZoomChanged);

    connect(player, &PlayerWidget::loaded, audioDecoder, [=]{ soundSpectrumZoom->setDisabled(false); });

    connect(player, &PlayerWidget::loaded, audioDecoder, &AudioDecoderWidget::onPlayerLoaded);

    connect(player, &PlayerWidget::loaded, timing, &TimingWidget::onPlayerLoaded);
    connect(player, &PlayerWidget::positionChanged, timing, &TimingWidget::onPlayerPositionChanged);
    connect(audioDecoder, &AudioDecoderWidget::generated, timing, &TimingWidget::onAudioDecoderGenerated);

    QBoxLayout* soundSpectrumLayout = new QHBoxLayout;
    soundSpectrumLayout->addWidget(soundSpectrum, 9);
    soundSpectrumLayout->addWidget(soundSpectrumZoom, 1);
    soundSpectrumZoom->setValue(50.0f);

    ui->gridLayout->addWidget(player, 0, 0);
    ui->gridLayout->addLayout(soundSpectrumLayout, 1, 0);
    ui->gridLayout->addWidget(audioDecoder, 2, 0);
    ui->gridLayout->addWidget(timing, 3, 0);

    setFocusPolicy(Qt::StrongFocus);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}

void SoundEditor::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        if (player->isPlaying()) {
            player->pause();
        } else {
            player->play();
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
