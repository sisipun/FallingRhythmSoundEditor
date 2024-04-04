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

    void setCurrentSample(qint64 index);
    void setTimingsSamples(QSet<qint64> indices);

    float getSamplePosition(qint64 index);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void paintSample(QPainter& painter, const DecodedSampleModel& sample, Qt::GlobalColor color);
    void updateSamplesPaintProperties();

    qint64 getSamplesDuration();
    qint64 getSamplesMaxAbsoluteData();
    float getWidthToDuration();
    float getHeightToDataRange();

private:
    QList<DecodedSampleModel> samples;
    QSet<qint64> timingsSamplesIndices;
    qint64 currentSampleIndex;

    qint64 samplesMinSecond;
    qint64 samplesMaxSecond;
    qint64 samplesMinData;
    qint16 samplesMaxData;
};

#endif // SOUNDSPECTRUMWIDGET_H
