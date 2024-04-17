#ifndef MUSICXMLPARSER_H
#define MUSICXMLPARSER_H

#include <QList>
#include <QString>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class MusicXmlModel;
QT_END_NAMESPACE

struct Note
{
    qint64 staff;
    qint64 duration;
};

struct Measure
{
    qint64 beats;
    qint64 beatType;
    QList<Note> notes;
};

struct Part
{
    QList<Measure> measures;
};

class MusicXmlParser
{
public:
    MusicXmlParser();

    MusicXmlModel read(QString line);

private:
    Part readPart(const QXmlStreamReader& reader);
    Measure readMeasure(const QXmlStreamReader& reader);
    Note readNode(const QXmlStreamReader& reader);
};

#endif // MUSICXMLPARSER_H
