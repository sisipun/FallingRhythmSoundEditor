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
class SoundSpectrumWidget;
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
    SoundSpectrumWidget* soundSpectrum = nullptr;
};
#endif // SOUNDEDITOR_H
