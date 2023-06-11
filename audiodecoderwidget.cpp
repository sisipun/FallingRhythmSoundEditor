#include "audiodecoderwidget.h"

#include "playerwidget.h"

#include <QAudioDecoder>
#include <QtWidgets>

AudioDecoderWidget::AudioDecoderWidget(PlayerWidget* player, QWidget *parent): QWidget{parent}
{
    this->player = player;

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
    QList<qint32> samplesData;
    for (const DecodedSampleModel& sample: decodedSamples) {
        samplesData.append(sample.maxData - sample.minData);
    }
    std::sort(samplesData.begin(), samplesData.end());
    qint32 dataThreshold = samplesData[3 * samplesData.size() / 5];

    QList<DecodedSampleModel> samplesToGenerate;
    for (qint64 i = 1; i < decodedSamples.size() - 1; i++) {
        const DecodedSampleModel& previousSample = decodedSamples[i - 1];
        const DecodedSampleModel& currentSample = decodedSamples[i];
        const DecodedSampleModel& nextSample = decodedSamples[i + 1];

        qint32 previousDataRange = previousSample.maxData - previousSample.minData;
        qint32 currentDataRange = currentSample.maxData - currentSample.minData;
        qint32 nextDataRange = nextSample.maxData - nextSample.minData;

        if (previousDataRange < currentDataRange && currentDataRange > nextDataRange && currentDataRange > dataThreshold) {
            samplesToGenerate.append(currentSample);
        }
    }

    if (samplesToGenerate.empty()) {
        return;
    }

    QList<qint64> samplesDistance;
    for (qint64 i = 1; i < samplesToGenerate.size(); i++) {
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

    QList<TimingModel> timings;
    for (qint64 i = 0; i < samplesToGenerate.size() - 1; i++) {
        const DecodedSampleModel& currentSample = samplesToGenerate[i];
        const DecodedSampleModel& nextSample = samplesToGenerate[i + 1];
        qint64 distance = nextSample.startTime - currentSample.startTime;

        if (distance < halfMeanDistance) {
            type = TimingType::PICKUP_LINE;
            endTime = nextSample.startTime;
        }
        else {
            TimingModel model = {startTime, endTime, type, TimingSide::RIGHT, position};
            timings.append(model);
            startTime = nextSample.startTime;
            endTime = startTime;
            type = TimingType::PICKUP;
        }

        if (distance > meanDistance) {
            position = (QRandomGenerator::global()->generateDouble() - 0.5f) * 2.0f;
        }
    }

    TimingModel model = {startTime, endTime, type, TimingSide::RIGHT, position};
    timings.append(model);

    emit generated(timings);
}

void AudioDecoderWidget::onDecodedBufferReady()
{
    QAudioBuffer buffer = audioDecoder->read();
    QAudioFormat format = buffer.format();

    qint16 const* data = buffer.constData<qint16>();
    qint64 frameDuration = buffer.duration() / buffer.frameCount();
    qint64 currentFrameStartTime = buffer.startTime();
    qint16 maxData = 0;
    qint16 minData = 0;
    qint64 startTime = currentFrameStartTime;
    for (qint64 i = 0; i < buffer.frameCount(); i++, currentFrameStartTime+=frameDuration) {
        for (qint16 j = 0; j < format.channelCount(); j++) {
            qint16 value = *(data + i * format.channelCount() + j);
            if (value > maxData) {
                maxData = value;
                if (maxData > -minData) {
                    startTime = currentFrameStartTime;
                }
            }
            if (value < minData) {
                minData = value;
                if (-minData > maxData) {
                    startTime = currentFrameStartTime;
                }
            }
        }
    }
    decodedSamples.append({startTime / 1000, minData, maxData});
}

void AudioDecoderWidget::onDecodeFinished()
{
    generateTimingsButton->setDisabled(false);
    emit decoded(decodedSamples);
}
