#include "musicxmlwidget.h"

#include "musicxmlfile.h"
#include "musicxmlmodel.h"
#include "musicxmlsettingswidget.h"

#include <QtWidgets>

MusicXmlWidget::MusicXmlWidget(QWidget *parent)
    : QWidget{parent}
{
    title = new QLabel(this);
    title->setAlignment(Qt::AlignCenter);
    title->setText(tr("MusicXML"));

    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &MusicXmlWidget::onImportButtonClicked);

    generateButton = new QPushButton(this);
    generateButton->setText(tr("Generate"));
    generateButton->setDisabled(true);
    connect(generateButton, &QPushButton::clicked, this, &MusicXmlWidget::onGenerateButtonClicked);

    settings = new MusicXmlSettingsWidget(this);
    settings->setVisible(false);
    connect(this, &MusicXmlWidget::imported, settings, &MusicXmlSettingsWidget::onImported);

    connect(this, &MusicXmlWidget::imported, this, &MusicXmlWidget::onImported);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(importButton);
    actionsLayout->addWidget(generateButton);

    layout->addLayout(titleLayout);
    layout->addLayout(actionsLayout);
    layout->addWidget(settings);

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
    settings->setVisible(true);
}

void MusicXmlWidget::onGenerateButtonClicked()
{
    QList<TimingModel> timings;

    qint64 leftVoice = settings->getLeftVoice();
    qint64 rightVoice = settings->getRightVoice();
    qint64 leftDuration = 0;
    qint64 rightDuration = 0;
    for (const Part& part: data.parts) {
        for (const Measure& measure: part.measures) {
            float quaterPerSecond = measure.tempo * measure.divisions / 60.0f;
            for (const Note& note: measure.notes) {
                float modifiedQuaterPerSecond = quaterPerSecond;
                if (note.voice == leftVoice) {
                    qint64 endLeftDuration = leftDuration + note.duration;
                    if (note.step != UNINITIALIZED_VALUE) {
                        timings.append({
                            qint64(leftDuration * 1000 / modifiedQuaterPerSecond),
                            qint64(endLeftDuration * 1000 / modifiedQuaterPerSecond),
                            note.duration > measure.divisions ? TimingType::PICKUP_LINE : TimingType::PICKUP,
                            TimingSide::LEFT,
                            2.0f * note.step / MAX_NOTE_STEP - 1
                        });
                    }
                    leftDuration = endLeftDuration;
                }

                if (note.voice == rightVoice) {
                    qint64 endRightDuration = rightDuration + note.duration;
                    if (note.step != UNINITIALIZED_VALUE) {
                        timings.append({
                            qint64(rightDuration * 1000 / modifiedQuaterPerSecond) + 1,
                            qint64(endRightDuration * 1000 / modifiedQuaterPerSecond) + 1, // TODO remove
                            note.duration > measure.divisions ? TimingType::PICKUP_LINE : TimingType::PICKUP,
                            TimingSide::RIGHT,
                            -(2.0f * note.step / MAX_NOTE_STEP - 1)
                        });
                    }
                    rightDuration = endRightDuration;
                }
            }
        }
    }

    qDebug() << timings.size();
    emit generated(timings);
}
