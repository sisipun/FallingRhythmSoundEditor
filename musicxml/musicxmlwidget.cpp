#include "musicxmlwidget.h"

#include "musicxmlfile.h"
#include "musicxmlmodel.h"

#include <QtWidgets>

MusicXmlWidget::MusicXmlWidget(QWidget *parent)
    : QWidget{parent}
{
    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &MusicXmlWidget::onImportButtonClicked);

    generateButton = new QPushButton(this);
    generateButton->setText(tr("Generate"));
    generateButton->setDisabled(true);
    connect(generateButton, &QPushButton::clicked, this, &MusicXmlWidget::onGenerateButtonClicked);

    title = new QLabel(this);
    title->setAlignment(Qt::AlignCenter);
    title->setText(tr("MusicXML"));

    connect(this, &MusicXmlWidget::imported, this, &MusicXmlWidget::onImported);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(importButton);
    actionsLayout->addWidget(generateButton);

    layout->addLayout(titleLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void MusicXmlWidget::onImportButtonClicked()
{
    QString importFilePath = QFileDialog::getOpenFileName(this, tr("Import file"), QString(), tr("Music XML (*.musicxml)"));
    if (importFilePath.isEmpty()) {
        return;
    }

    MusicXmlFile file(importFilePath);
    emit imported(importFilePath, file.read());
}

void MusicXmlWidget::onImported(QString path, MusicXmlModel data)
{
    this->data = data;

    title->setText(tr("MusicXML (%1)").arg(path));

    generateButton->setDisabled(false);
}

void MusicXmlWidget::onGenerateButtonClicked()
{
    QList<TimingModel> timings;

    QMap<qint64, qint64> durations {{1, 0}, {2, 0}};
    for (const Part& part: data.parts) {
        for (const Measure& measure: part.measures) {
            float quaterPerSecond = measure.tempo * measure.divisions / 60.0f;
            for (const Note& note: measure.notes) {
                if (!durations.contains(note.staff)) {
                    continue;
                }

                qint64 startDuration = durations[note.staff];
                qint64 endDuration = startDuration + note.duration;

                if (note.step != UNINITIALIZED_VALUE) {
                    TimingModel model;
                    qint64& currentDuration = durations[note.staff];
                    currentDuration += note.duration;

                    model.startTime = (startDuration * 1000 / quaterPerSecond);
                    model.endTime = (endDuration * 1000 / quaterPerSecond);
                    model.type = note.duration > measure.divisions ? TimingType::PICKUP_LINE : TimingType::PICKUP;
                    model.side = note.staff == 0 ? TimingSide::LEFT : TimingSide::RIGHT;
                    model.position = 2.0f * note.step / MAX_NOTE_STEP - 1;
                    timings.append(model);
                }

                durations[note.staff] = endDuration;
            }
        }
    }

    emit generated(timings);
}
