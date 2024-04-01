#ifndef TIMINGWIDGET_H
#define TIMINGWIDGET_H

#include "timingmodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class SoundDataParser;
class TimingModel;
class TimingView;
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
    void onUpdateButtonClicked();
    void onRemoveButtonClicked();
    void onTimingsItemChanged(QListWidgetItem* item, QListWidgetItem *previous);
    void onTimingsDoubleClicked(QListWidgetItem* item);
    void onExportButtonClicked();
    void onImportButtonClicked();

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void addTiming(qint64 startSecond, qint64 endSecond, TimingSide timingSide);
    void updateTimings();

private:
    PlayerWidget* player;
    SoundDataParser* parser;

    TimingView* timingsView = nullptr;
    QSpinBox* timingLengthInput = nullptr;
    QComboBox* timingTypeSelect = nullptr;
    QComboBox* timingSideSelect = nullptr;
    QSlider* positionInput = nullptr;
    QCheckBox* positionGenerateInput = nullptr;
    QPushButton* addButton = nullptr;
    QPushButton* removeButton = nullptr;
    QPushButton* exportButton = nullptr;
    QPushButton* importButton = nullptr;

    QMap<qint64, TimingModel> timings;
    QMap<TimingSide, qint64> startSeconds;

    static qint64 POSITION_RANGE;
    static qint64 MAX_PICKUP_LENGTH;
};

#endif // TIMINGWIDGET_H
