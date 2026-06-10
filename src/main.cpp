#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QDir>
#include <QCommandLineParser>
#include <QWidget>
#include <QStyleFactory>
#include <Magick++.h>

#include "mainwindowimpl.h"
#include "inisettings.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif

#include "translator.h"
#include "globals.h"

int main(int argc, char ** argv)
{
	InitializeMagick(*argv);

    QCoreApplication::setApplicationName("LessMB");
    QCoreApplication::setApplicationVersion(globals::VERSION);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app( argc, argv );
    app.setWindowIcon(QIcon(":/Images/res/icon.png"));

    IniSettings::init();
    QString theme = IniSettings::theme();
    if (theme != "none")
        app.setStyle(QStyleFactory::create(theme));

    app.setDesktopFileName("lessmb");

    QCommandLineOption winMagickPathOption({{"m", "debugMagickWindowsPath"}, "Set the default ImageMagick path on Windows (for debug purpose only!).", "C:\\MagickInstallPath"});
    QCommandLineOption importTxtListOption({{"l", "list"}, "Reads a txt file with a list of files to be imported.", "list.txt"});
    QCommandLineOption printSupportedFormats({{"p", "supported-formats"}, "Prints a list of readable/writable supported formats."});

    QCommandLineParser parser;
    parser.setApplicationDescription("LessMB - Batch Image Compression Tool");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(winMagickPathOption);
    parser.addOption(importTxtListOption);
    parser.addOption(printSupportedFormats);

    parser.process(app);

#if defined(Q_OS_OSX)
    QString appDirPath = QApplication::applicationDirPath();

    QDir gs_dir(appDirPath + "/../Resources/ghostscript");
    QString gs_path = gs_dir.absolutePath();

    QString magick_configure_path = appDirPath + "/../Resources/ImageMagick-7";
    QString gs_lib = gs_path + "/Resource/Init/";
    QString gs_fontpath = gs_path + "/Resource/Font/";
    QString gs_options = QString("-sGenericResourceDir=%1/Resource/ -sICCProfilesDir=%1/iccprofiles/").arg(gs_path);

    qputenv("MAGICK_CONFIGURE_PATH", magick_configure_path.toUtf8());
    qputenv("GS_LIB", gs_lib.toUtf8());
    qputenv("GS_FONTPATH", gs_fontpath.toUtf8());
    qputenv("GS_OPTIONS", gs_options.toUtf8());
#endif

#if defined(Q_OS_WIN)
    QString resdir = QApplication::applicationDirPath();

    if (parser.isSet(winMagickPathOption))
        resdir = parser.value(winMagickPathOption);

    qputenv("LD_LIBRARY_PATH", resdir.toUtf8());
    qputenv("MAGICK_HOME", resdir.toUtf8());
    qputenv("MAGICK_CONFIGURE_PATH", resdir.toUtf8());
    qputenv("MAGICK_CODER_MODULE_PATH", resdir.toUtf8() + "\\modules\\coders");
    qputenv("MAGICK_CODER_FILTER_PATH", resdir.toUtf8() + "\\modules\\filters");
#endif

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    Translator T;

    QTranslator *t = T.translation();

    if (t != NULL)
        app.installTranslator(t);

    MainWindowImpl win;

    if (parser.isSet(importTxtListOption)) {
        win.importListFromArgv(parser.value(importTxtListOption));
    }

    if (parser.isSet(printSupportedFormats)) {
        win.printSupportedFormats();

        return 0;
    }

    win.show();
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    return app.exec();
}