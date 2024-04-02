#include "soundspectrumwidget.h"

#include <QPainter>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent):
    QWidget{parent}
{
}

void SoundSpectrumWidget::onPlayerPositionChanged(float position)
{
    this->currentPosition = position;

    update();
}

void SoundSpectrumWidget::onAudioDecoderDecoded(QList<DecodedSampleModel> samples)
{
    this->samples.clear();
    this->samples.append(samples);

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

    setFixedWidth(samples.size() * 100);
    setFixedHeight(parentWidget()->size().height());

    update();
}

void SoundSpectrumWidget::onTimingTimingsChanged(QList<TimingModel> timings)
{
    this->timings.clear();
    this->timings.append(timings);
    this->timingsStartTime.clear();

    for (const TimingModel& timing: timings) {
        this->timingsStartTime.append(timing.startTime);
    }

    update();
}

void SoundSpectrumWidget::paintEvent(QPaintEvent *event)
{
    if (this->samples.empty()) {
        return;
    }

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    float widthToDuration = width() * 1.0f / samplesDuration;
    float heightToDataRange = height() * 1.0f / (2 * samplesMaxAbsoluteData);

    float last_x = 0;
    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        bool isCurrentPositionSample = i < samples.size() - 1 && sample.startTime <= currentPosition && samples[i + 1].startTime >= currentPosition;
        bool isTimingSample = timingsStartTime.contains(sample.startTime);
        Qt::GlobalColor color = Qt::black;
        if (isTimingSample && isCurrentPositionSample) {
            color = Qt::red;
        } else if (isCurrentPositionSample) {
            color = Qt::blue;
        } else if (isTimingSample) {
            color = Qt::green;
        }

        float x = (sample.startTime - samplesMinSecond) * widthToDuration;
        float minY = height() - (sample.minData + samplesMaxAbsoluteData) * heightToDataRange;
        float maxY = height() - (sample.maxData + samplesMaxAbsoluteData) * heightToDataRange;
        if (minY == maxY) {
            minY -= 1;
            maxY += 1;
        }

        painter.fillRect(last_x, minY, x - last_x, maxY - minY, color);
        last_x = x;
    }
}
