#include "sqliteproductmodel.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


using namespace Qt::StringLiterals;

namespace
{
constexpr auto SELECT_SQL{"select product_id, name, description, category_id, price, buy_price, image_url "
                          "from product_offline"_L1};
}


namespace ProductInventory
{

SQLiteProductModel::SQLiteProductModel(QObject *parent)
    : QSqlQueryModel{parent}
    , AbstractProductModel{}
{
    refresh();
}

QVariant SQLiteProductModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    if (role < Qt::UserRole)
        return QSqlQueryModel::data(index, role);

    const int columnIdx{role - Qt::UserRole - 1};
    const QModelIndex modelIdx{this->index(index.row(), columnIdx)};
    return QSqlQueryModel::data(modelIdx, Qt::DisplayRole);
}

bool SQLiteProductModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return {};
}

QHash<int, QByteArray> SQLiteProductModel::roleNames() const
{
    return {
        {int(RoleNames::ProductIdRole), "productId"_ba},
        {int(RoleNames::ProductNameRole), "productName"_ba},
        {int(RoleNames::DescriptionRole), "description"_ba},
        {int(RoleNames::PriceRole), "price"_ba},
        {int(RoleNames::BuyPriceRole), "buyPrice"_ba},
        {int(RoleNames::ImageUrlRole), "imageUrl"_ba},
        {int(RoleNames::CategoryIdRole), "categoryId"_ba},
    };
}

void SQLiteProductModel::refresh()
{
    setQuery(SELECT_SQL);
}

EntityLayer::Product SQLiteProductModel::getProductById(ProductId productId) const
{
    return get(find(productId).row());
}

EntityLayer::Product SQLiteProductModel::get(int row) const
{
    using enum RoleNames;
    EntityLayer::Product p;
    p.setProductId(valueAt(row, ProductIdRole).toInt());
    p.setName(valueAt(row, ProductNameRole).toString());
    p.setPrice(valueAt(row, PriceRole).toString().toDouble());
    p.setBuyPrice(valueAt(row, BuyPriceRole).toString().toDouble());
    p.setImageUrl(valueAt(row, ImageUrlRole).toString());

    return p;
}

bool SQLiteProductModel::contains(const EntityLayer::Product &product) const
{
    for (int i{}; i < rowCount(); ++i)
    {
        if (record(i).value(0).toInt() == product.productId())
            return true;
    }
    return false;
}

QModelIndex SQLiteProductModel::find(int productId) const
{
    using enum RoleNames;
    const auto columnIdx{int(ProductIdRole) - Qt::UserRole - 1};
    for (int i{}; i < rowCount(); ++i)
    {
        const auto modelIdx{index(i, columnIdx)};
        if (int foundProductId{data(modelIdx, Qt::DisplayRole).toInt()};
                productId == foundProductId)
        {
            return modelIdx;
        }
    }
    return {};
}

QJsonArray SQLiteProductModel::toJsonArray() const
{
    QJsonArray array;
    for (int i{}; i < rowCount(); ++i)
    {
        array.append(get(i).toJsonObject());
    }

    return array;
}

QModelIndex SQLiteProductModel::indexOf(int row, RoleNames role) const
{
    return index(row, columnOf(role));
}

QVariant SQLiteProductModel::valueAt(int row, RoleNames role) const
{
    return QSqlQueryModel::data(indexOf(row, role));
}

int SQLiteProductModel::columnOf(RoleNames role)
{
    return int(role) - Qt::UserRole - 1;
}

} // namespace ProductInventory
