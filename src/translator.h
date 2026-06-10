#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <QSettings>
#include <QTranslator>
#include <QList>
#include <QPair>

class Translator : public QObject
{
    Q_OBJECT
public:
        Translator();
        QTranslator *translation();
        void saveSettings(QString language);
        QList<QPair<QString, QString> > loadTranslationFiles();
        QString currentLanguage();
        QString loadCurrentTranslationName();
        QString findLangDir();
        bool qmFilesFound(const QString &path);
private:
        QSettings *settings;
		QString m_loc;
};

#endif // __TRANSLATOR_H__
