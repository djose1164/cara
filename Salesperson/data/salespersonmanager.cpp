#include "salespersonmanager.h"
#include "networkrequestsingleton.h"
#include <QNetworkReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification{"/%1"_L1};
}

namespace Data
{
SalespersonManager::SalespersonManager(QObject *parent)
    : Business::Manageable{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{"salespersons"_L1}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{}

void SalespersonManager::read(int id)
{
    const auto req{
        api_->createRequest(path_
                            + resourceIdentification.arg(QString::number(id)))
    };

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        emit finished();
        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        reply_->deleteLater();
    });
}

void SalespersonManager::readAll(int adminId)
{
    const QUrlQuery query{ {"admin_id"_L1, QString::number(adminId)} };

    const auto req{api_->createRequest(path_, query)};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        emit finished();
        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        reply_->deleteLater();
    });
}

void SalespersonManager::add(const QByteArray &data)
{
    const auto req{api_->createRequest(path_ + "/"_L1)};

    reply_ = manager_->post(req, data);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        emit finished();
        if (reply_->error() == QNetworkReply::NoError)
            emit added();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }

        reply_->deleteLater();
    });
}

void SalespersonManager::edit(int id, const QByteArray &data)
{
}

void SalespersonManager::remove(int id)
{

}
}
