#include "paymentresource.h"
#include <QRestReply>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace {
constexpr auto resourceIdentification = "/%1"_L1;
}

namespace Data
{
PaymentResource::PaymentResource(QObject *parent)
    : AbstractResource{parent}
    , data_{}
    , path_{}
    , paymentId_{}
{}

QList<QJsonObject> PaymentResource::data() const
{
    return data_;
}

void PaymentResource::refresh()
{
    manager_->get(api_->request(resourceIdentification.arg(QString::number(paymentId_))), this, [&](QRestReply *reply) {
        if (!reply->isSuccess())
        {
            qWarning() << "PaymentResource::refresh: " << reply->errorString();
            return;
        }

        const auto json{reply->json()};
        if (!json)
        {
            qWarning() << "PaymentResource::refresh: json no valid.";
            return;
        }

        const auto jsonObject{json.value()};
        const auto paymentObject{jsonObject["payment"].toObject()};
        if (!paymentObject["payment_details"].isArray())
        {
            qWarning() << "PaymentResource::refresh: payment_details is not an array.";
            return;
        }

        const auto detailsArray = paymentObject["payment_details"].toArray();
        for (const auto &payment : std::as_const(detailsArray))
            data_.append(payment.toObject());

        emit dataChanged();
    });
}
}
