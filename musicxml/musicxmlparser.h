      #ifndef MUSICXMLPARSER_H
#define MUSICXMLPARSER_H

#include <QList>
#include <QString>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class MusicXmlModel;
class Part;
class Measure;
class Note;
QT_END_NAMESPACE

class MusicXmlParser
{
public:
    MusicXmlParser();

    MusicXmlModel read(QString line);

private:
    Part readPart(QXmlStreamReader& reader) const;
    Measure readMeasure(QXmlStreamReader& reader) const;
    Note readNote(QXmlStreamReader& reader) const;
};

#endif // MUSICXMLPARSER_H
