#ifndef DECODEDSAMPLEMODEL_H
#define DECODEDSAMPLEMODEL_H

#include <QtGlobal>

struct DecodedSampleModel {
    qint64 startTime;
    qint64 endTime;
    qint16 minData;
    qint16 maxData;
};

#endif // DECODEDSAMPLEMODEL_H
