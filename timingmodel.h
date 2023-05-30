#ifndef TIMINGMODEL_H
#define TIMINGMODEL_H

#include "timingtype.h"
#include "timingside.h"

#include <QtGlobal>

struct TimingModel {
    float startSecond;
    float endSecond;
    TimingType type;
    TimingSide side;
    float position;
};

#endif // TIMINGMODEL_H
