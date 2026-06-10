#include "multipageconverter.h"
#include <Magick++.h>
#include <string>
#include <iostream>
#include <list>
#include <QDebug>

using namespace Magick;
using namespace std;

MultipageConverter::MultipageConverter(QObject *parent) :
    QObject(parent)
{

}

void MultipageConverter::readFile(QString fileName)
{
    Image my_image;

    m_pagesList.clear();

    list<Image> pdfPages;

    try {
    #if MagickLibVersion < 0x700
        Magick::readImages(&pdfPages, fileName.toStdString());
    #else
        Magick::pingImages(&pdfPages, fileName.toStdString());
    #endif

        for (std::list<Image>::iterator it = pdfPages.begin(); it != pdfPages.end(); ++it){
            MultipageItem mpi;

            mpi.w = it->columns();
            mpi.h = it->rows();

            mpi.xres = it->xResolution();
            mpi.yres = it->yResolution();

            mpi.depth = it->depth();

            m_pagesList << mpi;
        }
    }
    catch (Error& my_error) {
        qWarning() << "Warning: " << QString::fromStdString(my_error.what());
    }
    catch( Magick::WarningCoder &warning )
    {
        qWarning() << "Warning: " << QString::fromStdString(warning.what());
    }
    catch( Magick::Warning &warning )
    {
        qWarning() << "Warning: " << QString::fromStdString(warning.what());
    }
}

QList<MultipageItem> MultipageConverter::pages()
{
    return m_pagesList;
}
