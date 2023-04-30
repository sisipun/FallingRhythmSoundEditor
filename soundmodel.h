#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

#include "timingmodel.h"

#include <QList>
#include <QString>

struct SoundModel {
    QString soundName;
    QString soundFilePath;
    QList<TimingModel> timings;
};

#endif // SOUNDMODEL_H
