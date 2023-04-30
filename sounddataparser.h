#ifndef SOUNDDATAPARSER_H
#define SOUNDDATAPARSER_H

#include <QString>

QT_BEGIN_NAMESPACE
class SoundModel;
QT_END_NAMESPACE

class SoundDataParser {
public:
    SoundDataParser();

    QString write(const SoundModel& data);
    SoundModel read(QString line);
};

#endif // SOUNDDATAPARSER_H
