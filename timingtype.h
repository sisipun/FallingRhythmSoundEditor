#ifndef TIMINGTYPE_H
#define TIMINGTYPE_H

#include <QMap>
#include <QString>

enum class TimingType {
    PICKUP,
    PICKUP_LINE
};

const QMap<TimingType, QString> TIMING_TYPE_TO_STRING({
    std::make_pair(TimingType::PICKUP, "PICKUP"),
    std::make_pair(TimingType::PICKUP_LINE, "PICKUP_LINE")
});
const QMap<QString, TimingType> STRING_TO_TIMING_TYPE({
    std::make_pair("PICKUP", TimingType::PICKUP),
    std::make_pair("PICKUP_LINE", TimingType::PICKUP_LINE)
});

#endif // TIMINGTYPE_H
