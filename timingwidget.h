#ifndef TIMINGWIDGET_H
#define TIMINGWIDGET_H

#include "generatedtimingmodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class AudioDecoderWidget;
class SoundDataParser;
class TimingModel;
class TimingView;
enum class TimingType;
class QPushButton;
class QComboBox;
class QListWidget;
class QListWidgetItem;
class QDoubleSpinBox;
class QSlider;
QT_END_NAMESPACE

class TimingWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimingWidget(PlayerWidget* player, AudioDecoderWidget* audioDecoder, QWidget* parent = nullptr);
    ~TimingWidget();

    void reloadTimingsView();

private slots:
    void onPlayerLoaded(bool loaded);
    void onPlayerPositionChanged(float position);
    void onAudioDecoderGenerated(QList<GeneratedTimingModel> generatedTimings);

    void onAddButtonClicked();
    void onUpdateButtonClicked();
    void onRemoveButtonClicked();
    void onTimingsItemChanged(QListWidgetItem* item, QListWidgetItem *previous);
    void onTimingsDoubleClicked(QListWidgetItem* item);
    void onExportButtonClicked();
    void onImportButtonClicked();

    void keyPressEvent(QKeyEvent* event) override;

private:
    PlayerWidget* player;
    AudioDecoderWidget* audioDecoder;
    SoundDataParser* parser;

    TimingView* timingsView = nullptr;
    QDoubleSpinBox* timingLengthInput = nullptr;
    QComboBox* timingTypeSelect = nullptr;
    QComboBox* timingSideSelect = nullptr;
    QSlider* positionInput = nullptr;
    QPushButton* addButton = nullptr;
    QPushButton* removeButton = nullptr;
    QPushButton* exportButton = nullptr;
    QPushButton* importButton = nullptr;

    QMap<float, TimingModel> timings;

    static float POSITION_RANGE;
};

#endif // TIMINGWIDGET_H
