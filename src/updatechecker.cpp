#include <QDesktopServices>

#include "updatechecker.h"
#include "globals.h"



UpdateChecker::UpdateChecker(QObject *parent) :
    QObject(parent)
{
    m_update_available = false;

    mNetworkManager = new QNetworkAccessManager(this);
    QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));
}

void UpdateChecker::checkForUpdates()
{
    QUrl url(VERSION_URL);
    QNetworkRequest netReq(url);
    netReq.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);    // Autoredirect

    mNetworkManager->get(netReq);
}

bool UpdateChecker::isUpdateAvailable()
{
    return m_update_available;
}

void UpdateChecker::onNetworkReply(QNetworkReply* reply)
{
    QString replyString;
    if(reply->error() == QNetworkReply::NoError)
    {
        int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();
        //qDebug() << "httpstatuscode: " << httpstatuscode;

        //QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        //qDebug() << "possibleRedirectUrl: " << possibleRedirectUrl;

        switch(httpstatuscode)
        {
        case 200:
            if (reply->isReadable())
            {
                replyString = QString::fromUtf8(reply->readAll().data());

                checkIfIsNewVersion(replyString.toInt());
            }
            break;
        }

        //qDebug() << "replyString: " << replyString;
    }

    reply->deleteLater();
}

void UpdateChecker::checkIfIsNewVersion(int version)
{
    m_update_available = false;

    if (version > globals::CURRENT_INTERNAL_VERSION)
        m_update_available = true;

    emit updateAvailable(m_update_available);
}
