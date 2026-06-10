#ifndef MYLABELPREVIEWER_H
#define MYLABELPREVIEWER_H

#include <QLabel>
#include "thumbnailgeneratorthread.h"

class myLabelPreviewer : public QLabel
{
    Q_OBJECT
public:
    explicit myLabelPreviewer(QWidget *parent = 0);
    void loadPreview(QString fileName, bool generateThumbnail);

signals:
    void previewReady(int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);

private:
    void showLoadingAnimation();

    ThumbnailGeneratorThread *thumbGen;
    bool m_generateThumbnail;
    qreal m_scaleFactor;

private slots:
    void showPreview(QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);
};

#endif // MYLABELPREVIEWER_H
