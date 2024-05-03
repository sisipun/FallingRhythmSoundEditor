#include "musicxmlparser.h"

#include "musicxmlmodel.h"
#include "qdebug.h"

#include <QXmlStreamReader>

MusicXmlParser::MusicXmlParser()
{

}

MusicXmlModel MusicXmlParser::read(QString line)
{
    QXmlStreamReader reader(line);
    MusicXmlModel data;

    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (reader.name() == QString("part")) {
                data.addPart(readPart(reader));
            }
        }
    }

    if (reader.hasError()) {
        qDebug() << reader.errorString();
    }

    qDebug() << "Data";
    qDebug() << "Parts size: " << data.parts.size();
    for (const Part& part: data.parts) {
        qDebug() << "   Part";
        qDebug() << "   Measures size: " << part.measures.size();
        for (const Measure& measure: part.measures) {
            qDebug() << "       Measure";
            qDebug() << "       Divisions: " << measure.divisions;
            qDebug() << "       Beats: " << measure.beats;
            qDebug() << "       Beat Type: " << measure.beatType;
            qDebug() << "       Tempo: " << measure.tempo;
            qDebug() << "       Notes size : " << measure.notes.size();
            for (const Note& note: measure.notes) {
                qDebug() << "           Notes";
                qDebug() << "           Step: " << note.step;
                qDebug() << "           Alter: " << note.alter;
                qDebug() << "           Octave: " << note.octave;
                qDebug() << "           Duration: " << note.duration;
                qDebug() << "           Voice: " << note.voice;
                qDebug() << "           Time Modifiaction Normal: " << note.timeModifiactionNormal;
                qDebug() << "           Time Modifiaction Actual: " << note.timeModifiactionActual;
                qDebug() << "           Chord: " << note.chord;
            }
        }
    }

    return data;
}

Part MusicXmlParser::readPart(QXmlStreamReader& reader) const
{
    Part part;
    QXmlStreamReader::TokenType token;
    do {
        token = reader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (reader.name() == QString("measure")) {
                part.addMeasure(readMeasure(reader));
            }
        }
    } while (!(token == QXmlStreamReader::EndElement && reader.name() == QString("part")));
    return part;
}

Measure MusicXmlParser::readMeasure(QXmlStreamReader& reader) const
{
    Measure measure;
    QXmlStreamReader::TokenType token;
    do {
        token = reader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (reader.name() == QString("note")) {
                Note note = readNote(reader);
                if (!note.chord) {
                    measure.addNote(note);
                }
            } else if (reader.name() == QString("divisions")) {
                measure.divisions = reader.readElementText().toInt();
            } else if (reader.name() == QString("beats")) {
                measure.beats = reader.readElementText().toInt();
            } else if (reader.name() == QString("beat-type")) {
                measure.beatType = reader.readElementText().toInt();
            } else if (reader.name() == QString("sound") && reader.attributes().hasAttribute(QString("tempo"))) {
                measure.tempo = reader.attributes().value(QString("tempo")).toInt();
            }
        }
    } while (!(token == QXmlStreamReader::EndElement && reader.name() == QString("measure")));
    return measure;
}

Note MusicXmlParser::readNote(QXmlStreamReader& reader) const
{
    Note note;
    QXmlStreamReader::TokenType token;
    do {
        token = reader.readNext();
        if (reader.name() == QString("step")) {
            QString step = reader.readElementText();
            note.step = LAST_NOTE_STEP - step.data()[0].unicode();
        } else if (reader.name() == QString("alter")) {
            note.alter = reader.readElementText().toInt();
        } else if (reader.name() == QString("octave")) {
            note.octave = reader.readElementText().toInt();
        } else if (reader.name() == QString("duration")) {
            note.duration = reader.readElementText().toInt();
        } else if (reader.name() == QString("voice")) {
            note.voice = reader.readElementText().toInt();
        } else if (reader.name() == QString("normal-notes")) {
            note.timeModifiactionNormal = reader.readElementText().toInt();
        } else if (reader.name() == QString("actual-notes")) {
            note.timeModifiactionActual = reader.readElementText().toInt();
        }  else if (reader.name() == QString("chord")) {
            note.chord = true;
        }
    } while (!(token == QXmlStreamReader::EndElement && reader.name() == QString("note")));
    return note;
}
