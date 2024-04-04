#include "soundspectrumwidget.h"

#include <QPainter>
#include <QPaintEvent>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent)
    : QWidget{parent}
    , currentSampleIndex(-1)
    , samplesMinSecond(0)
    , samplesMaxSecond(0)
    , samplesMinData(0)
    , samplesMaxData(0)
{

}

void SoundSpectrumWidget::showSamples(const QList<DecodedSampleModel>& samples)
{
    this->samples = samples;
    if (!samples.isEmpty()) {
        samplesMinSecond = this->samples[0].startTime;
        samplesMaxSecond = this->samples[this->samples.size()-1].startTime;
        qint16 minData = this->samples[0].minData;
        qint16 maxData = this->samples[0].maxData;

        for (const DecodedSampleModel& sample: samples) {
            maxData = sample.maxData > maxData ? sample.maxData : maxData;
            minData = sample.minData < minData ? sample.minData : minData;
        }

        samplesMinData = minData;
        samplesMaxData = maxData;
    }

    update();
}

void SoundSpectrumWidget::setCurrentSample(qint64 index)
{
    this->currentSampleIndex = index;

    update();
}

void SoundSpectrumWidget::setTimingsSamples(QSet<qint64> indices)
{
    this->timingsSamplesIndices = indices;

    update();
}

float SoundSpectrumWidget::getSamplePosition(qint64 index)
{
    if (samples.size() <= index)
    {
        return 0;
    }

    float widthToDuration = getWidthToDuration();
    return (samples[index].startTime - samplesMinSecond) * widthToDuration;
}

void SoundSpectrumWidget::resizeEvent(QResizeEvent *event)
{
    update();
}

void SoundSpectrumWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (this->samples.empty()) {
        painter.eraseRect(event->rect());
        return;
    }

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];

        Qt::GlobalColor color = Qt::black;
        if (currentSampleIndex == i) {
            color = Qt::blue;
        } else if (timingsSamplesIndices.contains(i)) {
            color = Qt::green;
        }

        paintSample(painter, sample, color);
    }
}

void SoundSpectrumWidget::paintSample(QPainter& painter, const DecodedSampleModel& sample, Qt::GlobalColor color)
{
    float widthToDuration = getWidthToDuration();
    float heightToDataRange = getHeightToDataRange();
    qint64 samplesMaxAbsoluteData = getSamplesMaxAbsoluteData();
    float minX = (sample.startTime - samplesMinSecond) * widthToDuration;
    float maxX = (sample.endTime - samplesMinSecond) * widthToDuration;
    float minY = height() - (sample.minData + samplesMaxAbsoluteData) * heightToDataRange;
    float maxY = height() - (sample.maxData + samplesMaxAbsoluteData) * heightToDataRange;
    if (minY == maxY) {
        minY -= 1;
        maxY += 1;
    }
    painter.fillRect(minX, minY, maxX - minX, maxY - minY, color);
}

qint64 SoundSpectrumWidget::getSamplesDuration()
{
    return samplesMaxSecond - samplesMinSecond;
}

qint64 SoundSpectrumWidget::getSamplesMaxAbsoluteData()
{
    return (samplesMaxData > -samplesMinData) ? samplesMaxData : -samplesMinData;
}

float SoundSpectrumWidget::getWidthToDuration()
{
    qint64 samplesDuration = getSamplesDuration();
    return samplesDuration > 0 ? width() * 1.0f / samplesDuration : 0.0f;
}

float SoundSpectrumWidget::getHeightToDataRange()
{
    qint64 samplesMaxAbsoluteData = getSamplesMaxAbsoluteData();
    return samplesMaxAbsoluteData > 0 ? height() * 1.0f / (2 * samplesMaxAbsoluteData) : 0.0f;
}
