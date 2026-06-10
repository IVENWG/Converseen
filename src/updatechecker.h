#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = 0);
    void checkForUpdates();
    bool isUpdateAvailable();

private:
    void checkIfIsNewVersion(int version);

    QNetworkAccessManager* mNetworkManager;
    bool m_update_available;

private slots:
    void onNetworkReply(QNetworkReply* reply);

signals:
    void updateAvailable(const bool &isAvailable);
};

#endif // UPDATECHECKER_H