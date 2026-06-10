#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>

class IniSettings : public QObject
{
    Q_OBJECT
public:
    explicit IniSettings(QObject *parent = 0);

    static void init();

    static bool isDirWritable(const QDir &dir);
    static QString outputDir();
    static QByteArray windowGeometry();
    static int latestWrittenFormatIndex();
    static int overwriteMode();
    static int jpgQuality();
    static int pngQuality();
    static bool isWebPLosslessCompression();
    static int webPCompression();
    static int webPQuality();
    static bool iskWebPDithering();
    static bool isRemoveMetadata();
    static QString latestOpenedDir();
    static QString backgroundColor();

    static bool previewChecked();
    static bool scaleChecked();
    static int measure();    // get px or %
    static bool aspectRatioChecked();
    static bool resolutionChecked();
    static bool imageDirChecked();
    static bool renameChecked();
    static bool bgColorChecked();
    static int latestInterpFiltIdx();
    static int currentVersion();
	static QString language();
    static bool isAutoChechUpdates();
    static QString theme();

    static void setOutputDir(QString path);
    static void setWindowGeometry(QByteArray geometry);
    static void setLatestWrittenFormatIndex(int index);
    static void setOverwriteMode(int overwriteMode);
    static void setJpgQuality(int quality);
    static void setPngQuality(int quality);
    static void setLatestOpenedDir(QString path);
    static void setBackgroundColor(QString bgColor);
    static void setPreviewChecked(bool state);
    static void setScaleChecked(bool state);
    static void setMeasure(int index);
    static void setAspectRatioChecked(bool state);
    static void setResolutionChecked(bool state);
    static void setImageDirChecked(bool state);
    static void setRenameChecked(bool state);
    static void setBgColorChecked(bool state);
    static void setLatestInterpFiltIdx(int index);
    static void setCurrentVersion(int version);
	static void setLanguage(QString lang);
    static void setAutoChechUpdates(bool enabled);
    static void setTheme(const QString &themeName);

    static void setIsWebPLosslessCompression(const bool &value);
    static void setWebPCompression(const int &value);
    static void setWebPQuality(const int &value);
    static void setIskWebPDithering(const bool &value);
    static void setIsRemoveMetadata(const bool &value);


//private:
    static QSettings *settings;
signals:

public slots:

};

#endif // INISETTINGS_H
