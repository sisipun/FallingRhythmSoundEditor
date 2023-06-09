#include "sounddataparser.h"

#include "timingside.h"
#include "timingtype.h"
#include "soundmodel.h"
#include "timingmodel.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SoundDataParser::SoundDataParser() { }

QString SoundDataParser::write(const SoundModel& data)
{
    QJsonObject jsonResult;
    QJsonArray jsonTimings;

    for (const TimingModel& timing : data.timings) {
        QJsonObject jsonTiming;
        jsonTiming.insert("startSecond", timing.startTime / 1000.0f);
        jsonTiming.insert("endSecond", timing.endTime / 1000.0f);
        jsonTiming.insert("side", TIMING_SIDE_TO_STRING[timing.side]);
        jsonTiming.insert("type", TIMING_TYPE_TO_STRING[timing.type]);
        jsonTiming.insert("position", timing.position);
        jsonTimings.append(jsonTiming);
    }

    jsonResult.insert("soundName", data.soundName);
    jsonResult.insert("soundFilePath", data.soundFilePath);
    jsonResult.insert("timings", jsonTimings);

    QJsonDocument jsonDocument(jsonResult);
    return QString(jsonDocument.toJson(QJsonDocument::Compact));
}

SoundModel SoundDataParser::read(QString line)
{
    SoundModel data;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(line.toUtf8());
    QJsonObject jsonResult = jsonDocument.object();
    QJsonArray jsonTimings = jsonResult.find("timings")->toArray();

    data.soundFilePath = jsonResult.find("soundFilePath")->toString();

    for (const QJsonValueRef& jsonTimingsElement : jsonTimings) {
        QJsonObject jsonTiming = jsonTimingsElement.toObject();
        TimingModel timing {
            int(jsonTiming.find("startSecond")->toDouble() * 1000),
            int(jsonTiming.find("endSecond")->toDouble() * 1000),
            STRING_TO_TIMING_TYPE[jsonTiming.find("type")->toString()],
            STRING_TO_TIMING_SIDE[jsonTiming.find("side")->toString()],
            float(jsonTiming.find("position")->toDouble()),
        };
        data.timings.append(timing);
    }

    return data;
}
