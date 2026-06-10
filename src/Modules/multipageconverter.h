#ifndef MULTIPAGECONVERTER_H
#define MULTIPAGECONVERTER_H

#include <QObject>
#include <QList>

class MultipageItem {
public:
    int w, h, xres, yres, depth;
};

class MultipageConverter : public QObject
{
    Q_OBJECT
public:
    explicit MultipageConverter(QObject *parent = 0);

    void readFile(QString fileName);
    QList<MultipageItem> pages();

private:
    QList<MultipageItem> m_pagesList;

signals:

public slots:

};

#endif // MULTIPAGECONVERTER_H
