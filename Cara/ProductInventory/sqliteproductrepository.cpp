#include "sqliteproductrepository.h"

#include <QSqlQuery>
#include <QSqlError>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto SELECT_SQL{"select product_id, name, description, category_id, price, buy_price, image_url "
                          "from product_offline"_L1};
}

namespace ProductInventory
{

SQLiteProductRepository::SQLiteProductRepository(QObject *parent)
    : IProductRepository{parent}
{
}

} // namespace ProductInventory

void ProductInventory::SQLiteProductRepository::add(const EntityLayer::Product &product)
{
}

void ProductInventory::SQLiteProductRepository::update()
{
    QSqlQuery query;
    if (not query.exec(SELECT_SQL))
    {
        emit errorOccurred(Error::FailedToFetch, query.lastError().text());
        return;
    }

    QList<EntityLayer::Product> productList;
    while (query.next())
    {
        EntityLayer::Product p;
        p.setProductId(query.value(0).toInt());
        p.setName(query.value(1).toString());
        p.setDescription(query.value(2).toString());
        p.setCategoryId(query.value(3).toInt());
        p.setPrice(query.value(4).toInt());
        p.setBuyPrice(query.value(5).toInt());
        p.setImageUrl(query.value(6).toString());
        productList.append(std::move(p));
    }
    emit fetched(productList);
}

void ProductInventory::SQLiteProductRepository::remove(ProductId productId)
{
}
