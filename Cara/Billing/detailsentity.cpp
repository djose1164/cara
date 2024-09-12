#include "detailsentity.h"

using EntityLayer::DetailsEntity;
using namespace Qt::StringLiterals;

DetailsEntity *DetailsEntity::fromJson(const QJsonObject &data)
{
    const auto newDetail{new DetailsEntity};
    newDetail->setQuantity(data["quantity"_L1].toInt());
    Q_ASSERT(newDetail->quantity());

    newDetail->price_ = data["product"_L1]["sell_price"_L1].toString().toDouble();
    Q_ASSERT(newDetail->price() > 0);
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
        return product().valid() ? product().price() * quantity_ : 0;
    });

    quantity_.setBinding([&] { return product().selectedQty(); });
}

int DetailsEntity::quantity() const
{
    return quantity_.value();
}

double DetailsEntity::price() const
{
    return price_;
}

double DetailsEntity::total() const
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
