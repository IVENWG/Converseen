#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <Magick++.h>
#include <string>
#include <iostream>

using namespace Magick;
using namespace std;

#if MagickLibVersion < 0x700
    typedef FilterTypes IMFilterType;
#else
    typedef FilterType IMFilterType;
#endif

namespace globals
{
    const QString PROGRAM_NAME = "LessMB";
    const int CURRENT_INTERNAL_VERSION = 1000;
    const QString VERSION = "1.0.0";

    class Globals
    {
    public:
        static void setSignature(const QString &signature);
        static QString signature();
        static QString magickVersion();
        static qreal scaleFactor();

    private:
        static QString m_sig;
    };
}

#endif // GLOBALS_H
