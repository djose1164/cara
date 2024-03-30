#include "orderresource.h"

#include <QRestReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRestAccessManager.h>

using namespace Qt::StringLiterals;

static constexpr auto resourceId{"/%1"_L1};

namespace Billing
{
OrderResource::OrderResource(QObject *parent)
    : AbstractResource{parent}
    , path_{}
{
}

QString OrderResource::path() const
{
    return path_;
}

void OrderResource::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

void OrderResource::getOrderById(OrderId orderId)
{
    const auto req{api_->createRequest(
        path_ + resourceId.arg(QString::number(orderId))
    )};

    manager_->get(req, this, [&](QRestReply &reply) {
        if(!reply.isSuccess())
        {
            emit errorOccurred(reply.errorString());
            return;
        }

        const auto jsonDoc{reply.readJson()};
        emit fetched(jsonDoc->toJson());
    });
}
}
