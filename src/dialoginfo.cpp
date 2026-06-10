#include <QUrl>
#include "dialoginfo.h"
#include "globals.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));

    QString appVersion;
    QString magickVersion;
    QString iMVersionStrTr = tr("ImageMagick Version:");
    QString qtVersionStrTr = tr("Qt Version:");

#ifdef Q_OS_WIN32
    appVersion = QString("%1%2")
            .arg(QCoreApplication::applicationVersion())
            .arg(globals::Globals::signature());
#else
    appVersion = QCoreApplication::applicationVersion();
#endif

    QString title = QString("<p><span style=\" font-size:12pt; font-weight:600;\">LessMB %1</span><br />"
                            "<span style=\" font-size:10pt;\">%2</span></p>")
            .arg(appVersion)
            .arg(tr("Batch image compression and conversion tool."));

    labelTitle->setText(title);

    magickVersion = globals::Globals::magickVersion();
    QString im_versionString = QString("<p><span style=\" font-size:8pt; font-weight:600;\">%1 </span><span style=\" font-size:8pt;\">%2</span></p>")
                                   .arg(iMVersionStrTr)
                                   .arg(magickVersion);

    labelMagickVersion->setText(im_versionString);

    QString qt_versionString = QString("<p><span style=\" font-size:8pt; font-weight:600;\">%1 </span><span style=\" font-size:8pt;\">%2</span></p>")
                                   .arg(qtVersionStrTr)
                                   .arg(QT_VERSION_STR);
    labelQtVersion->setText(qt_versionString);

    adjustSize();
    this->resize(480, 280);
}