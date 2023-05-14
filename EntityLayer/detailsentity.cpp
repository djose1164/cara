#include "detailsentity.h"

using EntityLayer::DetailsEntity;

DetailsEntity::DetailsEntity(QObject *parent)
    : QObject{parent}
    , quantity_{}
    , product_{new ProductEntity(this)}
{
}

DetailsEntity::DetailsEntity(const DetailsEntity &rhs, QObject *parent)
    : QObject{parent}
    , quantity_{rhs.quantity()}
    , product_{new ProductEntity(this)}
{
    const auto otherProduct{rhs.product()};
    product_ = new ProductEntity(this);
    product_->setName(otherProduct->name());
    product_->setDescription(otherProduct->description());
    product_->setProductId(otherProduct->productId());
    product_->setImageUrl(otherProduct->imageUrl());
    product_->setPrice(otherProduct->price());

}

DetailsEntity &DetailsEntity::operator=(const DetailsEntity &other)
{
    if (this == &other)
        return *this;

    quantity_ = other.quantity_;
    if (other.product_)
    {
        const auto otherProduct{other.product()};
        product_ = new ProductEntity(this);
        product_->setName(otherProduct->name());
        product_->setDescription(otherProduct->description());
        product_->setProductId(otherProduct->productId());
        product_->setImageUrl(otherProduct->imageUrl());
        product_->setPrice(otherProduct->price());
    }
    else
    {
        product_ = nullptr;
    }

    return *this;
}

int DetailsEntity::quantity() const
{
    return quantity_;
}

int DetailsEntity::total() const
{
    return product_ ? product_->price() * quantity_ : 0;
}

EntityLayer::ProductEntity *DetailsEntity::product() const
{
    return product_;
}

QByteArray DetailsEntity::toJson() const
{
    QJsonObject object{};
    object["quantity"] = quantity_;
    object["product_id"] = product_->productId();

    return QJsonDocument(object).toJson(QJsonDocument::Compact);
}

void DetailsEntity::setProduct(ProductEntity *product)
{
    if (product_ != product)
    {
        product_ = product;
        emit productChanged();
        emit totalChanged();
    }
}

void DetailsEntity::setQuantity(int quantity)
{
    if (quantity_ != quantity)
    {
        quantity_ = quantity;
        emit quantityChanged();
        emit totalChanged();
    }
}
