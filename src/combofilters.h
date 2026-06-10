#ifndef COMBOFILTERS_H
#define COMBOFILTERS_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <Magick++.h>

#include "globals.h"

using namespace Magick;
using namespace std;

class Combofilters : public QComboBox
{
    Q_OBJECT
public:
    explicit Combofilters(QWidget *parent = 0);

    IMFilterType currentFilter();
    int searchFilterIndex(IMFilterType filter);
    
signals:

public slots:
    
private:
    void populateList();

    QList<IMFilterType> filtersList;
};

#endif // COMBOFILTERS_H
