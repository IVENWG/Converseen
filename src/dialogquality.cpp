#include "dialogquality.h"
#include <Magick++.h>

using namespace Magick;
using namespace std;

DialogQuality::DialogQuality(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(sliderJpegQ, SIGNAL(valueChanged(int)), this, SLOT(setJpegSpin(int)));
    connect(spinJpegQ, SIGNAL(valueChanged(int)), this, SLOT(setJpegSlider(int)));
    connect(sliderPngQ, SIGNAL(valueChanged(int)), this, SLOT(setPngSpin(int)));
    connect(spinPngQ, SIGNAL(valueChanged(int)), this, SLOT(setPngSlider(int)));

    connect(sliderWebPCompression, SIGNAL(valueChanged(int)), this, SLOT(setWebpCompressionSpin(int)));
    connect(spinWebPCompression, SIGNAL(valueChanged(int)), this, SLOT(setWebpCompressionSlider(int)));
    connect(sliderWebPQuality, SIGNAL(valueChanged(int)), this, SLOT(setWebpQualitySpin(int)));
    connect(spinWebPQuality, SIGNAL(valueChanged(int)), this, SLOT(setWebpQualitySlider(int)));

    connect(pushOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void DialogQuality::setInitValues(int jpegVal, int pngVal, IMFilterType filter)
{
    spinJpegQ->setValue(jpegVal);
    spinPngQ->setValue(pngVal);

    int filterIndex = comboFilters->searchFilterIndex(filter);
    comboFilters->setCurrentIndex(filterIndex);
}

void DialogQuality::setIsWebPLosslessCompression(const bool &value)
{
    checkWebPLCompression->setChecked(value);
}

void DialogQuality::setWebPCompression(const int &value)
{
    spinWebPCompression->setValue(value);
}

void DialogQuality::setWebPQuality(const int &value)
{
    spinWebPQuality->setValue(value);
}

void DialogQuality::setIskWebPDithering(const bool &value)
{
    checkWebPDithering->setChecked(value);
}

void DialogQuality::setIsRemoveMetadata(const bool &value)
{
    checkRemoveMetadata->setChecked(value);
}

int DialogQuality::getJpegQuality()
{
    return spinJpegQ->value();
}

int DialogQuality::getPngQuality()
{
    return spinPngQ->value();
}

bool DialogQuality::isWebPLosslessCompression()
{
    return checkWebPLCompression->isChecked();
}

int DialogQuality::getWebPCompression()
{
    return spinWebPCompression->value();
}

int DialogQuality::getWebPQuality()
{
    return spinWebPQuality->value();
}

bool DialogQuality::iskWebPDithering()
{
    return checkWebPDithering->isChecked();
}

bool DialogQuality::isRemoveMetadata()
{
    return checkRemoveMetadata->isChecked();
}

IMFilterType DialogQuality::getResamplingFilter()
{
    return comboFilters->currentFilter();
}

void DialogQuality::setJpegSpin(int value)
{
    spinJpegQ->setValue(value);
}

void DialogQuality::setJpegSlider(int value)
{
    sliderJpegQ->setValue(value);
}
//
void DialogQuality::setPngSpin(int value)
{
    spinPngQ->setValue(value);
}

void DialogQuality::setPngSlider(int value)
{
    sliderPngQ->setValue(value);
}

void DialogQuality::setWebpCompressionSpin(int value)
{
    spinWebPCompression->setValue(value);
}

void DialogQuality::setWebpCompressionSlider(int value)
{
    sliderWebPCompression->setValue(value);
}

void DialogQuality::setWebpQualitySpin(int value)
{
    spinWebPQuality->setValue(value);
}

void DialogQuality::setWebpQualitySlider(int value)
{
    sliderWebPQuality->setValue(value);
}

