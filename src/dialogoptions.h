#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QSettings>
#include <QDir>
#include "ui_dialogoptions.h"

class DialogOptions : public QDialog, private Ui::DialogOptions {
    Q_OBJECT
public:
    DialogOptions(QWidget *parent = 0);
private:
    void setLanguage();
    void setOverwriteMode();
    void setAutoUpdates();
    void setTheme();
    void loadSettings();
    void populateStyles();

    QSettings *settings;
private slots:
    void saveOptions();
};

#endif // DIALOGOPTIONS_H
