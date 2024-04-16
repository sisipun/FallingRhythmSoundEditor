#include "musicxmlfile.h"

#include "musicxmlmodel.h".h"

#include <QFile>
#include <QTextStream>

MusicXmlFile::MusicXmlFile(QString filePath)
    : filePath(filePath)
{
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

    return parser.read(in.readAll());
}

