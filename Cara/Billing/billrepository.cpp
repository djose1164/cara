#include "billrepository.h"
#include <QNetworkReply>
#include "networkrequestsingleton.h"
#include "notimplementated.h"
#include "payment.h"

using namespace Qt::StringLiterals;

namespace DataLayer {
BillRepository::BillRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , api_{Authentication::NetworkRequestSingleton::instance().api()}
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

        reply_->deleteLater();
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

        reply_->deleteLater();
    });
}

QNetworkReply *BillRepository::fetchUnpaidBills()
{
    qDebug() << "BillRepository::fetchUnpaidBills: issuing request...";
    using enum Entity::Payment::Status;
    constexpr int paymentStatusId{std::to_underlying(Unpaid)};

    QUrlQuery query;
    query.addQueryItem("payment_status_id"_L1, QString::number(paymentStatusId));

    const auto request{
        api_->createRequest("/orders/"_L1, std::move(query))
    };

    QNetworkReply *reply{manager_->get(request)};
    qDebug() << "BillRepository::fetchUnpaidBills: issuing request...Done";
    return reply;
}

QNetworkReply *BillRepository::fetchOrdersInQueue()
{
    return manager_->get(api_->createRequest("/orders/queue"_L1));
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

        reply_->deleteLater();
    });
}
}
