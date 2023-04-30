#include "sounddatafile.h"

#include "soundmodel.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

SoundDataFile::SoundDataFile(QString filePath)
    : filePath(filePath)
{
}

void SoundDataFile::write(SoundModel sound)
{
    QFile exportFile(filePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&exportFile);

    SoundDataParser parser;
    out << parser.write(sound);
}

SoundModel SoundDataFile::read()
{
    QFile importFile(filePath);
    if (!importFile.open(QIODevice::ReadOnly)) {
        return {};
    }

    QTextStream in(&importFile);
    if (in.atEnd()) {
        return {};
    }

    return parser.read(in.readLine());
}
