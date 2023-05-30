#ifndef GENERATEDTIMINGMODEL_H
#define GENERATEDTIMINGMODEL_H

#include "timingtype.h"
#include "timingside.h"

#include <QtGlobal>

struct GeneratedTimingModel {
    float startSecond;
    float endSecond;
    TimingType type;
    TimingSide side;
    float position;
};

#endif // GENERATEDTIMINGMODEL_H
