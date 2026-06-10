#include "updatechecker.h"

UpdateChecker::UpdateChecker(QObject *parent) :
    QObject(parent)
{
    m_update_available = false;
    mNetworkManager = nullptr;
}

void UpdateChecker::checkForUpdates()
{
    m_update_available = false;
}

bool UpdateChecker::isUpdateAvailable()
{
    return false;
}

void UpdateChecker::onNetworkReply(QNetworkReply* reply)
{
    if (reply)
        reply->deleteLater();
}

void UpdateChecker::checkIfIsNewVersion(int version)
{
    Q_UNUSED(version);
    m_update_available = false;
    emit updateAvailable(false);
}