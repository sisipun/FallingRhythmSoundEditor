#include "soundspectrumwidget.h"

#include <QPainter>

SoundSpectrumWidget::SoundSpectrumWidget(QWidget *parent)
    : QWidget{parent}
{
}

QSize SoundSpectrumWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

void SoundSpectrumWidget::setSamples(QList<DecodedSampleModel> samples)
{
    this->samples.clear();
    this->samples.append(samples);
    update();
}

void SoundSpectrumWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(0, 0, 0, height());
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(width(), height(), width(), 0);
    painter.drawLine(width(), height(), 0, height());

    if (this->samples.empty())
    {
        return;
    }

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    float minSecond = this->samples[0].startTime;
    float maxSecond = this->samples[this->samples.size()-1].startTime;
    float maxData = this->samples[0].data;

    for (const DecodedSampleModel& sample: samples)
    {
        float fabsData = qFabs(sample.data);
        maxData = fabsData > maxData ? fabsData : maxData;
    }

    float duration = maxSecond - minSecond;
    float widthToDuration = width() / duration;
    float dataRange = 2 * maxData;
    float halfHeight = height() / 2;
    float heightToDataRange = height() / dataRange;

    for (const DecodedSampleModel& sample: samples)
    {
        float x = (sample.startTime - minSecond) * widthToDuration;
        float y = height() - (sample.data + maxData) * heightToDataRange;

        painter.drawLine(x, halfHeight, x, y);
    }
}
