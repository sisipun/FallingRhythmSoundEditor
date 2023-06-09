#ifndef SOUNDSPECTRUMWIDGET_H
#define SOUNDSPECTRUMWIDGET_H

#include "decodedsamplemodel.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class SoundSpectrumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundSpectrumWidget(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;

public:
    void setSamples(QList<DecodedSampleModel> samples);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<DecodedSampleModel> samples;
};

#endif // SOUNDSPECTRUMWIDGET_H
