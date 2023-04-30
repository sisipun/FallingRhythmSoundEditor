#include "timingview.h"

#include <QtWidgets>

TimingView::TimingView(QWidget* parent)
    : QListWidget { parent }
{
}

void TimingView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        QWidget::keyPressEvent(event);
    } else {
        QListWidget::keyPressEvent(event);
    }
}
