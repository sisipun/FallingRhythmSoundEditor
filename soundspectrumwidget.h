#ifndef SOUNDSPECTRUMWIDGET_H
#define SOUNDSPECTRUMWIDGET_H

#include "decodedsamplemodel.h"
#include "timingmodel.h"

#include <QWidget>
#include <QPen>

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class SoundSpectrumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundSpectrumWidget(QWidget *parent = nullptr);

public:
    void showSamples(const QList<DecodedSampleModel>& samples);
    void reload();

    void setCurrentSample(int index);
    void setTimingsSamples(QSet<int> indices);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void paintSample(QPainter& painter, const DecodedSampleModel& sample, Qt::GlobalColor color);

private:
    QList<DecodedSampleModel> samples;
    int currentSampleIndex;
    QSet<int> timingsSamplesIndices;

    qint64 samplesMinSecond;
    qint64 samplesMaxSecond;
    qint64 samplesDuration;
    qint16 samplesMaxAbsoluteData;
    float widthToDuration;
    float heightToDataRange;
};

#endif // SOUNDSPECTRUMWIDGET_H
