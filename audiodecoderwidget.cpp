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

    generateTimingsButton = new QPushButton(this);
    generateTimingsButton->setText(tr("Generate"));
    generateTimingsButton->setDisabled(true);
    connect(generateTimingsButton, &QPushButton::clicked, this, &AudioDecoderWidget::onGenerateTimingsButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(decodeButton, 1);
    actionsLayout->addWidget(generateTimingsButton, 1);

    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void AudioDecoderWidget::onPlayerLoaded(bool loaded)
{
    decodeButton->setDisabled(!loaded);
    generateTimingsButton->setDisabled(true);
}

void AudioDecoderWidget::onDecodeButtonClicked()
{
    generateTimingsButton->setDisabled(true);
    decodedSamples.clear();
    audioDecoder->setSource(player->getSource());
    audioDecoder->start();
}

void AudioDecoderWidget::onGenerateTimingsButtonClicked()
{
    QList<DecodedSampleModel> samplesToGenerate;
    for (qint16 i = 1; i < decodedSamples.size() - 1; i++) {
        const DecodedSampleModel& previousSample = decodedSamples[i - 1];
        const DecodedSampleModel& currentSample = decodedSamples[i];
        const DecodedSampleModel& nextSample = decodedSamples[i + 1];

        if (qFabs(previousSample.data) < qFabs(currentSample.data) && qFabs(currentSample.data) > qFabs(nextSample.data)) {
            samplesToGenerate.append(currentSample);
        }
    }

    if (samplesToGenerate.empty())
    {
        return;
    }

    QList<qint64> samplesDistance;
    for (qint16 i = 1; i < samplesToGenerate.size(); i++) {
        samplesDistance.append(samplesToGenerate[i].startTime - samplesToGenerate[i -1].startTime);
    }
    std::sort(samplesDistance.begin(), samplesDistance.end());
    float meanDistance = samplesDistance[samplesDistance.size() / 2];

    QList<GeneratedTimingModel> timings;
    float position;
    for (qint16 i = 0; i < samplesToGenerate.size(); i++) {
        const DecodedSampleModel& currentSample = samplesToGenerate[i];

        float startTime = currentSample.startTime / 1000000.0f;
        if (i == 0 || (currentSample.startTime - samplesToGenerate[i - 1].startTime) > meanDistance)
        {
            position = (QRandomGenerator::global()->generateDouble() - 0.5f) * 2.0f;
        }

        GeneratedTimingModel model = {startTime, startTime, TimingType::PICKUP, TimingSide::RIGHT, position};
        timings.append(model);
    }

    emit generated(timings);
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
    generateTimingsButton->setDisabled(false);
    for (const DecodedSampleModel& decodedSample : decodedSamples) {
        qDebug() << decodedSample.startTime << " - " << decodedSample.data;
    }
}
