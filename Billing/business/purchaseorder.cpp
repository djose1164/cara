#include "purchaseorder.h"

#include <QRestReply>
#include <QJsonDocument>

#define checkError(reply)                                                      \
    do {                                                                       \
        if (!(reply)->isSuccess())                                              \
        {                                                                      \
            emit errorOccurred((reply)->errorString());                        \
            return;                                                            \
        }                                                                      \
    } while (false)

namespace Billing
{
PurchaseOrder::PurchaseOrder(QObject *parent)
    : AbstractResource{parent}
{}

void PurchaseOrder::placeOrder(const QVariantMap &data)
{
    const auto req{api_->createRequest(path_)};

    manager_->post(req, data, this, [&](QRestReply *reply) {
        checkError(reply);
        emit orderPlaced(reply->json()->toJson());
    });
}

void PurchaseOrder::fetchOrders()
{
    const auto req{api_->createRequest(path_)};

    manager_->get(req, this, [&](QRestReply *reply) {
        checkError(reply);
        emit ordersFetched(reply->json()->toJson());
    });
}
}
