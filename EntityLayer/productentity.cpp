#include "productentity.h"

using namespace EntityLayer;

ProductEntity::ProductEntity(QObject *parent)
    : QObject{parent}
    , name_{}
    , description_{}
    , price_{}
    , productId_{}
    , imageUrl_{}
{
}

ProductEntity *ProductEntity::fromMap(const QVariantMap &map)
{
    const auto product{new ProductEntity};
    product->setName(map["name"].toString());
    product->setImageUrl(map["image_url"].toString());
    product->setPrice(map["sell_price"].toInt());

    return product;
}

bool ProductEntity::operator==(ProductEntity * const rhs)
{
    return productId_ == rhs->productId();
}

bool ProductEntity::operator!=(ProductEntity * const rhs)
{
    return !(*this == rhs);
}

QString ProductEntity::name() const
{
    return name_;
}

QString ProductEntity::description() const
{
    return description_;
}

QString ProductEntity::imageUrl() const
{
    return imageUrl_;
}

int ProductEntity::price() const
{
    return price_;
}

QBindable<int> ProductEntity::bindablePrice() const
{
    return &price_;
}

int ProductEntity::productId() const
{
    return productId_;
}

QByteArray ProductEntity::toJson() const
{
    QJsonObject obj{};
    obj["product_id"] = productId_;
    obj["name"] = name_;
    obj["description"] = description_;
    obj["image_url"] = imageUrl_;
    obj["price"] = price();

    QJsonDocument doc{obj};
    return doc.toJson(QJsonDocument::Compact);
}

void ProductEntity::setName(const QString &name)
{
    if (name_ != name)
    {
        name_ = name;
        emit nameChanged();
    }
}

void ProductEntity::setDescription(const QString &description)
{
    if (description_ != description)
    {
        description_ = description;
        emit descriptionChanged();
    }
}

void ProductEntity::setImageUrl(const QString &url)
{
    if (imageUrl_ != url)
    {
        imageUrl_ = url;
        emit imageUrlChanged();
    }
}

void ProductEntity::setPrice(int price)
{
    price_ = price;

    // FIXME: Move this to QObjectBindableProperty
    //emit priceChanged();
}

void ProductEntity::setProductId(int productId)
{
    if (productId_ != productId)
    {
        productId_ = productId;
        emit productIdChanged();
    }
}
