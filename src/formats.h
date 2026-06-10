#ifndef FORMATS_H
#define FORMATS_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <QDir>
#include <Magick++.h>
#include <string>
#include <iostream>

using namespace Magick;
using namespace std;

class Formats : public QObject
{
    Q_OBJECT
public:
    static void loadFormats();

    static QString readableFiltersString();
    static QStringList readableFilters();
    static QStringList readableFormattedFilters();
    static QStringList writableFilters();
    static QStringList nativeReadableFormats();
    static bool isNativeReadable(QString fileName);
    static void printSupportedFormats();

private:
    static QString s_readableFiltersString;
    static QStringList s_readableFilters;
    static QStringList s_readableFormattedFilters;
    static QStringList s_writableFilters;
    static QStringList sortNonCaseSensitive(QStringList list);
    static bool isVideo(list<CoderInfo>::iterator entry);

signals:

public slots:

};

#endif // FORMATS_H
