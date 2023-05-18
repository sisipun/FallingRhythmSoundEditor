#ifndef AUDIODECODERWIDGET_H
#define AUDIODECODERWIDGET_H

#include "decodedsamplemodel.h"

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

private slots:
    void onPlayerLoaded(bool loaded);

    void onDecodeButtonClicked();

    void onDecodedBufferReady();
    void onDecodeFinished();

private:
    PlayerWidget* player;

    QAudioDecoder* audioDecoder = nullptr;

    QPushButton* decodeButton = nullptr;

    QList<DecodedSampleModel> decodedSamples;
};

#endif // AUDIODECODERWIDGET_H
