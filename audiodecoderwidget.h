#ifndef AUDIODECODERWIDGET_H
#define AUDIODECODERWIDGET_H

#include "decodedsamplemodel.h"
#include "timingmodel.h"

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class QAudioDecoder;
class QPushButton;
QT_END_NAMESPACE

class AudioDecoderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AudioDecoderWidget(PlayerWidget* player, QWidget *parent = nullptr);

signals:
    void decoded(QList<DecodedSampleModel> samples);
    void generated(QList<TimingModel> timings);

public slots:
    void onPlayerLoaded(bool loaded);

private slots:
    void onDecodeButtonClicked();
    void onGenerateTimingsButtonClicked();

    void onDecodedBufferReady();
    void onDecodeFinished();

private:
    PlayerWidget* player = nullptr;

    QAudioDecoder* audioDecoder = nullptr;

    QPushButton* decodeButton = nullptr;
    QPushButton* generateTimingsButton = nullptr;

    QList<DecodedSampleModel> decodedSamples;
};

#endif // AUDIODECODERWIDGET_H
