#ifndef TIMINGLISTWIDGET_H
#define TIMINGLISTWIDGET_H

#include "timingmodel.h"

#include <QListWidget>

class TimingListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit TimingListWidget(TimingSide side, QWidget* parent = nullptr);

signals:
    void timingChanged(const TimingModel& timing);
    void timingDoubleClicked(const TimingModel& timing);

public:
    void setCurrentRow(qint64 currentTime);
    void set(QList<TimingModel> timings);
    void add(qint64 startSecond, qint64 endSecond, TimingType type, float position);
    void removeSelected();
    QList<TimingModel> getTimings() const;

private slots:
    void keyPressEvent(QKeyEvent* event) override;
    void onItemChanged(QListWidgetItem* item, QListWidgetItem *previous);
    void onDoubleClicked(QListWidgetItem* item);

private:
    void update();

private:
    TimingSide side;
    QMap<qint64, TimingModel> timings;
    QMap<TimingSide, qint64> startSeconds;
};

#endif // TIMINGLISTWIDGET_H
