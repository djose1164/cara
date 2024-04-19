#include "detailsentity.h"

using EntityLayer::DetailsEntity;

DetailsEntity::DetailsEntity(QObject *parent)
    : QObject{parent}
    , quantity_{1}
    , total_{}
    , product_{}
{
    total_.setBinding([&] {
        return product().valid() ? product().price() * quantity_ : 0;
    });

    quantity_.setBinding([&] { return product().selectedQty(); });
}

int DetailsEntity::quantity() const
{
    return quantity_.value();
}

int DetailsEntity::total() const
{
    return total_.value();
}

const EntityLayer::Product &DetailsEntity::product() const
{
    return product_.value();
}

QByteArray DetailsEntity::toJson() const
{
    QJsonObject object{};
    object["quantity"] = quantity();
    const auto doc{QJsonDocument::fromJson(product().toJson())};
    const QJsonObject product{doc.object()};
    object["product"] = std::move(product);

    return QJsonDocument{object}.toJson(QJsonDocument::Compact);
}

void DetailsEntity::setProduct(const Product &product)
{
    product_ = product;
}

void DetailsEntity::setQuantity(int quantity)
{
    quantity_ = quantity;
}
