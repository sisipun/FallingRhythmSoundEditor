#ifndef TIMINGVIEW_H
#define TIMINGVIEW_H

#include <QListWidget>

class TimingView : public QListWidget {
public:
    explicit TimingView(QWidget* parent = nullptr);

private slots:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // TIMINGVIEW_H
