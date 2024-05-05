#ifndef MUSICXMLSETTINGSWIDGET_H
#define MUSICXMLSETTINGSWIDGET_H

#include "musicxmlmodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

class MusicXmlSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicXmlSettingsWidget(QWidget *parent = nullptr);

public slots:
    void onImported(QString path, MusicXmlModel data);

public:
    QString getPart() const;
    qint64 getLeftVoice() const;
    qint64 getRightVoice() const;

private:
    QComboBox* partSelect = nullptr;
    QComboBox* leftTimingVoiceSelect = nullptr;
    QComboBox* rightTimingVoiceSelect = nullptr;

};

#endif // MUSICXMLSETTINGSWIDGET_H
