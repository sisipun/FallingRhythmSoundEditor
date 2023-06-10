#ifndef SOUNDSPECTRUMWIDGET_H
#define SOUNDSPECTRUMWIDGET_H

#include "decodedsamplemodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class SoundSpectrumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundSpectrumWidget(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;

public:
    void setCurrentPosition(float position);
    void setSamples(QList<DecodedSampleModel> samples);
    void setTimingsStartTime(QList<qint64> timingsStartTime);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float currentPosition;
    QList<DecodedSampleModel> samples;
    QList<qint64> timingsStartTime;

    qint64 samplesMinSecond;
    qint64 samplesMaxSecond;
    qint64 samplesDuration;
    qint16 samplesMaxAbsoluteData;
};

#endif // SOUNDSPECTRUMWIDGET_H
