
#ifndef SOUNDSPECTRUMAREA_H
#define SOUNDSPECTRUMAREA_H

#include "decodedsamplemodel.h"
#include "timingmodel.h"

#include <QScrollArea>
#include <QWidget>

QT_BEGIN_NAMESPACE
class SoundSpectrumWidget;
QT_END_NAMESPACE

class SoundSpectrumArea : public QScrollArea
{
    Q_OBJECT
public:
    SoundSpectrumArea(QWidget *parent = nullptr);

public slots:
    void onPlayerPositionChanged(float position);
    void onZoomChanged(float value);
    void onAudioDecoderDecoded(QList<DecodedSampleModel> samples);
    void onTimingTimingsChanged(QList<TimingModel> timings);

private:
    int getCurrentSample() const;

private:
    SoundSpectrumWidget* soundSpectrum = nullptr;

    float playerPosition;
    float zoomValue;
    QList<DecodedSampleModel> samples;
    QList<TimingModel> timings;
};

#endif // SOUNDSPECTRUMAREA_H
