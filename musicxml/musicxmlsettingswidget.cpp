#include "musicxmlsettingswidget.h"

#include <QtWidgets>

MusicXmlSettingsWidget::MusicXmlSettingsWidget(QWidget *parent)
    : QWidget{parent}
{
    QLabel* timingStaffTitle = new QLabel(this);
    timingStaffTitle->setAlignment(Qt::AlignCenter);
    timingStaffTitle->setText(tr("Timing Staff"));

    QLabel* leftTimingStaffTitle = new QLabel(this);
    leftTimingStaffTitle->setAlignment(Qt::AlignCenter);
    leftTimingStaffTitle->setText(tr("Left"));

    leftTimingStaffSelect = new QComboBox(this);

    QLabel* rightTimingStaffTitle = new QLabel(this);
    rightTimingStaffTitle->setAlignment(Qt::AlignCenter);
    rightTimingStaffTitle->setText(tr("Right"));

    rightTimingStaffSelect = new QComboBox(this);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* timingStaffTitleLayout = new QHBoxLayout;
    timingStaffTitleLayout->addWidget(timingStaffTitle);

    QBoxLayout* leftTimingStaffSelectLayout = new QVBoxLayout;
    leftTimingStaffSelectLayout->addWidget(leftTimingStaffTitle);
    leftTimingStaffSelectLayout->addWidget(leftTimingStaffSelect);

    QBoxLayout* rightTimingStaffSelectLayout = new QVBoxLayout;
    rightTimingStaffSelectLayout->addWidget(rightTimingStaffTitle);
    rightTimingStaffSelectLayout->addWidget(rightTimingStaffSelect);

    QBoxLayout* timingStaffSelectsLayout = new QHBoxLayout;
    timingStaffSelectsLayout->addLayout(leftTimingStaffSelectLayout);
    timingStaffSelectsLayout->addLayout(rightTimingStaffSelectLayout);

    layout->addLayout(timingStaffTitleLayout);
    layout->addLayout(timingStaffSelectsLayout);

    setLayout(layout);
}


void MusicXmlSettingsWidget::onImported(QString path, MusicXmlModel data)
{
    leftTimingStaffSelect->clear();
    rightTimingStaffSelect->clear();

    QSet<QString> staffs;
    staffs.insert(QString::number(UNINITIALIZED_VALUE));
    for (const Part& part: data.parts) {
        for (const Measure& measure: part.measures) {
            for (const Note& note: measure.notes) {
                staffs.insert(QString::number(note.staff));
            }
        }
    }

    QList<QString> staffValues = staffs.values();
    staffValues.sort();
    leftTimingStaffSelect->addItems(staffValues);
    rightTimingStaffSelect->addItems(staffValues);
}

qint64 MusicXmlSettingsWidget::getLeftStaff() const
{
    return leftTimingStaffSelect->currentText().toInt();
}

qint64 MusicXmlSettingsWidget::getRightStaff() const
{
    return rightTimingStaffSelect->currentText().toInt();
}
