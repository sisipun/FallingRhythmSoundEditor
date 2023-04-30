#ifndef TIMINGSIDE_H
#define TIMINGSIDE_H

#include <QMap>
#include <QString>

enum class TimingSide {
    LEFT,
    RIGHT
};

const QMap<TimingSide, QString> TIMING_SIDE_TO_STRING({
    std::make_pair(TimingSide::LEFT, "LEFT"),
    std::make_pair(TimingSide::RIGHT, "RIGHT")
});
const QMap<QString, TimingSide> STRING_TO_TIMING_SIDE({
    std::make_pair("LEFT", TimingSide::LEFT),
    std::make_pair("RIGHT", TimingSide::RIGHT)
});

#endif // TIMINGSIDE_H
