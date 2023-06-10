#include "soundspectrumwidget.h"

#include <QPainter>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent)
    : QWidget{parent}
{
}

QSize SoundSpectrumWidget::minimumSizeHint() const
{
    return QSize(2000, 500);
}

void SoundSpectrumWidget::setCurrentPosition(float position)
{
    this->currentPosition = position;

    update();
}

void SoundSpectrumWidget::setSamples(QList<DecodedSampleModel> samples)
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

    update();
}

void SoundSpectrumWidget::setTimingsStartTime(QList<qint64> timingsStartTime)
{
    this->timingsStartTime.clear();
    this->timingsStartTime.append(timingsStartTime);

    update();
}

void SoundSpectrumWidget::paintEvent(QPaintEvent *event)
{
    QPen borderPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap);
    QPainter painter(this);
    painter.setPen(borderPen);
    painter.drawLine(0, 0, 0, height());
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(width(), height(), width(), 0);
    painter.drawLine(width(), height(), 0, height());

    if (this->samples.empty()) {
        return;
    }

    QPen samplePen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap);
    QPen currentPositionPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap);
    QPen timingPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(samplePen);

    float widthToDuration = width() * 1.0f / samplesDuration;
    float heightToDataRange = height() * 1.0f / (2 * samplesMaxAbsoluteData);

    for (qint16 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        bool isCurrentPositionSample = i < samples.size() - 1 && sample.startTime <= currentPosition && samples[i + 1].startTime >= currentPosition;
        bool isTimingSample = timingsStartTime.contains(sample.startTime);
        if (isCurrentPositionSample) {
            painter.setPen(currentPositionPen);
        } else if (isTimingSample) {
            painter.setPen(timingPen);
        }

        float x = (sample.startTime - samplesMinSecond) * widthToDuration;
        float minY = height() - (sample.minData + samplesMaxAbsoluteData) * heightToDataRange;
        float maxY = height() - (sample.maxData + samplesMaxAbsoluteData) * heightToDataRange;

        painter.drawLine(x, minY, x, maxY);
        painter.setPen(samplePen);
    }
}
