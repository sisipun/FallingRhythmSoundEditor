#ifndef TIMINGWIDGET_H
#define TIMINGWIDGET_H

#include "timingmodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class SoundDataParser;
class TimingListWidget;
class TimingModel;
enum class TimingType;
class QPushButton;
class QComboBox;
class QCheckBox;
class QListWidget;
class QListWidgetItem;
class QSpinBox;
class QSlider;
QT_END_NAMESPACE

class TimingWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimingWidget(PlayerWidget* player, QWidget* parent = nullptr);
    ~TimingWidget();

signals:
    void timingsChanged(QList<TimingModel> timings);

public slots:
    void onPlayerLoaded(bool loaded);
    void onPlayerPositionChanged(qint64 position);
    void onAudioDecoderGenerated(QList<TimingModel> generatedTimings);

private slots:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onTimingsItemChanged(const TimingModel& timing);
    void onTimingsDoubleClicked(const TimingModel& timing);
    void onExportButtonClicked();
    void onImportButtonClicked();

    void keyPressEvent(QKeyEvent* event) override;

private:
    void addTiming(const TimingModel& timing);

private:
    PlayerWidget* player;
    SoundDataParser* parser;

    TimingListWidget* leftTimingList = nullptr;
    TimingListWidget* rightTimingList = nullptr;
    QSpinBox* timingLengthInput = nullptr;
    QComboBox* timingTypeSelect = nullptr;
    QComboBox* timingSideSelect = nullptr;
    QSlider* positionInput = nullptr;
    QCheckBox* positionGenerateInput = nullptr;
    QPushButton* addButton = nullptr;
    QPushButton* removeButton = nullptr;
    QPushButton* exportButton = nullptr;
    QPushButton* importButton = nullptr;

    static qint64 POSITION_RANGE;
    static qint64 MAX_PICKUP_LENGTH;
};

#endif // TIMINGWIDGET_H
