#include "associatesalespersonrepositoryimp.h"
#include "constants.h"
#include "tokenmanager.h"

namespace Data {
AssociateSalespersonRepositoryImpl::AssociateSalespersonRepositoryImpl(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
{
    QQmlEngine engine;
    const auto constants = engine.singletonInstance<DataLayer::Constants *>("data", "Constants");
    Q_ASSERT(constants);
    base_ = constants->base() + "/associate_salespersons/";
}

void AssociateSalespersonRepositoryImpl::getAssociateSalespersonById(int id)
{

}

void AssociateSalespersonRepositoryImpl::addAssociateSalesperson(const Entity::Salesperson &salesperson)
{
    const QNetworkRequest request{prepareRequest()};

    reply_ = manager_->post(request, salesperson.toJson());

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto content{reply_->readAll()};
        emit finished(content);

        if (reply_->error() == QNetworkReply::NoError)
            emit added();
        else
            emit errorOccurred();
    });
}

void AssociateSalespersonRepositoryImpl::getAssociateSalespersons()
{
    const QNetworkRequest request{prepareRequest()};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto content{reply_->readAll()};
        emit finished(content);

        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
            emit errorOccurred();
    });
}

QNetworkRequest AssociateSalespersonRepositoryImpl::prepareRequest() const
{
    QNetworkRequest request{base_};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const auto token{DataLayer::TokenManager::instance()->accessToken().toUtf8()};
    request.setRawHeader("Authorization", token);
    return request;
}
}
