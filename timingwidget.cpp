#include "timingwidget.h"

#include "timingside.h"
#include "timingtype.h"
#include "playerwidget.h"
#include "sounddatafile.h"
#include "sounddataparser.h"
#include "soundmodel.h"
#include "timinglistwidget.h"
#include "timingmodel.h"

#include <QtWidgets>

qint64 TimingWidget::POSITION_RANGE = 100;
qint64 TimingWidget::MAX_PICKUP_LENGTH = 200;

TimingWidget::TimingWidget(PlayerWidget* player, QWidget* parent): QWidget { parent }
{
    this->player = player;
    this->parser = new SoundDataParser();

    leftTimingList = new TimingListWidget(TimingSide::LEFT, this);
    connect(leftTimingList, &TimingListWidget::timingDoubleClicked, this, &TimingWidget::onTimingsDoubleClicked);
    connect(leftTimingList, &TimingListWidget::timingChanged, this, &TimingWidget::onTimingsItemChanged);

    rightTimingList = new TimingListWidget(TimingSide::RIGHT, this);
    connect(rightTimingList, &TimingListWidget::timingDoubleClicked, this, &TimingWidget::onTimingsDoubleClicked);
    connect(rightTimingList, &TimingListWidget::timingChanged, this, &TimingWidget::onTimingsItemChanged);

    timingLengthInput = new QSpinBox(this);
    timingLengthInput->setRange(1, 999999);
    timingLengthInput->setValue(1);
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

    positionGenerateInput = new QCheckBox(this);
    positionGenerateInput->setChecked(false);
    positionGenerateInput->setDisabled(true);
    positionGenerateInput->setText("Generate");

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
    listLayout->addWidget(leftTimingList);
    listLayout->addWidget(rightTimingList);

    QBoxLayout* timingsActionsLayout = new QHBoxLayout;
    timingsActionsLayout->addWidget(addButton, 1);
    timingsActionsLayout->addWidget(removeButton, 1);

    QBoxLayout* timingsInfoActionsLayout = new QHBoxLayout;
    timingsInfoActionsLayout->addWidget(timingLengthInput, 2);
    timingsInfoActionsLayout->addWidget(timingSideSelect, 2);
    timingsInfoActionsLayout->addWidget(timingTypeSelect, 2);
    timingsInfoActionsLayout->addWidget(positionInput, 1);
    timingsInfoActionsLayout->addWidget(positionGenerateInput, 1);

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
    positionGenerateInput->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    leftTimingList->clearTimings();
    rightTimingList->clearTimings();
    timingLengthInput->setValue(1);
}

void TimingWidget::onPlayerPositionChanged(qint64 position)
{
    leftTimingList->setCurrentPosition(position);
    rightTimingList->setCurrentPosition(position);
}

void TimingWidget::onAudioDecoderGenerated(QList<TimingModel> generatedTimings)
{
    for (const TimingModel& timing : generatedTimings) {
        addTiming(timing);
    }
}

void TimingWidget::onAddButtonClicked()
{
    qint64 startSecond = player->getPosition();
    qint64 timingLength = timingLengthInput->value();
    QString timingSide = timingSideSelect->currentText();
    TimingType timingType = timingLength > MAX_PICKUP_LENGTH ? TimingType::PICKUP_LINE : TimingType::PICKUP;
    if (positionGenerateInput->isChecked()) {
        qint64 generatedPosition = QRandomGenerator::global()->bounded(-POSITION_RANGE, POSITION_RANGE);
        positionInput->setValue(generatedPosition);
    }
    float position = positionInput->value() / (POSITION_RANGE * 1.0f);
    addTiming({ startSecond, startSecond + timingLength, timingType, STRING_TO_TIMING_SIDE[timingSide], position });
}

void TimingWidget::onRemoveButtonClicked()
{
    leftTimingList->removeSelected();
    rightTimingList->removeSelected();
}

void TimingWidget::onTimingsItemChanged(const TimingModel& timing)
{
    timingLengthInput->setValue(timing.endTime - timing.startTime);
    timingSideSelect->setCurrentText(TIMING_SIDE_TO_STRING[timing.side]);
    timingTypeSelect->setCurrentText(TIMING_TYPE_TO_STRING[timing.type]);
    positionInput->setValue(timing.position * POSITION_RANGE);
}

void TimingWidget::onTimingsDoubleClicked(const TimingModel& timing)
{
    player->setPosition(timing.startTime);
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
    QList<TimingModel> timings;
    timings.append(leftTimingList->getTimings());
    timings.append(rightTimingList->getTimings());
    file.write({ "Default", player->getSoundName(), timings });
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
            addTiming(timing);
        }

        player->play();
    }
}

void TimingWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        onRemoveButtonClicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void TimingWidget::addTiming(const TimingModel& timing)
{
    if (timing.side == TimingSide::LEFT) {
        leftTimingList->add(timing.startTime, timing.endTime, timing.type, timing.position);
    } else if (timing.side == TimingSide::RIGHT) {
        rightTimingList->add(timing.startTime, timing.endTime, timing.type, timing.position);
    }
}
