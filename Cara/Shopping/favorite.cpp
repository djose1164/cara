#include "favorite.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace Shopping
{
Favorite::Favorite()
    : favoriteId_{}
    , productId_{}
    , userId_{}
{}

Favorite::ProductId Favorite::productId() const
{
    return productId_;
}

void Favorite::setProductId(ProductId newProductId)
{
    productId_ = newProductId;
}

Favorite::UserId Favorite::userId() const
{
    return userId_;
}

void Favorite::setUserId(UserId newUserId)
{
    userId_ = newUserId;
}

bool Favorite::valid() const
{
    return productId_ > 0 && userId_ > 0;
}

QByteArray Favorite::toJson() const
{
    QJsonObject obj{};
    obj["product_id"] = productId_;
    obj["user_id"] = userId_;

    return QJsonDocument{obj}.toJson();
}

Favorite::FavoriteId Favorite::favoriteId() const
{
    return favoriteId_;
}

void Favorite::setFavoriteId(FavoriteId newFavoriteId)
{
    favoriteId_ = newFavoriteId;
}
}
