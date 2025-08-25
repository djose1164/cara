#include "sqlitecommissionmodel.h"
#include "ProductInventory/sqliteproductmodel.h"
#include "commission.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


using namespace Qt::StringLiterals;

namespace
{
// Follow order in RoleNames
constexpr auto REFRESH_SQL{"select c.id, c.unique_code, admin_id, seller_id, rate, amount, "
                           "payment_date, created_at, s.forename||' '||s.surname as seller_name"
                           " from commission c"
                           " join seller s on s.id = c.seller_id"_L1};
constexpr auto SELECT_COMMISSION_ITEMS{"select product_id, quantity, unit_commission, supplier_id"
                                       " from commission_item ci"
                                       " where commission_uuid = :c_uuid"_L1};
}

namespace Billing
{

SQLiteCommissionModel::SQLiteCommissionModel(QObject *parent)
    : QSqlQueryModel{parent}
    , productModel_{}
{
    fetchCommissions();
}

QVariant SQLiteCommissionModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    int cUuidColumn{int(RoleNames::UniqueCodeRole) - Qt::UserRole - 1};
    QModelIndex cUuidIdx{this->index(index.row(), cUuidColumn)};

    const int columnIdx{role - Qt::UserRole - 1};
    QModelIndex modelIdx{this->index(index.row(), columnIdx)};
    switch (static_cast<RoleNames>(role))
    {
        using enum RoleNames;
    case PaymentDateRole:
        return QSqlQueryModel::data(modelIdx).toDateTime();
    }

    if (static_cast<RoleNames>(role) == RoleNames::CommissionItemsRole)
    {
        QUuid commissionUuid{QSqlQueryModel::data(cUuidIdx).toUuid()};
        return QVariant::fromValue(fetchCommissionItems(commissionUuid));
    }
    else if (static_cast<RoleNames>(role) == RoleNames::GrossProfitRole)
    {
        QUuid commissionUuid{QSqlQueryModel::data(cUuidIdx).toUuid()};
        Commission c;
        c.setCommissionItems(fetchCommissionItems(commissionUuid));
        return c.calculateGrossProfit();
    }
    else if (static_cast<RoleNames>(role) == RoleNames::AdminNameRole)
    {
        QSettings s;
        s.beginGroup("User");
        return s.value("forename").toString() +QString(" ")+ s.value("surname").toString();
    }
    else if (static_cast<RoleNames>(role) == RoleNames::CommissionIdRole)
    {
        return QSqlQueryModel::data(cUuidIdx);
    }

    return QSqlQueryModel::data(modelIdx);
}

bool SQLiteCommissionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (static_cast<RoleNames>(role))
    {
        using enum RoleNames;
    case PaymentDateRole:
        QSqlQuery q;
        q.prepare("update commission set payment_date = :paymentDate"_L1);
        q.bindValue(":paymentDate"_L1, value);
        q.exec();
        emit dataChanged(index, index, {role});
        return true;
    }

    qDebug() << "SQLiteCommissionModel::setData: " << index << value;
    return QSqlQueryModel::setData(index, value, role);
}

QHash<int, QByteArray> SQLiteCommissionModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(AdminIdRole), "adminId"_ba},
        {int(AmountRole), "amount"_ba},
        {int(CommissionIdRole), "commissionId"_ba},
        {int(CommissionItemsRole), "commissionItems"_ba},
        {int(CreatedAtRole), "createdAt"_ba},
        {int(GrossProfitRole), "grossProfit"_ba},
        {int(PaymentDateRole), "paymentDate"_ba},
        {int(RateRole), "rate"_ba},
        {int(SellerIdRole), "sellerId"_ba},
        {int(SellerNameRole), "sellerName"_ba},
        {int(UniqueCodeRole), "uniqueCode"_ba},
        {int(AdminNameRole), "adminName"_ba},
    };
}

void SQLiteCommissionModel::append(const QVariantMap &map)
{
    try
    {
        if (not productModel_)
        {
            qInfo() << "SQLiteCommission::append: you need to setup a ProductModel.";
            return;
        }
        Commission::validateMap(map);
        Commission c = Commission::fromMap(map);
        updateCommissionUnit(c.commissionItems());
        insertCommission(c);
        fetchCommissions();
    }
    catch (const std::invalid_argument &err)
    {
        emit errorOccurred(Error::InvalidRequest, err.what());
    }
}

