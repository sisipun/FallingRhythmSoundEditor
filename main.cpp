#include "playerwidget.h"
#include "soundeditor.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SoundEditor w;
    w.show();

    return a.exec();
}
