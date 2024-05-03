#include "musicxmlsettingswidget.h"

#include <QtWidgets>

MusicXmlSettingsWidget::MusicXmlSettingsWidget(QWidget *parent)
    : QWidget{parent}
{
    QLabel* timingVoiceTitle = new QLabel(this);
    timingVoiceTitle->setAlignment(Qt::AlignCenter);
    timingVoiceTitle->setText(tr("Timing Voice"));

    QLabel* leftTimingVoiceTitle = new QLabel(this);
    leftTimingVoiceTitle->setAlignment(Qt::AlignCenter);
    leftTimingVoiceTitle->setText(tr("Left"));

    leftTimingVoiceSelect = new QComboBox(this);

    QLabel* rightTimingVoiceTitle = new QLabel(this);
    rightTimingVoiceTitle->setAlignment(Qt::AlignCenter);
    rightTimingVoiceTitle->setText(tr("Right"));

    rightTimingVoiceSelect = new QComboBox(this);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* timingVoiceTitleLayout = new QHBoxLayout;
    timingVoiceTitleLayout->addWidget(timingVoiceTitle);

    QBoxLayout* leftTimingVoiceSelectLayout = new QVBoxLayout;
    leftTimingVoiceSelectLayout->addWidget(leftTimingVoiceTitle);
    leftTimingVoiceSelectLayout->addWidget(leftTimingVoiceSelect);

    QBoxLayout* rightTimingVoiceSelectLayout = new QVBoxLayout;
    rightTimingVoiceSelectLayout->addWidget(rightTimingVoiceTitle);
    rightTimingVoiceSelectLayout->addWidget(rightTimingVoiceSelect);

    QBoxLayout* timingVoiceSelectsLayout = new QHBoxLayout;
    timingVoiceSelectsLayout->addLayout(leftTimingVoiceSelectLayout);
    timingVoiceSelectsLayout->addLayout(rightTimingVoiceSelectLayout);

    layout->addLayout(timingVoiceTitleLayout);
    layout->addLayout(timingVoiceSelectsLayout);

    setLayout(layout);
}


void MusicXmlSettingsWidget::onImported(QString path, MusicXmlModel data)
{
    leftTimingVoiceSelect->clear();
    rightTimingVoiceSelect->clear();

    QSet<QString> voices;
    voices.insert(QString::number(UNINITIALIZED_VALUE));
    for (const Part& part: data.parts) {
        for (const Measure& measure: part.measures) {
            for (const Note& note: measure.notes) {
                voices.insert(QString::number(note.voice));
            }
        }
    }

    QList<QString> voiceValues = voices.values();
    voiceValues.sort();
    leftTimingVoiceSelect->addItems(voiceValues);
    rightTimingVoiceSelect->addItems(voiceValues);
}

qint64 MusicXmlSettingsWidget::getLeftVoice() const
{
    return leftTimingVoiceSelect->currentText().toInt();
}

qint64 MusicXmlSettingsWidget::getRightVoice() const
{
    return rightTimingVoiceSelect->currentText().toInt();
}
