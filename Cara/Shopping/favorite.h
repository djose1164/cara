#ifndef FAVORITE_H
#define FAVORITE_H

#include <QQmlEngine>

namespace Shopping
{
class Favorite
{
    Q_GADGET
    QML_VALUE_TYPE(favorite)
    QML_STRUCTURED_VALUE
    Q_PROPERTY(int favoriteId READ favoriteId WRITE setFavoriteId FINAL)
    Q_PROPERTY(int productId READ productId WRITE setProductId FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId FINAL)
    Q_PROPERTY(bool valid READ valid FINAL)
public:
    using ProductId = int;
    using UserId = int;
    using FavoriteId = int;

    Favorite();

    ProductId productId() const;
    void setProductId(ProductId newProductId);

    UserId userId() const;
    void setUserId(UserId newUserId);

    bool valid() const;

    Q_INVOKABLE QByteArray toJson() const;

    FavoriteId favoriteId() const;
    void setFavoriteId(FavoriteId newFavoriteId);

private:
    FavoriteId favoriteId_;
    ProductId productId_;
    UserId userId_;
};
}

#endif // FAVORITE_H
