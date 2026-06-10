#include <QMovie>
#include "mylabelpreviewer.h"
#include "formats.h"
#include "globals.h"

myLabelPreviewer::myLabelPreviewer(QWidget *parent) : QLabel(parent)
{
    thumbGen = new ThumbnailGeneratorThread(this);

    m_scaleFactor = globals::Globals::scaleFactor();

    connect(thumbGen, SIGNAL(pixmapGenerated(QImage, int, int, double, double)), this, SLOT(showPreview(QImage, int, int, double, double)));
}

void myLabelPreviewer::loadPreview(QString fileName, bool generateThumbnail)
{
    m_generateThumbnail = generateThumbnail;

    if (generateThumbnail)
        showLoadingAnimation();

    thumbGen->setFileName(fileName);
    thumbGen->setThumbnailGeneration(generateThumbnail);
    thumbGen->start();
}

void myLabelPreviewer::showPreview(QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y)
{
    if (thumbnail.isNull() || !m_generateThumbnail) {
        setText(tr("Preview"));
    }
    else {
        QPixmap pixmap = QPixmap::fromImage(thumbnail);
        pixmap = pixmap.scaled(QSize(320, 240) * m_scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmap.setDevicePixelRatio(m_scaleFactor);

        this->setPixmap(pixmap);
    }

    emit previewReady(orig_w, orig_h, orig_dens_x, orig_dens_y);
}

void myLabelPreviewer::showLoadingAnimation()
{
    QMovie *movie = new QMovie;
    movie->setFileName(":/movies/res/loading.gif");

    setMovie(movie);
    movie->start();
}
