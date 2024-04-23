 #ifndef MUSICXMLFILE_H
#define MUSICXMLFILE_H

#include "musicxmlparser.h"

#include <QString>

QT_BEGIN_NAMESPACE
class MusicXmlModel;
QT_END_NAMESPACE

class MusicXmlFile
{
public:
    MusicXmlFile(QString filePath);

    MusicXmlModel read();

private:
    QString filePath;
    MusicXmlParser parser;
};

#endif // MUSICXMLFILE_H
