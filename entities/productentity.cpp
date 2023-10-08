#include "productentity.h"

namespace EntityLayer {
Product::Product(QObject *parent)
    : QObject{parent}
    , name_{}
    , description_{}
    , price_{}
    , productId_{}
    , imageUrl_{}
{
}

Product *Product::fromMap(const QVariantMap &map)
{
    const auto product{new Product};
    product->setName(map["name"].toString());
    product->setImageUrl(map["image_url"].toString());
    product->setPrice(map["sell_price"].toInt());

    return product;
}

bool Product::operator==(Product * const rhs)
{
    return productId_ == rhs->productId();
}

bool Product::operator!=(Product * const rhs)
{
    return !(*this == rhs);
}

QString Product::name() const
{
    return name_;
}

QString Product::description() const
{
    return description_;
}

QString Product::imageUrl() const
{
    return imageUrl_;
}

int Product::price() const
{
    return price_;
}

QBindable<int> Product::bindablePrice() const
{
    return &price_;
}

int Product::productId() const
{
    return productId_;
}

QByteArray Product::toJson() const
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

void Product::setName(const QString &name)
{
    if (name_ != name)
    {
        name_ = name;
        emit nameChanged();
    }
}

void Product::setDescription(const QString &description)
{
    if (description_ != description)
    {
        description_ = description;
        emit descriptionChanged();
    }
}

void Product::setImageUrl(const QString &url)
{
    if (imageUrl_ != url)
    {
        imageUrl_ = url;
        emit imageUrlChanged();
    }
}

void Product::setPrice(int price)
{
    price_ = price;

    // FIXME: Move this to QObjectBindableProperty
    //emit priceChanged();
}

void Product::setProductId(int productId)
{
    if (productId_ != productId)
    {
        productId_ = productId;
        emit productIdChanged();
    }
}
}
