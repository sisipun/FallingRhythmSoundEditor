#include "musicxmlmodel.h"

void Measure::addNote(Note note)
{
    notes.append(note);
}

void Part::addMeasure(Measure measure)
{
    if (!measures.isEmpty()) {
        const Measure& previousMeasure = measures[measures.size() - 1];
        measure.divisions = measure.divisions == UNINITIALIZED_VALUE ? previousMeasure.divisions : measure.divisions;
        measure.beats = measure.beats == UNINITIALIZED_VALUE ? previousMeasure.beats : measure.beats;
        measure.beatType = measure.beatType == UNINITIALIZED_VALUE ? previousMeasure.beatType : measure.beatType;
        measure.tempo = measure.tempo == UNINITIALIZED_VALUE ? previousMeasure.tempo : measure.tempo;
    } else {
        measure.tempo = measure.tempo == UNINITIALIZED_VALUE ? DEFAULT_TEMPO_VALUE : measure.tempo;
    }

    measures.append(measure);
}

void MusicXmlModel::addPart(Part part)
{
    parts.append(part);
}
