#include "soundspectrumarea.h"

#include "soundspectrumwidget.h"

#include <QScrollBar>

SoundSpectrumArea::SoundSpectrumArea(QWidget *parent): QScrollArea(parent)
{
    verticalScrollBar()->setDisabled(true);

    soundSpectrum = new SoundSpectrumWidget(this);
    this->zoomValue = 100.0f;

    setWidget(soundSpectrum);
}

void SoundSpectrumArea::onPlayerPositionChanged(float position)
{
    this->playerPosition = position;

    int currentSample = getCurrentSample();
    if (currentSample >= 0) {
        soundSpectrum->setCurrentSample(currentSample);
    }
}

void SoundSpectrumArea::onZoomChanged(float value)
{
    this->zoomValue = value;
    soundSpectrum->setFixedWidth(this->samples.size() * value);
    soundSpectrum->setFixedHeight(height());
    soundSpectrum->reload();
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

    QSet<int> timingsSamplesIndices;
    for (int i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        for (int j = 0; j < timings.size(); j++) {
            const TimingModel& timing = timings[j];
            if (timing.startTime < sample.endTime && timing.endTime > sample.startTime) {
                timingsSamplesIndices.insert(i);
            }
        }
    }
    soundSpectrum->setTimingsSamples(timingsSamplesIndices);
}

int SoundSpectrumArea::getCurrentSample() const
{
    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        if (sample.startTime > playerPosition) {
            return i;
        }
    }

    return -1;
}
