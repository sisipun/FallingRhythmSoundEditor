#ifndef MUSICXMLMODEL_H
#define MUSICXMLMODEL_H

#include <QList>

class Note
{
public:
    qint64 staff = -1;
    qint64 duration = -1;
};

class Measure
{
    void addNote(Note note);
public:
    qint64 divisions = -1;
    qint64 beats = -1;
    qint64 beatType = -1;
    qint64 tempo = 120;
    QList<Note> notes;
};

class Part
{
    void addMeasure(Measure measure);
public:
    QList<Measure> measures;
};

class MusicXmlModel
{
public:
    void addPart(Part part);
public:
    QList<Part> parts;
};

#endif // MUSICXMLMODEL_H
