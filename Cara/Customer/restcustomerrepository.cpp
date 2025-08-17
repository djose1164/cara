#include "restcustomerrepository.h"
#include "Billing/payment.h"
#include "Network/networkrequestsingleton.h"
#include "Network/qnamsingleton.h"
#include <QRestAccessManager>
#include <QRestReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto paymentStatusIdField{"payment_status_id"_L1};
}

RestCustomerRepository::RestCustomerRepository(QObject *parent)
    : CustomerRepository{parent}
    , manager_{new QRestAccessManager(QNAMSingleton::instance().qnam(), this)}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , path_{"customers/"_L1}
{
}

void RestCustomerRepository::readBySalespersonId(SalespersonId salespersonId)
{
    Q_UNUSED(salespersonId);
    Q_ASSERT(false);
}

void RestCustomerRepository::fetchCustomersWithOutstandingOrders()
{
    using enum Entity::Payment::Status;
    const QUrlQuery query{{paymentStatusIdField, QString::number(int(Credited))},
                          {paymentStatusIdField, QString::number(int(Unpaid))}};
    const auto req{api_->createRequest(path_, query)};

    manager_->get(req, this, [this](QRestReply &reply) {
        if (!reply.isSuccess())
        {
            emit errorOccurred(reply.error(), reply.errorString());
        }

        if (const auto json{reply.readJson()}; json and json->isObject())
        {
            const QJsonArray customersArr = json->object()["customers"_L1].toArray();

            QList<QJsonObject> jsonList;
            std::ranges::for_each(customersArr, [&](const auto &json) {
                jsonList.append(json.toObject());
            });

            emit fetched(jsonList);
        }
    });
}
