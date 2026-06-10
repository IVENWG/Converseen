#ifndef DIALOGCONVERSIONSTATUS_H
#define DIALOGCONVERSIONSTATUS_H

#include "ui_dialogconversionstatus.h"

class DialogConversionStatus : public QDialog, private Ui::DialogConversionStatus {
    Q_OBJECT
public:
    DialogConversionStatus(QWidget *parent = 0);
    void setup(int n_images);
    void conversionStatus(int conv_status, QString fileName);
    void step(int conv_status, QString fileName);
    void setErrorMsg(QString err_status);
    void addOutputDirectory(QString path);

private:
    int c_tot;
    int c_ok;
    int c_no;

    int m_totimages;

    QString m_msg;

    QStringList outDirs;
    QMenu *menu;
    QAction *actionOpenAllDirs;

    void counter(int conv_status);
    void resetButtons();
    void setupOpenButton();

signals:
    void stopProcess();

private slots:
    void abort();
    void openDir(QAction *action);
    void openAllDirs();
};

#endif // DIALOGCONVERSIONSTATUS_H
