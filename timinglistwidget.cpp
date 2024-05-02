#include "timinglistwidget.h"

#include <QtWidgets>

TimingListWidget::TimingListWidget(TimingSide side, QWidget* parent)
    : QListWidget { parent },
    side(side)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this, &TimingListWidget::itemDoubleClicked, this, &TimingListWidget::onDoubleClicked);
    connect(this, &TimingListWidget::currentItemChanged, this, &TimingListWidget::onItemChanged);
}

void TimingListWidget::setCurrentRow(qint64 currentTime)
{
    int currentRow = 0;
    QList<qint64> timingsKeys = timings.keys();
    for (qint64 key : timingsKeys) {
        const TimingModel& timing = timings[key];
        if (currentTime > timing.startTime) {
            setCurrentRow(currentRow);
            scrollToItem(currentItem(), QAbstractItemView::ScrollHint::PositionAtCenter);
        }
        else {
            break;
        }

        currentRow++;
    }
}

void TimingListWidget::set(QList<TimingModel> timings)
{
    this->timings.clear();
    for (const TimingModel& timing : timings) {
        this->timings[timing.startTime] = timing;
    }
    update();
}

void TimingListWidget::add(qint64 startSecond, qint64 endSecond, TimingType type, float position)
{
    timings[startSecond] = { startSecond, endSecond, type, side, position };
    update();
}

void TimingListWidget::removeSelected()
{
    for (QListWidgetItem* item : selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toInt());
    }
    update();
}

QList<TimingModel> TimingListWidget::getTimings() const
{
    return timings.values();
}

void TimingListWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        QWidget::keyPressEvent(event);
    } else {
        QListWidget::keyPressEvent(event);
    }
}

void TimingListWidget::onItemChanged(QListWidgetItem* item, QListWidgetItem *previous)
{
    if (currentRow() >= 0) {
        const TimingModel& timing = timings[item->data(Qt::UserRole).toInt()];
        emit timingChanged(timing);
    }
}

void TimingListWidget::onDoubleClicked(QListWidgetItem* item)
{
    if (currentRow() >= 0) {
        const TimingModel& timing = timings[item->data(Qt::UserRole).toInt()];
        emit timingDoubleClicked(timing);
    }
}

void TimingListWidget::update()
{
    clear();

    QList<qint64> timingsKeys = timings.keys();
    for (qint64 seconds : timingsKeys) {
        const TimingModel& timing = timings[seconds];
        QString title = QString("Type: %1, Timing: [%2 - %3], Position: %4")
                            .arg(TIMING_TYPE_TO_STRING[timing.type])
                            .arg(timing.startTime)
                            .arg(timing.endTime)
                            .arg(timing.position);
        QListWidgetItem* item = new QListWidgetItem(title, this);
        item->setData(Qt::UserRole, seconds);
    }
}
