#include "ordercontroller.h"
#include "data/restorderrepository.h"
#include "entity/payment.h"

using namespace Qt::StringLiterals;

namespace Business
{
OrderController::OrderController(QObject *parent)
    : QObject{parent}
    , repository_{new Data::RestOrderRepository(this)}
{
    connect(repository_, &OrderRepository::resourceAdded, this, &OrderController::orderCreated);
}

void OrderController::createOrder(const QVariantMap &order)
{
    repository_->add(order);
}

bool OrderController::validateOrder(const QVariantMap &order)
{
    if (!order.contains("salesperson"))
    {
        setErrorString("salesperson is missing.");
        return false;
    }
    if (!order.contains("customer"))
    {
        setErrorString("customer is missing.");
        return false;
    }
    if (!order.contains("products"))
    {
        setErrorString("products is missing.");
        return false;
    }
    if (!order.contains("pay"))
    {
        setErrorString("pay is missing.");
        return false;
    }
    if (!order.contains("orderDate"))
    {
        setErrorString("orderDate is missing.");
        return false;
    }

    const int salespersonId{order["salesperson"].toMap()["id"].toInt()};
    if (salespersonId < 1)
    {
        setErrorString("salesperson's id is missing.");
        return false;
    }

    const auto customer{order["customer"].toMap()};
    if (customer["id"].toInt() < 1 || customer["fullname"].toString().isEmpty())
    {
        setErrorString("customer's id or fullname are missing.");
        return false;
    }

    const auto productList{order["products"].toList()};
    if (productList.isEmpty())
    {
        setErrorString("product list is empty.");
        return false;
    }

    const auto pay{order["pay"].toMap()};
    if (!pay.contains("payment_status_id"_L1))
    {
        setErrorString("pay's payment_status_id is missing."_L1);
        return false;
    }
    if (pay["payment_status_id"_L1].toInt() == int(Entity::Payment::Status::Unpaid))
        return true;
    if (!pay.contains("paid_amount") || !pay.contains("payment_details") || !pay.contains("amount_to_pay"))
    {
        setErrorString("pay's paid_amount or payment_detail or amount_to_pay are missing.");
        return false;
    }

    return true;
}

OrderController::Error OrderController::error() const
{
    return error_;
}

void OrderController::setError(Error newError)
{
    if (error_ == newError)
        return;
    error_ = newError;
    emit errorChanged();
}

QString OrderController::errorString() const
{
    return errorString_;
}

void OrderController::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}
}
