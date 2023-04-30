#ifndef SOUNDDATAFILE_H
#define SOUNDDATAFILE_H

#include "sounddataparser.h"

#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class SoundModel;
QT_END_NAMESPACE

class SoundDataFile {
public:
    SoundDataFile(QString filePath);

    void write(SoundModel sounds);
    SoundModel read();

private:
    QString filePath;
    SoundDataParser parser;
};

#endif // SOUNDDATAFILE_H
