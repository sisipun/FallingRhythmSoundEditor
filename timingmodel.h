#ifndef TIMINGMODEL_H
#define TIMINGMODEL_H

#include "timingtype.h"
#include "timingside.h"

#include <QtGlobal>

struct TimingModel {
    qint64 startTime;
    qint64 endTime;
    TimingType type;
    TimingSide side;
    float position;
};

#endif // TIMINGMODEL_H
