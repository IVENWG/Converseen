#ifndef SIZEUTIL_H
#define SIZEUTIL_H

#include <QString>

class SizeUtil
{
public:
    SizeUtil();
    static QString simplifyFileSize(qint64 size);
};

#endif // SIZEUTIL_H
