#include <QMessageBox>
#include <QStyleFactory>
#include "dialogoptions.h"
#include "translator.h"
#include "inisettings.h"

#define INIFILENAME ".lessmb.conf"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(pushOk, SIGNAL(clicked()), this, SLOT(saveOptions()));
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(reject()));

    Translator t;
    for (int i = 0; i < t.loadTranslationFiles().count(); i++)
        comboLangs->addItem(t.loadTranslationFiles().at(i).second);

    comboLangs->addItem("English");

    populateStyles();

    loadSettings();
}

void DialogOptions::setLanguage()
{
    Translator t;
    QString lang;

    if (comboLangs->currentText() == "English")
        lang = "English";
    else
        lang = t.loadTranslationFiles()
            .at(comboLangs->currentIndex())
            .first;

    if (lang != t.loadCurrentTranslationName()) {
        t.saveSettings(lang);

        QMessageBox::warning(0, tr("Warning!"),
                                  tr("Please, restart LessMB to apply current language!"));
    }
}

void DialogOptions::setOverwriteMode()
{
    if (radioAskFirst->isChecked())
        IniSettings::setOverwriteMode(false);
    if (radioOverwrite->isChecked())
        IniSettings::setOverwriteMode(true);
}

void DialogOptions::setAutoUpdates()
{
    bool enabled;

    if (checkCheckUpdates->isChecked())
        enabled = true;
    else
        enabled = false;

    IniSettings::setAutoChechUpdates(enabled);
}

void DialogOptions::setTheme()
{
    const QString currentTheme = IniSettings::theme();

    QString newTheme;

    if (comboStyle->currentIndex() == 0) {
        newTheme = "none";
    } else {
        newTheme = comboStyle->currentText();
    }

    if (newTheme != currentTheme) {
        IniSettings::setTheme(newTheme);

        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please restart LessMB to apply the new theme!"));
    }
}

void DialogOptions::saveOptions()
{
    setLanguage();
    setOverwriteMode();
    setAutoUpdates();
    setTheme();

    IniSettings::settings->sync();
    accept();
}

void DialogOptions::loadSettings()
{
    bool overwriteMode = IniSettings::overwriteMode();

    if (overwriteMode)
        radioOverwrite->setChecked(true);
    else
        radioAskFirst->setChecked(true);

    bool autoUpdates = IniSettings::isAutoChechUpdates();
    if (autoUpdates)
        checkCheckUpdates->setCheckState(Qt::Checked);
    else
        checkCheckUpdates->setCheckState(Qt::Unchecked);

    Translator t;
    
    int idx = comboLangs->findText(t.currentLanguage(), Qt::MatchExactly);
    comboLangs->setCurrentIndex(idx);
}

void DialogOptions::populateStyles()
{
    QStringList availableStyles = QStyleFactory::keys();
    comboStyle->addItem(tr("System"));
    comboStyle->addItems(availableStyles);

    QString currentTheme = IniSettings::theme();
    int currentIndex = comboStyle->findText(currentTheme);

    if ((currentIndex != -1) && (currentTheme != "none")) {
        comboStyle->setCurrentIndex(currentIndex);
    }
    else {
        comboStyle->setCurrentIndex(0);
    }
}
