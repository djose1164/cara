#include "detailsentity.h"

using EntityLayer::DetailsEntity;

DetailsEntity::DetailsEntity(QObject *parent)
    : QObject{parent}
    , quantity_{}
    , total_{}
    , product_{new ProductEntity(this)}
{
     total_.setBinding([&] {
            return product_->price() * quantity_ ;
    });
}

int DetailsEntity::quantity() const
{
    return quantity_.value();
}

int DetailsEntity::total() const
{
    return total_.value();
}

EntityLayer::ProductEntity *DetailsEntity::product() const
{
    return product_;
}

QByteArray DetailsEntity::toJson() const
{
    QJsonObject object{};
    object["quantity"] = quantity();
    const QJsonObject product{QJsonDocument::fromJson(product_->toJson()).object()};
    object["product"] = product;

    return QJsonDocument(object).toJson(QJsonDocument::Compact);
}

void DetailsEntity::setProduct(ProductEntity * const product)
{
    product->setParent(this);
    product_ = product;
}

void DetailsEntity::setQuantity(int quantity)
{
    quantity_ = quantity;
}
