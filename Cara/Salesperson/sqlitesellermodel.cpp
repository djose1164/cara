#include "sqlitesellermodel.h"

#include "salesperson.h"

#include <QSqlQuery>
#include <QSqlError>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto SELECT_SQL{"select forename||' '||surname as fullname, forename, surname, id"
                          " from seller"_L1};
constexpr auto INSERT_SQL{"insert into seller(forename, surname)"
                          " values(:forename, :surname)"_L1};
}

namespace Seller
{

SQLiteSellerModel::SQLiteSellerModel(QObject *parent)
    : QSqlQueryModel{parent}
{
    refresh();
}

QVariant SQLiteSellerModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    const int columnIdx{role - Qt::UserRole - 1};
    const QModelIndex modelIdx{this->index(index.row(), columnIdx)};

    return QSqlQueryModel::data(modelIdx);
}

QHash<int, QByteArray> SQLiteSellerModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(FullnameRole), "fullname"_ba},
        {int(ForenameRole), "forename"_ba},
        {int(SurnameRole), "surname"_ba},
        {int(SellerIdRole), "sellerId"_ba},
    };
}

void SQLiteSellerModel::refresh()
{
    setQuery(SELECT_SQL);
    emit countChanged();
}

void SQLiteSellerModel::insert(int idx, const Entity::Salesperson &seller)
{
    Q_UNUSED(idx);
    Q_UNUSED(seller);
}

void SQLiteSellerModel::append(const QVariantMap &seller)
{
    QSqlQuery query;
    query.prepare(INSERT_SQL);
    query.bindValue(":forename"_L1, seller["forename"_L1].toString());
    query.bindValue(":surname"_L1, seller["surname"_L1].toString());

    if (query.exec())
    {
        refresh();
        emit sellerAdded(query.lastInsertId().toInt());
    }
}

int SQLiteSellerModel::count() const
{
    return rowCount();
}

void SQLiteSellerModel::setCount(int newCount)
{
    if (count_ == newCount)
        return;
    count_ = newCount;
    emit countChanged();
}

void SQLiteSellerModel::setSellers(const QList<Entity::Salesperson> &sellerList)
{
}

} // namespace Seller
