#include "billrepository.h"
#include "Utils/networkrequestsingleton.h"
#include "notimplementated.h"
#include <QNetworkReply>

using namespace Qt::StringLiterals;

namespace DataLayer {
BillRepository::BillRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{

}

void BillRepository::fetchByCustomerId(int customerId)
{
    constexpr auto path{"/orders/?customer_id=%1"_L1};
    const auto request{
        api_->createRequest(path.arg(QString::number(customerId)))
    };

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
    throw Utils::NotImplementated();
}

void BillRepository::fetchByAdminId(int adminId)
{
    constexpr auto path{"/orders/?admin_id=%1"_L1};
    const auto request{api_->createRequest(path.arg(QString::number(adminId)))};

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
    constexpr auto path{"/orders/?%1=%2"_L1};
    const auto request{
        api_->createRequest(path.arg(param, QString::number(id)))
    };

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
