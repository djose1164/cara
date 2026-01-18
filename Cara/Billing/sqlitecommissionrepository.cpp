#include "sqlitecommissionrepository.h"
#include "commission.h"

#include <QSqlQuery>
#include <QSqlError>

using namespace Qt::StringLiterals;


// Follow order in RoleNames
constexpr auto REFRESH_SQL{"select c.id, c.unique_code, c.admin_id, seller_id, rate, amount, "
                           "payment_date, created_at, s.forename||' '||s.surname as seller_name,"
                           " u.forename as admin_forename, u.surname as admin_surname"
                           " from commission c"
                           " join seller s on s.id = c.seller_id"
                           " join user u on u.admin_id = c.admin_id"_L1};
constexpr auto SELECT_COMMISSION_ITEMS{"select product_id, quantity, unit_commission, supplier_id, product_name"
                                       " from vw_commission_detail"
                                       " where commission_uuid = :c_uuid"_L1};
constexpr auto INSERT_COMMISSION{
    "INSERT INTO commission"
    "(unique_code, rate, amount, payment_date, admin_id, seller_id) "
    "values(:unique_code, :rate, :amount, :payment_date, :admin_id, :seller_id)"_L1};

constexpr auto INSERT_COMMISSION_ITEM{
    "INSERT INTO commission_item"
    "(unique_code, commission_id, product_id, quantity, unit_commission, supplier_id, "
    "commission_uuid)"
    "values(:unique_code, :commission_id, :product_id, :quantity, :unit_commission, :supplier_id, :commission_uuid)"_L1};

constexpr auto UPDATE_COMMISSION{R"(
    update commission
    set payment_date = :paymentDate,
        id = :id,
        rate = :rate,
        amount = :amount,
        admin_id = :adminId,
        seller_id = :sellerId
    where unique_code = :uniqueCode)"_L1
};

