#include "commissionmodel.h"
#include "pricingservice.h"
#include "sqliteproductmodel.h"

#include <QJsonDocument>

using namespace Qt::StringLiterals;

namespace Billing
{

CommissionModel::CommissionModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
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
        connect(repository_, &CommissionRepository::errorOccurred, this, &CommissionModel::errorOccurred);
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

void CommissionModel::commissionsFetched(const QList<Billing::Commission> &commissionList)
{
    beginResetModel();
    data_ = commissionList;
    endResetModel();

    setBusy(false);
    emit modelUpdated();
}

void CommissionModel::commissionAdded(const Billing::Commission &commission)
{
    if (!repository_->supportsServerSideCalculation())
    {
        clear();
        fetchCommissions();

        setBusy(false);
        emit commissionAddedSuccessfully();
        return;
    }

    const qsizetype count{data_.count()};
    beginInsertRows(QModelIndex{}, count, count);
    data_.append(commission);
    endInsertRows();

    setBusy(false);
    emit commissionAddedSuccessfully();
}

void CommissionModel::commissionPatched(const QList<int> &roles)
{
    Q_UNUSED(roles);
}

void CommissionModel::setBusy(bool newBusy)
{
    busy_ = newBusy;
}

void CommissionModel::updateCommissionUnit(QList<CommissionItem> &ciList)
{
    auto productModel{std::make_unique<ProductInventory::SQLiteProductModel>()};
    const auto pricingService{std::make_unique<PricingService>(productModel.get())};

    std::ranges::for_each(ciList, [&](auto &ci) {
        const auto sellPrice{pricingService->sellPriceForProduct(ci.productId())};
        const auto buyPrice{pricingService->buyPriceForProduct(ci.productId())};
        ci.setUnitCommission(sellPrice - buyPrice);
    });
}

bool CommissionModel::busy() const
{
    return busy_;
}

QBindable<bool> CommissionModel::bindableBusy()
{
    return &busy_;
}

QAbstractItemModel *CommissionModel::itemModel()
{
    return static_cast<QAbstractItemModel *>(this);
}

void CommissionModel::scheduleDelete()
{
    deleteLater();
}

void CommissionModel::fetchCommissionsBySellerId(SellerId sellerId)
{
    if (repository_)
        repository_->fetchCommissionsBySellerId(sellerId);
}

void CommissionModel::clear()
{
    beginResetModel();
    data_.clear();
    endResetModel();
}

int CommissionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
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
        return entity.getGrossProfit();
    case RoleNames::UniqueCodeRole:
        return entity.uniqueCode();
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
        Commission c = entity;
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
        {int(GrossProfitRole), "grossProfit"_ba},
        {int(UniqueCodeRole), "uniqueCode"_ba}
    };
}

void CommissionModel::append(const QVariantMap &map)
{
    if (not repository_)
        return;

    try
    {
        Commission::validateMap(map);
        auto commission{Commission::fromMap(map)};
        if (!repository_->supportsServerSideCalculation())
        {
            updateCommissionUnit(commission.commissionItems());
            commission.calculateAmount();
        }

        repository_->addCommission(commission);

        setBusy(true);
    }
    catch (const std::invalid_argument &err)
    {
        setBusy(false);
        emit errorOccurred(CommissionRepository::Error::AddFailure, err.what());
    }
}

} // namespace Billing
