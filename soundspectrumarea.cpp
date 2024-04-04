#include "soundspectrumarea.h"

#include "soundspectrumwidget.h"

#include <QScrollBar>

SoundSpectrumArea::SoundSpectrumArea(QWidget *parent)
    : QScrollArea{parent}
    , playerPosition(0.0f)
    , zoomValue(10.0f)
{
    verticalScrollBar()->setDisabled(true);

    soundSpectrum = new SoundSpectrumWidget(this);
    setWidget(soundSpectrum);
}

void SoundSpectrumArea::onPlayerLoaded()
{
    onTimingTimingsChanged(QList<TimingModel>());
    onAudioDecoderDecoded(QList<DecodedSampleModel>());
}

void SoundSpectrumArea::onPlayerPositionChanged(float position)
{
    this->playerPosition = position;

    qint64 currentSample = getCurrentSample();
    if (currentSample >= 0) {
        soundSpectrum->setCurrentSample(currentSample);
        horizontalScrollBar()->setValue(soundSpectrum->getSamplePosition(currentSample) - (width() / 2.0));
    }
}

void SoundSpectrumArea::onZoomChanged(float value)
{
    this->zoomValue = value;
    soundSpectrum->setFixedWidth(this->samples.size() * value);
    soundSpectrum->setFixedHeight(height());
}

void SoundSpectrumArea::onAudioDecoderDecoded(QList<DecodedSampleModel> samples)
{
    this->samples = samples;
    soundSpectrum->setFixedWidth(this->samples.size() * zoomValue);
    soundSpectrum->setFixedHeight(height());
    soundSpectrum->showSamples(samples);
    soundSpectrum->setCurrentSample(getCurrentSample());
}

void SoundSpectrumArea::onTimingTimingsChanged(QList<TimingModel> timings)
{
    this->timings = timings;

    QSet<qint64> timingsSamplesIndices;
    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        for (qint64 j = 0; j < timings.size(); j++) {
            const TimingModel& timing = timings[j];
            if (timing.startTime < sample.endTime && timing.endTime > sample.startTime) {
                timingsSamplesIndices.insert(i);
            }
        }
    }
    soundSpectrum->setTimingsSamples(timingsSamplesIndices);
}

qint64 SoundSpectrumArea::getCurrentSample() const
{
    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        if (sample.startTime > playerPosition) {
            return i;
        }
    }

    return -1;
}
