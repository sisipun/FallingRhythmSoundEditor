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
    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        // qDebug() << reader.name() << " " << token;
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


    if(reader.hasError()) {
        qDebug() << reader.errorString();
    }

    MusicXmlModel data;
    return data;
}
