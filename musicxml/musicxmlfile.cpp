#include "musicxmlfile.h"

#include "musicxmlmodel.h".h"

#include <QFile>
#include <QTextStream>

MusicXmlFile::MusicXmlFile(QString filePath)
    : filePath(filePath)
{
}

void MusicXmlFile::write(const MusicXmlModel& data)
{
    QFile exportFile(filePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&exportFile);

    out << parser.write(data);
}

MusicXmlModel MusicXmlFile::read()
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

