#ifndef GHOSTSCRIPTUTIL_H
#define GHOSTSCRIPTUTIL_H

#include <QString>

namespace GhostscriptUtil
{
    QString executablePath();
    bool isInstalled();
    void setupEnvironment();
}

#endif // GHOSTSCRIPTUTIL_H