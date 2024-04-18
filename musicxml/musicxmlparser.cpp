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
                data.parts.append(readPart(reader));
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
                qDebug() << "           Duration: " << note.duration;
                qDebug() << "           Staff: " << note.staff;
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
                part.measures.append(readMeasure(reader));
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
                measure.notes.append(readNote(reader));
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
        if (reader.name() == QString("duration")) {
            note.duration = reader.readElementText().toInt();
        } else if (reader.name() == QString("staff")) {
            note.staff = reader.readElementText().toInt();
        }
    } while (!(token == QXmlStreamReader::EndElement && reader.name() == QString("note")));
    return note;
}
