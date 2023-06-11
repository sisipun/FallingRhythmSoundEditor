#include "timingwidget.h"

#include "timingside.h"
#include "timingtype.h"
#include "playerwidget.h"
#include "sounddatafile.h"
#include "sounddataparser.h"
#include "soundmodel.h"
#include "timingmodel.h"
#include "timingview.h"

#include <QtWidgets>

float TimingWidget::POSITION_RANGE = 100;

TimingWidget::TimingWidget(PlayerWidget* player, QWidget* parent): QWidget { parent }
{
    this->player = player;
    this->parser = new SoundDataParser();

    timingsView = new TimingView(this);
    timingsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(timingsView, &TimingView::itemDoubleClicked, this, &TimingWidget::onTimingsDoubleClicked);
    connect(timingsView, &TimingView::currentItemChanged, this, &TimingWidget::onTimingsItemChanged);

    timingLengthInput = new QSpinBox(this);
    timingLengthInput->setRange(1, 999999);
    timingLengthInput->setValue(1000);
    timingLengthInput->setDisabled(true);

    timingSideSelect = new QComboBox(this);
    timingSideSelect->addItems(QStringList(STRING_TO_TIMING_SIDE.keys()));
    timingSideSelect->setCurrentIndex(0);
    timingSideSelect->setDisabled(true);

    timingTypeSelect = new QComboBox(this);
    timingTypeSelect->addItems(QStringList(STRING_TO_TIMING_TYPE.keys()));
    timingTypeSelect->setCurrentIndex(0);
    timingTypeSelect->setDisabled(true);

    positionInput = new QSlider(Qt::Horizontal, this);
    positionInput->setRange(-POSITION_RANGE, POSITION_RANGE);
    positionInput->setDisabled(true);

    addButton = new QPushButton(this);
    addButton->setText(tr("+"));
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this, &TimingWidget::onAddButtonClicked);

    removeButton = new QPushButton(this);
    removeButton->setText(tr("-"));
    removeButton->setDisabled(true);
    connect(removeButton, &QPushButton::clicked, this, &TimingWidget::onRemoveButtonClicked);

    exportButton = new QPushButton(this);
    exportButton->setText(tr("Export"));
    exportButton->setDisabled(true);
    connect(exportButton, &QPushButton::clicked, this, &TimingWidget::onExportButtonClicked);

    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &TimingWidget::onImportButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* listLayout = new QHBoxLayout;
    listLayout->addWidget(timingsView);

    QBoxLayout* timingsActionsLayout = new QHBoxLayout;
    timingsActionsLayout->addWidget(addButton, 1);
    timingsActionsLayout->addWidget(removeButton, 1);

    QBoxLayout* timingsInfoActionsLayout = new QHBoxLayout;
    timingsInfoActionsLayout->addWidget(timingLengthInput, 1);
    timingsInfoActionsLayout->addWidget(timingSideSelect, 1);
    timingsInfoActionsLayout->addWidget(timingTypeSelect, 1);
    timingsInfoActionsLayout->addWidget(positionInput, 1);

    QBoxLayout* parsingActionsLayout = new QHBoxLayout;
    parsingActionsLayout->addWidget(exportButton, 1);
    parsingActionsLayout->addWidget(importButton, 1);

    layout->addLayout(listLayout);
    layout->addLayout(timingsActionsLayout);
    layout->addLayout(timingsInfoActionsLayout);
    layout->addLayout(parsingActionsLayout);

    setLayout(layout);
}

TimingWidget::~TimingWidget()
{
    delete parser;
}

void TimingWidget::onPlayerLoaded(bool loaded)
{
    timingLengthInput->setDisabled(!loaded);
    timingSideSelect->setDisabled(!loaded);
    timingTypeSelect->setDisabled(!loaded);
    positionInput->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    timings.clear();
    timingLengthInput->setValue(1000);
    updateTimings();
}

void TimingWidget::onPlayerPositionChanged(qint64 position)
{
    int currentRow = 0;
    QList<qint64> timingsKeys = timings.keys();
    for (qint64 key : timingsKeys) {
        const TimingModel& timing = timings[key];
        if (position > timing.startTime) {
            timingsView->setCurrentRow(currentRow);
            timingsView->scrollToItem(timingsView->currentItem(), QAbstractItemView::ScrollHint::PositionAtCenter);
        }
        else {
            break;
        }

        currentRow++;
    }
}

