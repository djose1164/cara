#include "orderqueuemodel.h"

using namespace Qt::StringLiterals;

namespace Billing
{

OrderQueueModel::OrderQueueModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{}
    , repositoryNotifier_{}
    , repositoryConnections_{}
{
    repositoryNotifier_ = repository_.addNotifier([this] {
        std::ranges::for_each(repositoryConnections_, [](QMetaObject::Connection &conn) {
            disconnect(conn);
        });
        repositoryConnections_.clear();

        repositoryConnections_.append(
            connect(repository_.value(), &QueueRepository::queueFetched,
                    this, &OrderQueueModel::queueFetched)
        );
    });
}

int OrderQueueModel::rowCount(const QModelIndex &parent) const
{
    return data_.size();
}

QVariant OrderQueueModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid() or index.row() >= data_.size())
        return {};

    const auto &queue{data_.at(index.row())};
    const auto &bill{queue->bindableBill().value()};
    switch(static_cast<RoleNames>(role))
    {
    case RoleNames::CommentRole:
        return queue->bindableLastComment().value();
    case RoleNames::CreatedAtRole:
        return queue->bindableCreatedAt().value();
    case RoleNames::SalespersonRole:
        return QVariant::fromValue(queue->bindableSalesperson().value());
    case RoleNames::OrderRole:
        return QVariant::fromValue(queue->bindableBill().value());
    case RoleNames::QueueStatusIdRole:
        return QVariant::fromValue(queue->bindableStatus().value());
    case RoleNames::QueueStatusStringRole:
        return queue->bindableStatusString().value();
    case RoleNames::DetailsRole:
        return QVariant::fromValue(bill->details());
    case RoleNames::PaymentRole:
        return QVariant::fromValue(bill->payment());
    case RoleNames::DateRole:
        return bill->date();
    case RoleNames::StatusRole:
        return QVariant::fromValue(bill->orderStatus());
    case RoleNames::CustomerRole:
        return QVariant::fromValue(bill->customer());
    case RoleNames::TakenRole:
        return queue->bindableTaken().value();
    case RoleNames::DoneRole:
        return bill->done();
    case RoleNames::OrderIdRole:
        return bill->orderId();
    case RoleNames::ProductQuantityRole:
        return bill->quantityOfProducts();
    case RoleNames::TotalToPayRole:
        return bill->totalToPay();
    }
}

bool OrderQueueModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (not index.isValid())
        return false;

    OrderQueue *entity{data_[index.row()].get()};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CommentRole:
    case RoleNames::CreatedAtRole:
    case RoleNames::SalespersonRole:
    case RoleNames::OrderRole:
    case RoleNames::QueueStatusIdRole:
        entity->bindableStatus()
                .setValue(static_cast<OrderQueue::Status>(value.toInt()));
        emit dataChanged(index, index, {role, int(RoleNames::QueueStatusStringRole)});
        Q_ASSERT(entity->bindableStatus().value() == static_cast<OrderQueue::Status>(value.toInt()));
        return true;
    case RoleNames::QueueStatusStringRole:
    case RoleNames::DetailsRole:
    case RoleNames::PaymentRole:
    {
        auto &&newPayment{value.value<Entity::Payment>()};
        entity->bindableBill().value()->setPayment(newPayment);
        emit dataChanged(index, index, {role});
        return true;
    }
    case RoleNames::DateRole:
    case RoleNames::StatusRole:
    case RoleNames::CustomerRole:
        break;
    case RoleNames::TakenRole:
    {
        entity->bindableTaken().setValue(value.toBool());
        emit dataChanged(index, index, {role});
        Q_ASSERT(entity->bindableTaken().value() == value.toBool());
        return true;
    }
    case RoleNames::DoneRole:
    case RoleNames::OrderIdRole:
    case RoleNames::ProductQuantityRole:
    case RoleNames::TotalToPayRole:
        break;
    }
    return false;
}

QHash<int, QByteArray> OrderQueueModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(CommentRole), "comment"_ba},
        {int(CreatedAtRole), "createdAt"_ba},
        {int(SalespersonRole), "salesperson"_ba},
        {int(OrderRole), "order"_ba},
        {int(QueueStatusIdRole), "queueStatusId"_ba},
        {int(QueueStatusStringRole), "queueStatusString"_ba},
        {int(DetailsRole), "details"_ba},
        {int(OrderIdRole), "orderId"_ba},
        {int(PaymentRole), "payment"_ba},
        {int(DateRole), "date"_ba},
        {int(StatusRole), "status"_ba},
        {int(CustomerRole), "customer"_ba},
        {int(TakenRole), "taken"_ba},
        {int(DoneRole), "done"_ba},
        {int(ProductQuantityRole), "productQuantity"_ba},
        {int(TotalToPayRole), "totalToPay"_ba}
    };
}

QBindable<QueueRepository *> OrderQueueModel::bindableRepository()
{
    return &repository_;
}

QString OrderQueueModel::statusIdToString(OrderQueue::Status s) const
{
    return OrderQueue::statusToString(s);
}

OrderQueue::Status OrderQueueModel::statusStringToId(const QString &s)
{
    return OrderQueue::statusStringToId(s);
}

void OrderQueueModel::queueFetched(const QJsonArray &queue)
{
    qDebug() << "OrderQueueModel::queueFetched: setting data...";

    beginResetModel();
    data_.clear();
    for (const auto &jsonValue: queue)
        data_.push_back(std::move(Billing::OrderQueue::fromJson(jsonValue.toObject())));
    endResetModel();
}

} // namespace Billing
