#include "orderentity.h"
#include "productentity.h"

using namespace EntityLayer;

OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
{
}

QQmlListProperty<ProductEntity> OrderEntity::products()
{
    return {
        this, this,
        &OrderEntity::append,
        &OrderEntity::count,
        &OrderEntity::product,
        &OrderEntity::clear,
        &OrderEntity::replace,
        &OrderEntity::popBack
    };
}

qsizetype OrderEntity::count() const
{
    return products_.count();
}

ProductEntity *OrderEntity::product(qsizetype index) const
{
    return products_[index];
}

QVariantMap OrderEntity::toMap() const
{
    return {};
}

void OrderEntity::appendProduct(ProductEntity *product)
{
    products_.append(product);
}

void OrderEntity::clear()
{
    products_.clear();
}

void OrderEntity::replaceProduct(qsizetype index, ProductEntity *detail)
{
    products_.replace(index, detail);
}

void OrderEntity::popBack()
{
    products_.pop_back();
}

void OrderEntity::append(QQmlListProperty<ProductEntity> *list, ProductEntity *detail)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    entity->appendProduct(detail);
}

qsizetype OrderEntity::count(QQmlListProperty<ProductEntity> *list)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    return entity->count();
}

void OrderEntity::clear(QQmlListProperty<ProductEntity> *list)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    entity->clear();
}

void OrderEntity::replace(QQmlListProperty<ProductEntity> *list, qsizetype index, ProductEntity *detail)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    entity->replaceProduct(index, detail);
}

void OrderEntity::popBack(QQmlListProperty<ProductEntity> *list)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    entity->popBack();
}

ProductEntity *OrderEntity::product(QQmlListProperty<ProductEntity> *list, qsizetype index)
{
    const auto entity{qobject_cast<OrderEntity *>(list->object)};
    return entity->product(index);
}

