#ifndef TIMINGMODEL_H
#define TIMINGMODEL_H

#include "timingtype.h"
#include "timingside.h"

#include <Qt>

struct TimingModel {
    float startSecond;
    float endSecond;
    TimingType type;
    TimingSide side;
};

#endif // TIMINGMODEL_H
