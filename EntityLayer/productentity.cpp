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

int ProductEntity::productId() const
{
    return productId_;
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
    if (price_ != price)
    {
        price_ = price;
        emit priceChanged();
    }
}

void ProductEntity::setProductId(int productId)
{
    if (productId_ != productId)
    {
        productId_ = productId;
        emit productIdChanged();
    }
}
