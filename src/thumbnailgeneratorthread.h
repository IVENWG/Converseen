#ifndef THUMBNAILGENERATORTHREAD_H
#define THUMBNAILGENERATORTHREAD_H

#include <string>
#include <iostream>
#include <Magick++.h>

#include <QThread>
#include <QObject>
#include <QPixmap>
#include <QPixmapCache>
#include "cachingsystem.h"

using namespace Magick;
using namespace std;

class ThumbnailGeneratorThread : public QThread
{
    Q_OBJECT
public:
    ThumbnailGeneratorThread(QObject *parent);
    void run();

    void setFileName(QString fileName);
    void setThumbnailGeneration(bool generate);

private:
    void createThumbnail();
    QImage autoOrientImage(const QImage &image, const int &orientation);
    QImage* toQImage(const Image &image);
    QString m_fileName;
    bool m_generateThumbnail;

signals:
    void pixmapGenerated(QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);

public slots:

};

#endif // THUMBNAILGENERATORTHREAD_H