void TimingWidget::onAudioDecoderGenerated(QList<TimingModel> generatedTimings)
{
    for (const TimingModel& timing : generatedTimings) {
        timings[timing.startTime] = {timing.startTime, timing.endTime, timing.type, timing.side, timing.position};
    }
    updateTimings();
}

void TimingWidget::onAddButtonClicked()
{
    qint64 startSecond = player->getPosition();
    qint64 timingLength = timingLengthInput->value();
    QString timingType = timingTypeSelect->currentText();
    QString timingSide = timingSideSelect->currentText();
    float position = positionInput->value() / POSITION_RANGE;
    timings[startSecond] = { startSecond, startSecond + timingLength, STRING_TO_TIMING_TYPE[timingType], STRING_TO_TIMING_SIDE[timingSide], position };
    updateTimings();
}

void TimingWidget::onUpdateButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        TimingModel& timing = timings[item->data(Qt::UserRole).toInt()];
        qint64 timingLength = timingLengthInput->value();
        QString timingType = timingTypeSelect->currentText();
        QString timingSide = timingSideSelect->currentText();
        float position = positionInput->value();
        timing.endTime = timing.startTime + timingLength;
        timing.type = STRING_TO_TIMING_TYPE[timingType];
        timing.side = STRING_TO_TIMING_SIDE[timingSide];
        timing.position = position / POSITION_RANGE;
    }

    updateTimings();
}

void TimingWidget::onRemoveButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toInt());
    }

    updateTimings();
}

void TimingWidget::onTimingsItemChanged(QListWidgetItem* item, QListWidgetItem *previous)
{
    if (timingsView->currentRow() >= 0) {
        TimingModel& timing = timings[item->data(Qt::UserRole).toInt()];
        timingLengthInput->setValue(timing.endTime - timing.startTime);
        timingSideSelect->setCurrentText(TIMING_SIDE_TO_STRING[timing.side]);
        timingTypeSelect->setCurrentText(TIMING_TYPE_TO_STRING[timing.type]);
        positionInput->setValue(timing.position * POSITION_RANGE);
    }
}

void TimingWidget::onTimingsDoubleClicked(QListWidgetItem* item)
{
    player->setPosition(item->data(Qt::UserRole).toInt());
}

void TimingWidget::onExportButtonClicked()
{
    player->pause();

    QString exportFilePath = QFileDialog::getSaveFileName(
        this, tr("Export file"), QString(), tr("Json Files (*.json)"));
    if (exportFilePath.isEmpty()) {
        return;
    }

    SoundDataFile file(exportFilePath);
    file.write({ "Default", player->getSoundName(), timings.values() });
}

void TimingWidget::onImportButtonClicked()
{
    player->pause();

    QString importFilePath = QFileDialog::getOpenFileName(
        this, tr("Import file"), QString(), tr("Json Files (*.json)"));
    if (importFilePath.isEmpty()) {
        return;
    }

    SoundDataFile file(importFilePath);
    SoundModel sound = file.read();

    if (!sound.soundFilePath.isEmpty()) {
        player->load(sound.soundFilePath);
        for (const TimingModel& timing : sound.timings) {
            timings[timing.startTime] = timing;
        }

        updateTimings();
        player->play();
    }
}

void TimingWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onUpdateButtonClicked();
    } else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        onRemoveButtonClicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void TimingWidget::updateTimings()
{
    timingsView->clear();
    QList<qint64> timingsKeys = timings.keys();
    for (qint64 seconds : timingsKeys) {
        const TimingModel& timing = timings[seconds];
        QString title = QString("Side: %1, Type: %2, Timing: [%3 - %4], Position: %5")
                            .arg(TIMING_SIDE_TO_STRING[timing.side])
                            .arg(TIMING_TYPE_TO_STRING[timing.type])
                            .arg(timing.startTime)
                            .arg(timing.endTime)
                            .arg(timing.position);
        QListWidgetItem* item = new QListWidgetItem(title, timingsView);
        item->setData(Qt::UserRole, seconds);
    }

    emit timingsChanged(timings.values());
}
