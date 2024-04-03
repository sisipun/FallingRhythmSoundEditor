#ifndef SOUNDEDITOR_H
#define SOUNDEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SoundEditor;
}
class PlayerWidget;
class AudioDecoderWidget;
class TimingWidget;
class SoundSpectrumArea;
QT_END_NAMESPACE

class SoundEditor : public QMainWindow {
    Q_OBJECT

public:
    SoundEditor(QWidget* parent = nullptr);
    ~SoundEditor();

private slots:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::SoundEditor* ui;
    PlayerWidget* player = nullptr;
    AudioDecoderWidget* audioDecoder = nullptr;
    TimingWidget* timing = nullptr;
    SoundSpectrumArea* soundSpectrum = nullptr;
};
#endif // SOUNDEDITOR_H
