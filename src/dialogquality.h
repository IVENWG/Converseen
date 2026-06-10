#ifndef DIALOGQUALITY_H
#define DIALOGQUALITY_H

#include "ui_dialogquality.h"

class DialogQuality : public QDialog, private Ui::DialogQuality {
    Q_OBJECT
public:
    DialogQuality(QWidget *parent = 0);
    void setInitValues(int jpegVal, int pngVal, IMFilterType filter);

    void setIsWebPLosslessCompression(const bool &value);
    void setWebPCompression(const int &value);
    void setWebPQuality(const int &value);
    void setIskWebPDithering(const bool &value);
    void setIsRemoveMetadata(const bool &value);

    int getJpegQuality();
    int getPngQuality();

    bool isWebPLosslessCompression();
    int getWebPCompression();
    int getWebPQuality();
    bool iskWebPDithering();
    bool isRemoveMetadata();

    IMFilterType getResamplingFilter();

private slots:
    void setJpegSpin(int value);
    void setJpegSlider(int value);

    void setPngSpin(int value);
    void setPngSlider(int value);

    void setWebpCompressionSpin(int value);
    void setWebpCompressionSlider(int value);
    void setWebpQualitySpin(int value);
    void setWebpQualitySlider(int value);
};

#endif // DIALOGQUALITY_H
