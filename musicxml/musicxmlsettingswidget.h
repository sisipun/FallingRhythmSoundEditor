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
    qint64 getLeftStaff() const;
    qint64 getRightStaff() const;

private:
    QComboBox* leftTimingStaffSelect = nullptr;
    QComboBox* rightTimingStaffSelect = nullptr;

};

#endif // MUSICXMLSETTINGSWIDGET_H
