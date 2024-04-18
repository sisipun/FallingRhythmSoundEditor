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
    void onGenerateButtonClicked();

private:
    QPushButton* decodeButton = nullptr;
    QPushButton* generateButton = nullptr;
};

#endif // MUSICXMLDECODERWIDGET_H