void SQLiteCommissionModel::insertCommission(const Commission &commission)
{
    QSqlQuery query;
    bool res = query.prepare(
        "INSERT INTO commission"
        "(unique_code, rate, amount, payment_date, admin_id, seller_id)"
        "values(:unique_code, :rate, :amount, :payment_date, :admin_id, :seller_id)"_L1);
    if (not res)
    {
        qDebug() << "SQLiteCommissionModel::insertCommission: failed to prepare query. Error: "
                 << query.lastError();
        return;
    }

    query.bindValue(":unique_code"_L1, commission.uniqueCode());
    query.bindValue(":rate"_L1, commission.rate());
    query.bindValue(":amount"_L1, commission.amount());
    query.bindValue(":payment_date"_L1, commission.paymentDate());
    query.bindValue(":admin_id"_L1, commission.adminId());
    query.bindValue(":seller_id"_L1, commission.salespersonId());

    bool success{};
    if (query.exec())
    {
        std::ranges::for_each(commission.commissionItems(), [&](CommissionItem &ci) {
            EntityLayer::Product p{productModel_->getProductById(ci.productId())};

            ci.setUnitCommission(p.price() - p.buyPrice());
            ci.setCommissionId(commission.commissionId());
            ci.setCommissionUuid(commission.uniqueCode());
            if (success = insertCommissionItem(ci); not success)
                qCritical() << QSqlDatabase::database().lastError();
        });
    }
    else
    {
        qCritical() << "SQLiteCommissionModel::insertCommission: couldn't insert commission: "
                     << commission << ": " << query.lastError();
    }

    if (success)
        emit commissionAddedSuccessfully();
    else
        emit errorOccurred(Error::ItemInsertFailed,
                           "Couldn't insert commission item! Error: %1"_L1
                               .arg(query.lastError().text()));
}

bool SQLiteCommissionModel::insertCommissionItem(const CommissionItem &ci)
{
    QSqlQuery query;
    query.prepare("INSERT INTO commission_item"
                  "(unique_code, commission_id, product_id, quantity, unit_commission, supplier_id, commission_uuid)"
                  "values(:unique_code, :commission_id, :product_id, :quantity, :unit_commission, :supplier_id, :commission_uuid)"_L1);
    query.bindValue(":unique_code"_L1, QUuid{}.createUuid());
    query.bindValue(":commission_id"_L1, (ci.commissionId() > 0 ? ci.commissionId() : QVariant{}));
    query.bindValue(":product_id"_L1, ci.productId());
    query.bindValue(":quantity"_L1, ci.quantity());
    query.bindValue(":unit_commission"_L1, ci.unitCommission());
    query.bindValue(":supplier_id"_L1, ci.supplierId());
    query.bindValue(":commission_uuid"_L1, ci.commissionUuid());
    return query.exec() || ([&]{ qCritical() << "Error commission_item:"
                                             << query.lastError() <<"\n"<< ci; return false; })();
}

void SQLiteCommissionModel::fetchCommissions()
{
    setQuery(REFRESH_SQL);

    if (lastError().isValid())
        qCritical() << "SQLiteCommissionModel::refresh: " << lastError();
}

QList<CommissionItem> SQLiteCommissionModel::fetchCommissionItems(const QUuid &commissionId) const
{
    QSqlQuery query;
    if (not query.prepare(SELECT_COMMISSION_ITEMS))
    {
        qDebug() << "SQLiteCommissionModel::fetchCommissionItems: could not prepare query. Error: "
                 << query.lastError();
        return {};
    }
    query.bindValue(":c_uuid"_L1, commissionId);
    query.exec();

    // select product_id, quantity, unit_commission, supplier_id
    QList<CommissionItem> ciList;
    while (query.next())
    {
        CommissionItem ci;
        ci.setProductId(query.value(0).toInt());
        ci.setQuantity(query.value(1).toInt());
        ci.setUnitCommission(query.value(2).toDouble());
        ci.setSupplierId(query.value(3).toInt());
        ci.setCommissionUuid(commissionId);
        ciList.append(std::move(ci));
    }

    return ciList;
}

void SQLiteCommissionModel::updateCommissionUnit(QList<CommissionItem> &ciList)
{
    ProductInventory::SQLiteProductModel productModel;
    std::ranges::for_each(ciList, [&](auto &ci) {
        QModelIndex idx{productModel.find(ci.productId())};
        const auto product{productModel.get(idx.row())};
        ci.setUnitCommission(product.price() - product.buyPrice());
    });
}

ProductInventory::SQLiteProductModel *SQLiteCommissionModel::productModel() const
{
    return productModel_;
}

void SQLiteCommissionModel::setProductModel(ProductInventory::SQLiteProductModel *newProductModel)
{
    if (productModel_ == newProductModel)
        return;
    productModel_ = newProductModel;
    emit productModelChanged();
}

QAbstractItemModel *SQLiteCommissionModel::itemModel()
{
    return static_cast<QAbstractItemModel *>(this);
}

void SQLiteCommissionModel::scheduleDelete()
{
    deleteLater();
}

void SQLiteCommissionModel::fetchCommissionsBySellerId(SellerId sellerId)
{
    qDebug() << "SQLiteCommissionModel::fetchCommissionsBySellerId: " << sellerId;
}

} // namespace Billing

