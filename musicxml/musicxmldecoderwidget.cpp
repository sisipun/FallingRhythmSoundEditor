#include "musicxmldecoderwidget.h"

#include "musicxmlfile.h"
#include "musicxmlmodel.h"

#include <QtWidgets>

MusicXmlDecoderWidget::MusicXmlDecoderWidget(QWidget *parent)
    : QWidget{parent}
{
    decodeButton = new QPushButton(this);
    decodeButton->setText(tr("Decode"));
    connect(decodeButton, &QPushButton::clicked, this, &MusicXmlDecoderWidget::onDecodeButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(decodeButton, 1);

    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void MusicXmlDecoderWidget::onDecodeButtonClicked()
{
    QString importFilePath = QFileDialog::getOpenFileName(this, tr("Import file"), QString(), tr("Music XML (*.musicxml)"));
    if (importFilePath.isEmpty()) {
        return;
    }

    MusicXmlFile file(importFilePath);
    MusicXmlModel data = file.read();
}
