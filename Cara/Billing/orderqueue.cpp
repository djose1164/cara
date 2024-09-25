#include "orderqueue.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto orderField{"order"_L1};
constexpr auto commentField{"last_comment"_L1};
constexpr auto createdAtField{"created_at"_L1};
constexpr auto salespersonField{"salesperson"_L1};
constexpr auto statusIdField{"order_queue_status_id"_L1};
constexpr auto takenField{"taken"_L1};
}

namespace Billing
{

OrderQueue::OrderQueue(QObject *parent)
    : QObject{parent}
    , bill_{new Entity::Bill{this}}
    , createdAt_{}
    , status_{Status::Any}
    , statusString_{}
    , salesperson_{}
    , lastComment_{}
{
    statusString_.setBinding([this] {
        return statusToString(static_cast<Status>(status_.value()));
    });
}

void OrderQueue::setBill(Entity::Bill *newBill)
{
    bill_ = newBill;
    bill_->setParent(this);
}

QBindable<Entity::Bill *> OrderQueue::bindableBill() const
{
    return &bill_;
}

QBindable<QDateTime> OrderQueue::bindableCreatedAt() const
{
    return &createdAt_;
}

QBindable<OrderQueue::Status> OrderQueue::bindableStatus()
{
    return &status_;
}

QBindable<QString> OrderQueue::bindableStatusString() const
{
    return &statusString_;
}

QBindable<Entity::Salesperson> OrderQueue::bindableSalesperson() const
{
    return &salesperson_;
}

QBindable<bool> OrderQueue::bindableTaken()
{
    return &taken_;
}

QBindable<QString> OrderQueue::bindableLastComment()
{
    return &lastComment_;
}

QString OrderQueue::statusToString(Status statudId)
{
    switch (statudId)
    {
        using enum Status;
    case Unassigned:
        return "Sin asignar"_L1;
    case Assigned:
        return "Asignado"_L1;
    case Completed:
        return "Completado"_L1;
    case Cancelled:
        return "Cancelado"_L1;
    case Refund:
        return "Reembolsado"_L1;
    case Returned:
        return "Devuelto"_L1;
    case Any:
        return "Todas"_L1;
    default:
        return "Error"_L1;
    }
}

OrderQueue::Status OrderQueue::statusStringToId(QAnyStringView s)
{
    if (s == "Sin asignar"_L1)
        return Status::Unassigned;
    else if (s == "Asignadas"_L1)
        return Status::Assigned;
    else if (s == "Completadas"_L1)
        return Status::Completed;
    else if (s == "Canceladas"_L1)
        return Status::Cancelled;
    else if (s == "Devueltas"_L1)
        return Status::Returned;
    else if (s == "Reembolsadas"_L1)
        return Status::Refund;
    else
        return Status::Any;
}

std::unique_ptr<OrderQueue> OrderQueue::fromJson(const QJsonObject &json)
{
    auto queue{std::make_unique<OrderQueue>()};
    auto &&orderMap{json["order"]
        .toObject()
        .toVariantMap()
    };
    queue->setBill(Entity::Bill::fromMap(orderMap));

    Q_ASSERT(queue->bill_->totalToPay() > 0);
    Q_ASSERT(queue->bill_->quantityOfProducts());
    Q_ASSERT(queue->bill_->orderId() > 0);

    const auto dateString{json[createdAtField].toString()};
    queue->createdAt_ = QDateTime::fromString(dateString, Qt::ISODate);
    queue->status_ = static_cast<Status>(json[statusIdField].toInt());
    queue->salesperson_ = Entity::Salesperson::fromJson(
        QJsonDocument::fromVariant(json[salespersonField])
            .toJson()
    );
    queue->taken_ = json[takenField].toBool();
    queue->lastComment_ = json[commentField].toString();
    return queue;
}

} // namespace Billing
