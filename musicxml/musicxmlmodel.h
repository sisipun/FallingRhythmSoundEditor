#ifndef MUSICXMLMODEL_H
#define MUSICXMLMODEL_H

#include <QList>

const qint64 UNINITIALIZED_VALUE = -1;
const qint64 DEFAULT_TEMPO_VALUE = 120;
const qint64 LAST_NOTE_STEP = 'G';
const qint64 MAX_NOTE_STEP = 'G' - 'A';

struct Note
{
public:
    qint64 step = UNINITIALIZED_VALUE;
    qint64 alter = 0;
    qint64 octave = UNINITIALIZED_VALUE;
    qint64 voice = UNINITIALIZED_VALUE;
    qint64 duration = UNINITIALIZED_VALUE;
    bool chord = false;
};

struct Measure
{
public:
    void addNote(Note note);

public:
    qint64 divisions = UNINITIALIZED_VALUE;
    qint64 beats = UNINITIALIZED_VALUE;
    qint64 beatType = UNINITIALIZED_VALUE;
    qint64 tempo = UNINITIALIZED_VALUE;
    qint64 ending = UNINITIALIZED_VALUE;
    bool startRepeat = false;
    bool endRepeat = false;
    QList<Note> notes;
};

struct Part
{
public:
    void addMeasure(Measure measure);

public:
    QList<Measure> measures;
};

struct MusicXmlModel
{
public:
    void addPart(Part part);

public:
    QList<Part> parts;
};

#endif // MUSICXMLMODEL_H
