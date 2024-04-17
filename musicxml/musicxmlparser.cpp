#include "musicxmlparser.h"

#include "musicxmlmodel.h"
#include "qdebug.h"

#include <QXmlStreamReader>

MusicXmlParser::MusicXmlParser()
{

}

MusicXmlModel MusicXmlParser::read(QString line)
{
    QList<QString> stack;
    QXmlStreamReader reader(line);

    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            qDebug() << reader.name();
        }
        if(token == QXmlStreamReader::EndElement) {
            qDebug() << "/" << reader.name();
        }
    }


    if (reader.hasError()) {
        qDebug() << reader.errorString();
    }

    MusicXmlModel data;
    return data;
}

Part MusicXmlParser::readPart(const QXmlStreamReader& reader)
{
    return {};
}

Measure MusicXmlParser::readMeasure(const QXmlStreamReader& reader)
{
    return {};
}

Note MusicXmlParser::readNode(const QXmlStreamReader& reader)
{
    return {};
}
