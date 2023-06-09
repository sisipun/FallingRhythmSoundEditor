#ifndef AUDIODECODERWIDGET_H
#define AUDIODECODERWIDGET_H

#include "decodedsamplemodel.h"
#include "generatedtimingmodel.h"

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class SoundSpectrumWidget;
class QAudioDecoder;
class QPushButton;
QT_END_NAMESPACE

class AudioDecoderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AudioDecoderWidget(PlayerWidget* player, QWidget *parent = nullptr);

signals:
    void generated(QList<GeneratedTimingModel> timings);

private slots:
    void onPlayerLoaded(bool loaded);

    void onDecodeButtonClicked();
    void onGenerateTimingsButtonClicked();

    void onDecodedBufferReady();
    void onDecodeFinished();

private:
    PlayerWidget* player = nullptr;
    SoundSpectrumWidget* soundSpectrumWidget = nullptr;

    QAudioDecoder* audioDecoder = nullptr;

    QPushButton* decodeButton = nullptr;
    QPushButton* generateTimingsButton = nullptr;

    QList<DecodedSampleModel> decodedSamples;
};

#endif // AUDIODECODERWIDGET_H
