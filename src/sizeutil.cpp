#include "sizeutil.h"

QString SizeUtil::simplifyFileSize(qint64 size)
{
    QString sSize;

    sSize = QString("%1 Bytes").arg(size);
    if (size > 1024)
        sSize = QString("%1 KiB").arg(QString::number( ((double)size / 1024), 'f', 1) );
    if (size / 1024 > 1024)
        sSize = QString("%1 MiB").arg(QString::number( ((double)size / 1024 / 1024), 'f', 1) );

    return sSize;
}
