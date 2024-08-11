#include "detailsentity.h"

using EntityLayer::DetailsEntity;
using namespace Qt::StringLiterals;

DetailsEntity *DetailsEntity::fromJson(const QJsonObject &data)
{
    const auto newDetail{new DetailsEntity};
    newDetail->setQuantity(data["quantity"_L1].toInt());
    newDetail->price_ = data["price"_L1].toInt();
    newDetail->setProduct(
        Product::fromMap(data["product"_L1].toVariant().toMap())
    );

    return newDetail;
}

DetailsEntity::DetailsEntity(QObject *parent)
    : QObject{parent}
    , quantity_{1}
    , price_{}
    , total_{}
    , product_{}
{
    total_.setBinding([&] {
        return product().valid() ? price_ * quantity_ : 0;
    });

    quantity_.setBinding([&] { return product().selectedQty(); });
}

int DetailsEntity::quantity() const
{
    return quantity_.value();
}

int DetailsEntity::price() const
{
    return price_;
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
