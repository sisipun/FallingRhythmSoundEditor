#ifndef MUSICXMLDECODERWIDGET_H
#define MUSICXMLDECODERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class MusicXmlDecoderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicXmlDecoderWidget(QWidget *parent = nullptr);

private slots:
    void onDecodeButtonClicked();

private:
    QPushButton* decodeButton = nullptr;

};

#endif // MUSICXMLDECODERWIDGET_H
