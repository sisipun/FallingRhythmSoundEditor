
#ifndef SOUNDSPECTRUMPAINTZONEWIDGET_H
#define SOUNDSPECTRUMPAINTZONEWIDGET_H


#include <QWidget>


class SoundSpectrumPaintZoneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundSpectrumPaintZoneWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // SOUNDSPECTRUMPAINTZONEWIDGET_H
