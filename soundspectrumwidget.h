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

    QSize minimumSizeHint() const override;

public slots:
    void onPlayerPositionChanged(float position);
    void onAudioDecoderDecoded(QList<DecodedSampleModel> samples);
    void onTimingTimingsChanged(QList<TimingModel> timings);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float currentPosition;
    QList<DecodedSampleModel> samples;
    QList<TimingModel> timings;
    QList<qint64> timingsStartTime;

    qint64 samplesMinSecond;
    qint64 samplesMaxSecond;
    qint64 samplesDuration;
    qint16 samplesMaxAbsoluteData;

    QPen samplePen;
    QPen timingPen;
    QPen currentPositionPen;
    QPen currentPositionTimingPen;
};

#endif // SOUNDSPECTRUMWIDGET_H
