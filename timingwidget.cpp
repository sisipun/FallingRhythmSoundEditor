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

TimingWidget::TimingWidget(PlayerWidget* player, QWidget* parent)
    : QWidget { parent }
{
    this->player = player;
    connect(this->player, &PlayerWidget::loaded, this, &TimingWidget::onPlayerLoaded);
    connect(this->player, &PlayerWidget::positionChanged, this, &TimingWidget::onPositionChanged);

    this->parser = new SoundDataParser();

    timingsView = new TimingView(this);
    timingsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(timingsView, &TimingView::itemDoubleClicked, this, &TimingWidget::onTmingsDoubleClocked);

    timingLengthInput = new QDoubleSpinBox(this);
    timingLengthInput->setRange(0.01, 9999.99);
    timingLengthInput->setValue(1.0);
    timingLengthInput->setDisabled(true);

    timingSideSelect = new QComboBox(this);
    timingSideSelect->addItems(QStringList(STRING_TO_TIMING_SIDE.keys()));
    timingSideSelect->setCurrentIndex(0);
    timingSideSelect->setDisabled(true);

    timingTypeSelect = new QComboBox(this);
    timingTypeSelect->addItems(QStringList(STRING_TO_TIMING_TYPE.keys()));
    timingTypeSelect->setCurrentIndex(0);
    timingTypeSelect->setDisabled(true);

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
    connect(
        exportButton, &QPushButton::clicked, this, &TimingWidget::onExportButtonClicked);

    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &TimingWidget::onImportButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* listLayout = new QHBoxLayout;
    listLayout->addWidget(timingsView);

    QBoxLayout* timingsActionsLayout = new QHBoxLayout;
    timingsActionsLayout->addWidget(timingLengthInput, 1);
    timingsActionsLayout->addWidget(timingSideSelect, 1);
    timingsActionsLayout->addWidget(timingTypeSelect, 1);
    timingsActionsLayout->addWidget(addButton, 1);
    timingsActionsLayout->addWidget(removeButton, 1);

    QBoxLayout* parsingActionsLayout = new QHBoxLayout;
    parsingActionsLayout->addWidget(exportButton, 1);
    parsingActionsLayout->addWidget(importButton, 1);

    layout->addLayout(listLayout);
    layout->addLayout(timingsActionsLayout);
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
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    timings.clear();
    timingLengthInput->setValue(1.0);
    reloadTimingsView();
}

void TimingWidget::onPositionChanged(float position)
{
    int currentRow = 0;
    QList<float> timingsKeys = timings.keys();
    for (float key : timingsKeys) {
        const TimingModel& timing = timings[key];
        if (position > timing.startSecond && position < timing.endSecond) {
            timingsView->setCurrentRow(currentRow);
            timingsView->scrollToItem(timingsView->currentItem(), QAbstractItemView::ScrollHint::PositionAtCenter);
            break;
        }

        currentRow++;
    }
}

void TimingWidget::onAddButtonClicked()
{
    float startSecond = player->getPosition();
    float timingLength = timingLengthInput->value();
    QString timingType = timingTypeSelect->currentText();
    QString timingSide = timingSideSelect->currentText();
    timings[startSecond] = { startSecond, startSecond + timingLength, STRING_TO_TIMING_TYPE[timingType], STRING_TO_TIMING_SIDE[timingSide] };
    reloadTimingsView();
}

void TimingWidget::onUpdateButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        TimingModel& timing = timings[item->data(Qt::UserRole).toFloat()];
        float timingLength = timingLengthInput->value();
        QString timingType = timingTypeSelect->currentText();
        QString timingSide = timingSideSelect->currentText();
        timing.endSecond = timing.startSecond + timingLength;
        timing.type = STRING_TO_TIMING_TYPE[timingType];
        timing.side = STRING_TO_TIMING_SIDE[timingSide];
    }

    reloadTimingsView();
}

void TimingWidget::onRemoveButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    reloadTimingsView();
}

void TimingWidget::onTmingsDoubleClocked(QListWidgetItem* item)
{
    player->setPosition(item->data(Qt::UserRole).toFloat());
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
            timings[timing.startSecond] = timing;
        }

        reloadTimingsView();
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

void TimingWidget::reloadTimingsView()
{
    timingsView->clear();
    QList<float> timingsKeys = timings.keys();
    float lastEndSecond = 0.0;
    for (float seconds : timingsKeys) {
        const TimingModel& timing = timings[seconds];
        float distnaceFromPrevious = timing.startSecond - lastEndSecond;
        QString title = QString("Side: %1, Type: %2, Timing: [%3 - %4], Distance from previous: %5")
                            .arg(TIMING_SIDE_TO_STRING[timing.side])
                            .arg(TIMING_TYPE_TO_STRING[timing.type])
                            .arg(timing.startSecond)
                            .arg(timing.endSecond)
                            .arg(distnaceFromPrevious);
        QListWidgetItem* item = new QListWidgetItem(title, timingsView);
        if (distnaceFromPrevious < 0.0) {
            item->setBackground(Qt::red);
        } else if (distnaceFromPrevious < 0.3) {
            item->setBackground(Qt::darkYellow);
        }

        item->setData(Qt::UserRole, seconds);
        lastEndSecond = timing.endSecond;
    }
}
