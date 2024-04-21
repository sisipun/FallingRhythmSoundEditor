#ifndef MUSICXMLWIDGET_H
#define MUSICXMLWIDGET_H

#include "musicxmlmodel.h"
#include "../timingmodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MusicXmlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicXmlWidget(QWidget *parent = nullptr);

signals:
    void imported(QString path, MusicXmlModel data);
    void generated(QList<TimingModel> timings);

private slots:
    void onImportButtonClicked();
    void onImported(QString path, MusicXmlModel data);
    void onGenerateButtonClicked();

private:
    QLabel* title = nullptr;
    QPushButton* importButton = nullptr;
    QPushButton* generateButton = nullptr;

    MusicXmlModel data;
};

#endif // MUSICXMLWIDGET_H
