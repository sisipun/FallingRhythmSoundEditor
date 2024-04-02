#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "playerwidget.h"
#include "audiodecoderwidget.h"
#include "timingwidget.h"
#include "soundspectrumwidget.h"

#include <QKeyEvent>
#include <QScrollArea>
#include <QScrollBar>

SoundEditor::SoundEditor(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new PlayerWidget(this);
    soundSpectrum = new SoundSpectrumWidget(this);
    audioDecoder = new AudioDecoderWidget(player, this);
    timing = new TimingWidget(player, this);

    connect(player, &PlayerWidget::positionChanged, soundSpectrum, &SoundSpectrumWidget::onPlayerPositionChanged);
    connect(audioDecoder, &AudioDecoderWidget::decoded, soundSpectrum, &SoundSpectrumWidget::onAudioDecoderDecoded);
    connect(timing, &TimingWidget::timingsChanged, soundSpectrum, &SoundSpectrumWidget::onTimingTimingsChanged);

    connect(player, &PlayerWidget::loaded, audioDecoder, &AudioDecoderWidget::onPlayerLoaded);

    connect(player, &PlayerWidget::loaded, timing, &TimingWidget::onPlayerLoaded);
    connect(player, &PlayerWidget::positionChanged, timing, &TimingWidget::onPlayerPositionChanged);
    connect(audioDecoder, &AudioDecoderWidget::generated, timing, &TimingWidget::onAudioDecoderGenerated);

    QScrollArea* soundSpectrucScrollArea = new QScrollArea(this);
    soundSpectrucScrollArea->setWidget(soundSpectrum);
    soundSpectrucScrollArea->verticalScrollBar()->setDisabled(true);

    ui->gridLayout->addWidget(player);
    ui->gridLayout->addWidget(soundSpectrucScrollArea);
    ui->gridLayout->addWidget(audioDecoder);
    ui->gridLayout->addWidget(timing);

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
