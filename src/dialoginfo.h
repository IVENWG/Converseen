#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include "ui_dialoginfo.h"

class DialogInfo : public QDialog, private Ui::DialogInfo {
    Q_OBJECT
public:
    DialogInfo(QWidget *parent = 0);
};

#endif // DIALOGINFO_H