namespace Billing
{

SQLiteCommissionRepository::SQLiteCommissionRepository(QObject *parent)
    : Billing::CommissionRepository{parent}
{
}

void SQLiteCommissionRepository::fetchCommissionsBySellerId(int spId)
{
}

void SQLiteCommissionRepository::fetchCommissions()
{
    QSqlQuery query;
    if (not query.exec(REFRESH_SQL))
    {
        emit errorOccurred(Error::FetchFailure, query.lastError().text());
        return;
    }

    QList<Commission> commissionList;
    //select c.id, c.unique_code, admin_id, seller_id, rate, amount, payment_date, created_at, seller_name, admin_forename, surname
    while (query.next())
    {
        Commission c;
        c.setCommissionId(query.value(0).toInt());
        c.setUniqueCode(query.value(1).toUuid());
        c.setAdminId(query.value(2).toInt());
        c.setSalespersonId(query.value(3).toInt());
        c.setRate(query.value(4).toDouble());
        c.setAmount(query.value(5).toDouble());
        c.setPaymentDate(query.value(6).toDateTime());
        c.setCreatedAt(query.value(7).toDateTime());

        Entity::Salesperson seller;
        seller.setSalespersonId(c.salespersonId());
        seller.setForename(query.value(8).toString());
        c.setSalesperson(std::move(seller));

        Entity::UserEntity admin;
        qDebug() << query.value(9).toString() << " " << query.value(11).toString();
        admin.setForename(query.value(9).toString());
        admin.setSurname(query.value(10).toString());
        admin.setUserId(c.adminId());
        c.setAdmin(std::move(admin));

        auto commissionItems{fetchCommissionItems(c.uniqueCode())};
        if (commissionItems)
            c.setCommissionItems(std::move(*commissionItems));

        commissionList.append(std::move(c));
    }
    emit fetched(commissionList);
}

void SQLiteCommissionRepository::addCommission(const Commission &commission)
{
    if (not insertCommission(commission))
    {
        const QString msg{"Couldn't add commission!"_L1};
        emit errorOccurred(Error::AddFailure, msg);
        return;
    }
}

void SQLiteCommissionRepository::patchCommission(const Commission &commission,
                                                 std::function<void()> callback)
{
    const auto fail = [&](const QString &errmsg) {
        qWarning() << "SQLiteCommissionRepository::patchCommission: " << errmsg;
        emit errorOccurred(Error::PatchFailure, errmsg);
    };

    QSqlQuery query;
    if (not query.prepare(UPDATE_COMMISSION))
    {
        const QString errmsg{query.lastError().text()};
        fail(errmsg);
        return;
    }

    query.bindValue(":paymentDate"_L1, commission.paymentDate());
    query.bindValue(":uniqueCode"_L1, commission.uniqueCode());
    const auto commissionId = commission.commissionId() > 0 ? commission.commissionId() : QVariant{};
    query.bindValue(":id"_L1, commissionId);
    query.bindValue(":rate"_L1, commission.rate());
    query.bindValue(":sellerId"_L1, commission.salespersonId());
    query.bindValue(":adminId"_L1, commission.adminId());
    query.bindValue(":amount"_L1, commission.amount());

    if (query.exec())
    {
        if (callback)
            callback();
    }
    else
    {
        const QString errmsg{query.lastError().text()};
        fail(errmsg);
    }
}

bool SQLiteCommissionRepository::supportsServerSideCalculation() const
{
    return false;
}

bool SQLiteCommissionRepository::insertCommission(const Commission &commission)
{
    QSqlQuery query;
    if (not query.prepare(INSERT_COMMISSION))
    {
        qDebug() << "SQLiteCommissionModel::insertCommission: failed to prepare query. Error: "
                 << query.lastError();
        return false;
    }

    query.bindValue(":unique_code"_L1, commission.uniqueCode());
    query.bindValue(":rate"_L1, commission.rate());
    query.bindValue(":amount"_L1, commission.amount());
    query.bindValue(":payment_date"_L1, commission.paymentDate());
    query.bindValue(":admin_id"_L1, commission.adminId());
    query.bindValue(":seller_id"_L1, commission.salespersonId());

    bool success{true};
    unsigned failedInserts{};
    if (query.exec()) // If commission inserted, asign the commission_id to each item.
    {
        std::ranges::for_each(commission.commissionItems(), [&](CommissionItem &ci) {
            ci.setCommissionId(commission.commissionId());
            ci.setCommissionUuid(commission.uniqueCode());
            const bool lastResult{insertCommissionItem(ci)};
            success = success and lastResult;
            if (not lastResult)
            {
                ++failedInserts;
                qWarning() << "Failed to inser CommissionItem: "
                           << QSqlDatabase::database().lastError().text();
            }
        });
    }
    else
    {
        qCritical() << "SQLiteCommissionModel::insertCommission: couldn't insert commission: "
                    << commission << ": " << query.lastError();
        emit errorOccurred(Error::CommissionInsertFailed,
                           "Couldn't insert %1 commissions: SQL error: %2"_L1
                               .arg(QString::number(failedInserts),
                                    query.lastError().text()));
        return false;
    }


    if (success)
        emit added(commission);
    else
        emit errorOccurred(Error::ItemInsertFailed,
                           "Couldn't insert commission item! Error: %1"_L1.arg(
                               query.lastError().text()));
    return success;
}

bool SQLiteCommissionRepository::insertCommissionItem(const CommissionItem &ci)
{
    QSqlQuery query;
    if (not query.prepare(INSERT_COMMISSION_ITEM))
        return false;

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

// TODO: This should be refactored into a new repository to handle commission items only.
std::optional<QList<CommissionItem>> SQLiteCommissionRepository::fetchCommissionItems(
    const QUuid &commissionId) const
{
    QSqlQuery query;
    if (not query.prepare(SELECT_COMMISSION_ITEMS))
    {
        qDebug() << "SQLiteCommissionModel::fetchCommissionItems: could not prepare query. Error: "
                 << query.lastError();
        return std::nullopt;
    }
    query.bindValue(":c_uuid"_L1, commissionId);
    if (not query.exec())
    {
        constexpr auto msgerr{"Couldn't fetch fetch commissions items!"_L1};
        qWarning() << "SQLiteCommissionModel::fetchCommissionItems: " << msgerr;
        return std::nullopt;
    }

    // select product_id, quantity, unit_commission, supplier_id, productname
    QList<CommissionItem> ciList;
    while (query.next())
    {
        CommissionItem ci;
        ci.setProductId(query.value(0).toInt());
        ci.setQuantity(query.value(1).toInt());
        ci.setUnitCommission(query.value(2).toDouble());
        ci.setSupplierId(query.value(3).toInt());
        ci.setProductName(query.value(4).toString());
        ci.setCommissionUuid(commissionId);
        ciList.append(std::move(ci));
    }

    return ciList;
}

} // namespace Billing
