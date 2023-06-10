#include "audiodecoderwidget.h"

#include "playerwidget.h"
#include "soundspectrumwidget.h"

#include <QAudioDecoder>
#include <QtWidgets>

float AudioDecoderWidget::MICROSECONDS_IN_SECOND = 1000000.0f;

AudioDecoderWidget::AudioDecoderWidget(PlayerWidget* player, QWidget *parent)
    : QWidget{parent}
{
    this->player = player;
    connect(this->player, &PlayerWidget::loaded, this, &AudioDecoderWidget::onPlayerLoaded);
    connect(this->player, &PlayerWidget::positionChanged, this, &AudioDecoderWidget::onPlayerPositionChanged);

    this->soundSpectrumWidget = new SoundSpectrumWidget(this);

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

    QBoxLayout* spectrumLayout = new QHBoxLayout;
    spectrumLayout->addWidget(soundSpectrumWidget, 1);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(decodeButton, 1);
    actionsLayout->addWidget(generateTimingsButton, 1);

    layout->addLayout(spectrumLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void AudioDecoderWidget::onPlayerLoaded(bool loaded)
{
    decodeButton->setDisabled(!loaded);
    generateTimingsButton->setDisabled(true);
}

void AudioDecoderWidget::onPlayerPositionChanged(float position)
{
    soundSpectrumWidget->setCurrentPosition(position * MICROSECONDS_IN_SECOND);
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

        if (
            (previousSample.maxData < currentSample.maxData && currentSample.maxData > nextSample.maxData) ||
            (previousSample.minData > currentSample.minData && currentSample.minData < nextSample.minData)
        ) {
            samplesToGenerate.append(currentSample);
        }
    }

    if (samplesToGenerate.empty()) {
        return;
    }

    QList<qint64> samplesDistance;
    for (qint16 i = 1; i < samplesToGenerate.size(); i++) {
        samplesDistance.append(samplesToGenerate[i].startTime - samplesToGenerate[i -1].startTime);
    }
    std::sort(samplesDistance.begin(), samplesDistance.end());
    qint64 meanDistance = samplesDistance[samplesDistance.size() / 2];
    qint64 halfMeanDistance = samplesDistance[samplesDistance.size() / 4];

    const DecodedSampleModel& firstSample = samplesToGenerate[0];
    float position = (QRandomGenerator::global()->generateDouble() - 0.5f) * 2.0f;
    qint64 startTime = firstSample.startTime;
    qint64 endTime = startTime;
    TimingType type = TimingType::PICKUP;

    QList<GeneratedTimingModel> timings;
    QList<qint64> timingsStartTime;
    for (qint16 i = 0; i < samplesToGenerate.size() - 1; i++) {
        const DecodedSampleModel& currentSample = samplesToGenerate[i];
        const DecodedSampleModel& nextSample = samplesToGenerate[i + 1];
        qint64 distance = nextSample.startTime - currentSample.startTime;

        if (distance < halfMeanDistance) {
            type = TimingType::PICKUP_LINE;
            endTime = nextSample.startTime;
        }
        else {
            GeneratedTimingModel model = {startTime / MICROSECONDS_IN_SECOND, endTime / MICROSECONDS_IN_SECOND, type, TimingSide::RIGHT, position};
            timings.append(model);
            timingsStartTime.append(startTime);
            startTime = nextSample.startTime;
            endTime = startTime;
            type = TimingType::PICKUP;
        }

        if (distance > meanDistance) {
            position = (QRandomGenerator::global()->generateDouble() - 0.5f) * 2.0f;
        }
    }

    GeneratedTimingModel model = {startTime / MICROSECONDS_IN_SECOND, endTime / MICROSECONDS_IN_SECOND, type, TimingSide::RIGHT, position};
    timings.append(model);
    timingsStartTime.append(startTime);

    soundSpectrumWidget->setTimingsStartTime(timingsStartTime);
    emit generated(timings);
}

void AudioDecoderWidget::onDecodedBufferReady()
{
    QAudioBuffer buffer = audioDecoder->read();
    QAudioFormat format = buffer.format();

    qint16 const* data = buffer.constData<qint16>();
    qint16 maxData = 0;
    qint16 minData = 0;
    for (int i = 0; i < buffer.frameCount(); i++) {
        for (int j = 0; j < format.channelCount(); j++) {
            qint16 value = *(data + i * format.channelCount() + j);
            if (value > maxData) {
                maxData = value;
            }
            if (value < minData) {
                minData = value;
            }
        }
    }

    decodedSamples.append({buffer.startTime(), minData, maxData});
}

void AudioDecoderWidget::onDecodeFinished()
{
    generateTimingsButton->setDisabled(false);
    soundSpectrumWidget->setSamples(decodedSamples);
}
