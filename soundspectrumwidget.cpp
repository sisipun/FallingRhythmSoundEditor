#include "soundspectrumwidget.h"

#include <QPainter>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent):
    QWidget{parent}
{
}

void SoundSpectrumWidget::showSamples(const QList<DecodedSampleModel>& samples)
{
    this->samples = samples;

    samplesMinSecond = this->samples[0].startTime;
    samplesMaxSecond = this->samples[this->samples.size()-1].startTime;
    qint16 minData = this->samples[0].minData;
    qint16 maxData = this->samples[0].maxData;

    for (const DecodedSampleModel& sample: samples) {
        maxData = sample.maxData > maxData ? sample.maxData : maxData;
        minData = sample.minData < minData ? sample.minData : minData;
    }

    samplesDuration = samplesMaxSecond - samplesMinSecond;
    samplesMaxAbsoluteData = (maxData > -minData) ? maxData : -minData;

    widthToDuration = width() * 1.0f / samplesDuration;
    heightToDataRange = height() * 1.0f / (2 * samplesMaxAbsoluteData);

    reload();
}

void SoundSpectrumWidget::reload()
{
    update();
}

void SoundSpectrumWidget::setCurrentSample(int index)
{
    this->currentSampleIndex = index;

    reload();
}

void SoundSpectrumWidget::setTimingsSamples(QSet<int> indices)
{
    this->timingsSamplesIndices = indices;

    reload();
}

void SoundSpectrumWidget::paintEvent(QPaintEvent *event)
{
    if (this->samples.empty()) {
        return;
    }

    QPainter painter(this);
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
