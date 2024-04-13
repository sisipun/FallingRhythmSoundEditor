#ifndef MUSICXMLPARSER_H
#define MUSICXMLPARSER_H

#include <QString>

QT_BEGIN_NAMESPACE
class MusicXmlModel;
QT_END_NAMESPACE

class MusicXmlParser
{
public:
    MusicXmlParser();

    QString write(const MusicXmlModel& data);
    MusicXmlModel read(QString line);
};

#endif // MUSICXMLPARSER_H
