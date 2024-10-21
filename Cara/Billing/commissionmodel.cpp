#include "commissionmodel.h"

#include <QJsonDocument>

using namespace Qt::StringLiterals;

namespace Billing
{

CommissionModel::CommissionModel(QObject *parent)
    : QAbstractListModel{parent}
    , repository_{}
    , busy_{}
{
}

CommissionRepository *CommissionModel::repository() const
{
    return repository_;
}

void CommissionModel::setRepository(CommissionRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    if (repository_)
        repository_->disconnect(this);
    repository_ = newRepository;
    if (repository_)
    {
        connect(repository_, &CommissionRepository::errorOccuerred, this, &CommissionModel::errorOccurred);
        connect(repository_, &CommissionRepository::fetched, this, &CommissionModel::commissionsFetched);
        connect(repository_, &CommissionRepository::added, this, &CommissionModel::commissionAdded);
    }
    emit repositoryChanged();
}

void CommissionModel::fetchCommissions()
{
    if (!repository_)
        return;

    repository_->fetchCommissions();
    setBusy(true);
}

void CommissionModel::commissionsFetched(const QList<Commission> &commissionList)
{
    beginResetModel();
    data_ = commissionList;
    endResetModel();

    setBusy(false);
}

void CommissionModel::commissionAdded(const Commission &commission)
{
    const qsizetype count{data_.count()};
    beginInsertRows(QModelIndex{}, count, count);
    data_.append(commission);
    endInsertRows();

    setBusy(false);
    emit commissionAddedSuccessfully();
}

void CommissionModel::commissionPatched(const QList<int> &roles)
{
    // emit dataChanged()
}

void CommissionModel::setBusy(bool newBusy)
{
    busy_ = newBusy;
}

bool CommissionModel::busy() const
{
    return busy_;
}

QBindable<bool> CommissionModel::bindableBusy()
{
    return &busy_;
}

int CommissionModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant CommissionModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    const auto &entity{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CommissionIdRole:
        return entity.commissionId();
    case RoleNames::AdminIdRole:
        return entity.adminId();
    case RoleNames::SalespersonIdRole:
        return entity.salespersonId();
    case RoleNames::RateRole:
        return entity.rate();
    case RoleNames::AmountRole:
        return entity.amount();
    case RoleNames::PaymentDateRole:
        return entity.paymentDate();
    case RoleNames::CreatedAtRole:
        return entity.createdAt();
    case RoleNames::SellerIdRole:
        return entity.salesperson().salespersonId();
    case RoleNames::SellerNameRole:
        return entity.salesperson().fullname();
    case RoleNames::AdminNameRole:
        return entity.admin().fullname();
    case RoleNames::CommissionItemsRole:
        Q_ASSERT(entity.commissionItems().count() > 0);
        return QVariant::fromValue(entity.commissionItems());
    case RoleNames::GrossProfitRole:
        return entity.calculateGrossProfit();
    }
}

bool CommissionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (not index.isValid())
        return false;

    auto &entity{data_[index.row()]};
    switch (static_cast<RoleNames>(role))
    {
    using enum RoleNames;
    case PaymentDateRole:
    {
        Commission c;
        c.setCommissionId(entity.commissionId());
        auto &&paymentDate{value.toDateTime()};
        c.setPaymentDate(paymentDate.toUTC());
        setBusy(true);
        repository_->patchCommission(c,
                                     [this, index, role, &entity, c]
                                     {
                                        qDebug() << "CommissionModel::setData: emitting dataChanged";
                                        entity.setPaymentDate(c.paymentDate());
                                        setBusy(false);
                                        emit dataChanged(index, index, {role});
                                     });
    }
    default:
        return false;
    }

    return true;
}

QHash<int, QByteArray> CommissionModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(CommissionIdRole), "commissionId"_ba},
        {int(AdminIdRole), "adminId"_ba},
        {int(SalespersonIdRole), "salespersonId"_ba},
        {int(RateRole), "rate"_ba},
        {int(AmountRole), "amount"_ba},
        {int(PaymentDateRole), "paymentDate"_ba},
        {int(CreatedAtRole), "createdAt"_ba},
        {int(SellerIdRole), "sellerId"_ba},
        {int(SellerNameRole), "sellerName"_ba},
        {int(AdminNameRole), "adminName"_ba},
        {int(CommissionItemsRole), "commissionItems"_ba},
        {int(GrossProfitRole), "grossProfit"_ba}
    };
}

void CommissionModel::append(const QVariantMap &map)
{
    if (not repository_)
        return;

    try
    {
        Commission::validateMap(map);
        repository_->addCommission(Commission::fromMap(map));
        setBusy(true);
    }
    catch (const std::invalid_argument &err)
    {
        emit errorOccurred(err.what(), CommissionRepository::Error::FailedToAdd);
    }
}

} // namespace Billing
