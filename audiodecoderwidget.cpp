#include "audiodecoderwidget.h"

#include "playerwidget.h"

#include <QAudioDecoder>
#include <QtWidgets>

AudioDecoderWidget::AudioDecoderWidget(PlayerWidget* player, QWidget *parent)
    : QWidget{parent}
{
    this->player = player;
    connect(this->player, &PlayerWidget::loaded, this, &AudioDecoderWidget::onPlayerLoaded);

    audioDecoder = new QAudioDecoder(this);
    connect(audioDecoder, &QAudioDecoder::bufferReady, this, &AudioDecoderWidget::onDecodedBufferReady);
    connect(audioDecoder, &QAudioDecoder::finished, this, &AudioDecoderWidget::onDecodeFinished);

    decodeButton = new QPushButton(this);
    decodeButton->setText(tr("Decode"));
    decodeButton->setDisabled(true);
    connect(decodeButton, &QPushButton::clicked, this, &AudioDecoderWidget::onDecodeButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);\

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(decodeButton, 1);

    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void AudioDecoderWidget::onPlayerLoaded(bool loaded)
{
    decodeButton->setDisabled(!loaded);
}

void AudioDecoderWidget::onDecodeButtonClicked()
{
    decodedSamples.clear();
    audioDecoder->setSource(player->getSource());
    audioDecoder->start();
}

void AudioDecoderWidget::onDecodedBufferReady()
{
    QAudioBuffer buffer = audioDecoder->read();
    QAudioFormat format = buffer.format();

    qint16 const* data = buffer.constData<qint16>();
    qint16 maxData = 0;
    for (int i = 0; i < buffer.frameCount(); i++) {
        for (int j = 0; j < format.channelCount(); j++) {
            qint16 value = *(data + i * format.channelCount() + j);
            if (qFabs(value) > maxData)
            {
                maxData = value;
            }
        }
    }

    decodedSamples.append({buffer.startTime(), maxData});
}

void AudioDecoderWidget::onDecodeFinished()
{
    for (const DecodedSampleModel& decodedSample : decodedSamples)
    {
        qDebug() << decodedSample.startTime << " - " << decodedSample.data;
    }
}
