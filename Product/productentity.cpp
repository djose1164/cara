#include "productentity.h"

namespace EntityLayer {
Product::Product(QObject *parent)
    : QObject{parent}
    , name_{}
    , description_{}
    , price_{}
    , productId_{}
    , imageUrl_{}
    , selectedQty_{}
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

QPointer<Product> Product::fromJson(const QByteArray &data)
{
    const auto inventoryObj{QJsonDocument::fromJson(data).object()};
    const auto productObj = inventoryObj.value("product").toObject();

    const auto product{new Product};
    product->setProductId(productObj.value("id").toInt());
    product->setName(productObj.value("name").toString());
    product->setDescription(productObj.value("description").toString());
    product->setImageUrl(productObj.value("image_url").toString());
    product->setPrice(productObj.value("sell_price").toInt());
    product->setBuyPrice(productObj.value("buy_price").toInt());
    product->setMaximumStockLevel(inventoryObj.value("maximum_stock_level").toInt());
    product->setMinimumStockLevel(inventoryObj.value("minimum_stock_level").toInt());
    product->setReorderPoint(inventoryObj.value("reorder_point").toInt());
    product->setQuantityAvailable(inventoryObj.value("quantity_available").toInt());
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

int Product::selectedQty() const
{
    return selectedQty_;
}

void Product::setSelectedQty(int newSelectedQty)
{
    if (selectedQty_ == newSelectedQty)
        return;
    selectedQty_ = newSelectedQty;
    emit selectedQtyChanged();
}

int Product::reorderPoint() const
{
    return reorderPoint_;
}

void Product::setReorderPoint(int newReorderPoint)
{
    if (reorderPoint_ == newReorderPoint)
        return;
    reorderPoint_ = newReorderPoint;
    emit reorderPointChanged();
}

int Product::quantityAvailable() const
{
    return quantityAvailable_;
}

void Product::setQuantityAvailable(int newQuantityAvailable)
{
    if (quantityAvailable_ == newQuantityAvailable)
        return;
    quantityAvailable_ = newQuantityAvailable;
    emit quantityAvailableChanged();
}

int Product::minimumStockLevel() const
{
    return minimumStockLevel_;
}

void Product::setMinimumStockLevel(int newMinimumStockLevel)
{
    if (minimumStockLevel_ == newMinimumStockLevel)
        return;
    minimumStockLevel_ = newMinimumStockLevel;
    emit minimumStockLevelChanged();
}

int Product::maximumStockLevel() const
{
    return maximumStockLevel_;
}

void Product::setMaximumStockLevel(int newMaximumStockLevel)
{
    if (maximumStockLevel_ == newMaximumStockLevel)
        return;
    maximumStockLevel_ = newMaximumStockLevel;
    emit maximumStockLevelChanged();
}

int Product::buyPrice() const
{
    return buyPrice_;
}

void Product::setBuyPrice(int newBuyPrice)
{
    if (buyPrice_ == newBuyPrice)
        return;
    buyPrice_ = newBuyPrice;
    emit buyPriceChanged();
}
}
