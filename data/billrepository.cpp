#include "billrepository.h"
#include "constants.h"
#include "tokenmanager.h"
#include "tokenmanager.h"

namespace DataLayer {
BillRepository::BillRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
{

}

void BillRepository::fetchByCustomerId(int customerId)
{
    QQmlEngine engine;
    const auto constants{engine.singletonInstance<Constants *>("data", "Constants")};

    Q_ASSERT(!constants->base().isEmpty());
    const QUrl url{constants->base() + QString("/orders/?customer_id=%1").arg(customerId)};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const auto tokenManager{TokenManager::instance()};
    const QString token{tokenManager->accessToken()};

    request.setRawHeader("Authorization", token.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};
        if (reply_->error() == QNetworkReply::NoError)
            emit billsFetched(data);
        else
            emit errorOccurred(data);
    });
}

void BillRepository::fetchById(int orderId)
{

}

void BillRepository::fetchByAdminId(int adminId)
{
    QQmlEngine engine;
    const auto constants{engine.singletonInstance<Constants *>("data", "Constants")};

    Q_ASSERT(!constants->base().isEmpty());
    const QUrl url{constants->base() + QString("/orders/?admin_id=%1").arg(adminId)};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const auto tokenManager{TokenManager::instance()};
    const QString token{tokenManager->accessToken()};

    Q_ASSERT(!token.isEmpty());

    request.setRawHeader("Authorization", token.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};
        if (reply_->error() == QNetworkReply::NoError)
            emit billsFetched(data);
        else
            emit errorOccurred(data);
    });
}

void BillRepository::fetchBy(const QString &param, int id)
{
    QQmlEngine engine;
    const auto constants{engine.singletonInstance<Constants *>("data", "Constants")};

    Q_ASSERT(!constants->base().isEmpty());
    const QUrl url{constants->base() + QString("/orders/?%1=%2").arg(param, id)};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const auto tokenManager{TokenManager::instance()};
    const QString token{tokenManager->accessToken()};
    qDebug() << "TokenManager: " << token;
    Q_ASSERT(!token.isEmpty());

    request.setRawHeader("Authorization", token.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};
        if (reply_->error() == QNetworkReply::NoError)
            emit billsFetched(data);
        else
            emit errorOccurred(data);
    });
}
}
