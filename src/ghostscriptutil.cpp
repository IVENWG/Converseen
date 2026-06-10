#include "ghostscriptutil.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>

namespace {

bool runVersionCheck(const QString &command)
{
    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process.start(command, QStringList() << "--version");

    if (!process.waitForStarted(3000)) {
        return false;
    }

    if (!process.waitForFinished(5000)) {
        process.kill();
        return false;
    }

    return process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0;
}

#if defined(Q_OS_WIN)

void appendProgramFilesCandidates(QStringList *candidates, const QString &rootPath)
{
    const QDir programFiles(rootPath);
    if (!programFiles.exists()) {
        return;
    }

    const QStringList versions = programFiles.entryList(
        QDir::Dirs | QDir::NoDotAndDotDot,
        QDir::Name | QDir::Reversed);

    for (const QString &version : versions) {
        const QStringList names = {"gswin64c.exe", "gswin32c.exe"};
        for (const QString &name : names) {
            const QString fullPath = programFiles.filePath(version + "/bin/" + name);
            if (QFileInfo::exists(fullPath)) {
                candidates->append(fullPath);
            }
        }
    }
}

QStringList candidateExecutables(const QString &appDirPath)
{
    QStringList candidates;

    const QStringList relativePaths = {
        "ghostscript/bin/gswin64c.exe",
        "ghostscript/bin/gswin32c.exe",
        "ghostscript/gswin64c.exe",
        "ghostscript/gswin32c.exe",
        "gs/bin/gswin64c.exe",
        "gs/bin/gswin32c.exe",
    };

    for (const QString &relativePath : relativePaths) {
        const QString fullPath = QDir(appDirPath).filePath(relativePath);
        if (QFileInfo::exists(fullPath)) {
            candidates.append(fullPath);
        }
    }

    const QDir bundledGhostscript(QDir(appDirPath).filePath("ghostscript"));
    if (bundledGhostscript.exists()) {
        const QStringList versionDirs = bundledGhostscript.entryList(
            QStringList() << "gs*",
            QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QString &versionDir : versionDirs) {
            const QStringList names = {"gswin64c.exe", "gswin32c.exe"};
            for (const QString &name : names) {
                const QString fullPath = bundledGhostscript.filePath(versionDir + "/bin/" + name);
                if (QFileInfo::exists(fullPath)) {
                    candidates.append(fullPath);
                }
            }
        }
    }

    appendProgramFilesCandidates(&candidates, "C:/Program Files/gs");
    appendProgramFilesCandidates(&candidates, "C:/Program Files (x86)/gs");

    candidates << "gswin64c" << "gswin32c";

    return candidates;
}

QString findResourceDir(const QString &executablePath)
{
    QFileInfo fi(executablePath);
    QDir gsRoot = fi.dir();

    if (gsRoot.dirName().compare("bin", Qt::CaseInsensitive) == 0) {
        gsRoot.cdUp();
        const QString resourceDir = gsRoot.filePath("Resource");
        if (QDir(resourceDir).exists()) {
            return resourceDir;
        }
    }

    return QString();
}

void prependPath(const QString &directory)
{
    if (directory.isEmpty()) {
        return;
    }

    const QByteArray currentPath = qgetenv("PATH");
    const QString nativeDir = QDir::toNativeSeparators(directory);

    if (QString::fromLocal8Bit(currentPath).contains(nativeDir, Qt::CaseInsensitive)) {
        return;
    }

    const QByteArray updatedPath = nativeDir.toLocal8Bit() + ";" + currentPath;
    qputenv("PATH", updatedPath);
}

void configureGhostscriptResources(const QString &executablePath)
{
    const QString resourceDir = findResourceDir(executablePath);
    if (resourceDir.isEmpty()) {
        return;
    }

    const QString initDir = QDir(resourceDir).filePath("Init");
    const QString fontDir = QDir(resourceDir).filePath("Font");

    if (QDir(initDir).exists()) {
        qputenv("GS_LIB", QDir::toNativeSeparators(initDir).toLocal8Bit());
    }

    if (QDir(fontDir).exists()) {
        qputenv("GS_FONTPATH", QDir::toNativeSeparators(fontDir).toLocal8Bit());
    }

    const QString options = QString("-sGenericResourceDir=%1/ -sICCProfilesDir=%1/iccprofiles/")
        .arg(QDir::toNativeSeparators(resourceDir));
    qputenv("GS_OPTIONS", options.toLocal8Bit());
}

#endif

} // namespace

QString GhostscriptUtil::executablePath()
{
#if defined(Q_OS_WIN)
    const QString appDir = QCoreApplication::applicationDirPath();

    for (const QString &candidate : candidateExecutables(appDir)) {
        if (candidate.endsWith(".exe", Qt::CaseInsensitive)) {
            if (runVersionCheck(candidate)) {
                return QFileInfo(candidate).absoluteFilePath();
            }
        } else if (runVersionCheck(candidate)) {
            return candidate;
        }
    }

    return QString();
#else
    return QString();
#endif
}

bool GhostscriptUtil::isInstalled()
{
#if defined(Q_OS_WIN)
    return !executablePath().isEmpty();
#else
    return true;
#endif
}

void GhostscriptUtil::setupEnvironment()
{
#if defined(Q_OS_WIN)
    const QString gsExe = executablePath();
    if (gsExe.isEmpty()) {
        return;
    }

    prependPath(QFileInfo(gsExe).absolutePath());
    configureGhostscriptResources(gsExe);
#endif
}