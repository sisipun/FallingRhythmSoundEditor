#include "soundspectrumwidget.h"

#include <QPainter>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent):
    QWidget{parent},
    samplePen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap),
    timingPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap),
    currentPositionPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap),
    currentPositionTimingPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap)
{
}

QSize SoundSpectrumWidget::minimumSizeHint() const
{
    return QSize(2000, 500);
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
    painter.setPen(samplePen);

    float widthToDuration = width() * 1.0f / samplesDuration;
    float heightToDataRange = height() * 1.0f / (2 * samplesMaxAbsoluteData);

    for (qint64 i = 0; i < samples.size(); i++) {
        const DecodedSampleModel& sample = samples[i];
        bool isCurrentPositionSample = i < samples.size() - 1 && sample.startTime <= currentPosition && samples[i + 1].startTime >= currentPosition;
        bool isTimingSample = timingsStartTime.contains(sample.startTime);
        if (isTimingSample && isCurrentPositionSample) {
            painter.setPen(currentPositionTimingPen);
        } else if (isCurrentPositionSample) {
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
    qDebug() << "Draw finished";
}
