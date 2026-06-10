#include <QScreen>
#include <QGuiApplication>
#include "globals.h"

QString globals::Globals::m_sig;

void globals::Globals::setSignature(const QString &signature)
{
    m_sig = signature;
}

QString globals::Globals::signature()
{
    using namespace globals;
    return m_sig;
}

QString globals::Globals::magickVersion()
{
    QString imVersion;
    const char *version = MagickCore::GetMagickVersion(nullptr);

    imVersion = QString::fromStdString(version);

    return imVersion;
}

qreal globals::Globals::scaleFactor()
{
    qreal scale = 1.0;

    if (QGuiApplication::screens().count() > 0) {
        QScreen *screen = QGuiApplication::screens().at(0);
        scale = screen->devicePixelRatio();
    }

    return scale;
}
