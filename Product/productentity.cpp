#include "productentity.h"

namespace EntityLayer {
Product::Product()
    : name_{}
    , description_{}
    , price_{}
    , productId_{}
    , imageUrl_{}
    , maximumStockLevel_{}
    , minimumStockLevel_{}
    , quantityAvailable_{}
    , reorderPoint_{}
    , selectedQty_{}
{
}

Product Product::fromMap(const QVariantMap &map)
{
    Product product{};
    product.setName(map["name"].toString());
    product.setImageUrl(map["image_url"].toString());
    product.setPrice(map["sell_price"].toInt());
    product.setProductId(map["id"].toInt());

    Q_ASSERT(product.name().isEmpty() == false);
    Q_ASSERT(product.price_ > 0);

    return product;
}

Product Product::fromJson(const QByteArray &data)
{
    const auto inventoryObj{QJsonDocument::fromJson(data).object()};
    const auto productObj = inventoryObj.value("product").toObject();

    Product product{};
    product.setProductId(productObj.value("id").toInt());
    product.setName(productObj.value("name").toString());
    product.setDescription(productObj.value("description").toString());
    product.setImageUrl(productObj.value("image_url").toString());
    product.setPrice(productObj.value("sell_price").toInt());
    product.setBuyPrice(productObj.value("buy_price").toInt());
    product.setMaximumStockLevel(inventoryObj.value("maximum_stock_level").toInt());
    product.setMinimumStockLevel(inventoryObj.value("minimum_stock_level").toInt());
    product.setReorderPoint(inventoryObj.value("reorder_point").toInt());
    product.setQuantityAvailable(inventoryObj.value("quantity_available").toInt());

    Q_ASSERT(product.price() > 0);
    return product;
}

bool Product::operator==(const Product &rhs) const
{
    return productId_ == rhs.productId();
}

bool Product::operator!=(const Product &rhs) const
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
    }
}

void Product::setDescription(const QString &description)
{
    if (description_ != description)
    {
        description_ = description;
    }
}

void Product::setImageUrl(const QString &url)
{
    if (imageUrl_ != url)
    {
        imageUrl_ = url;
    }
}

void Product::setPrice(int price)
{
    price_ = price;
}

void Product::setProductId(int productId)
{
    if (productId_ != productId)
    {
        productId_ = productId;
    }
}

int Product::selectedQty() const
{
    return selectedQty_;
}

void Product::setSelectedQty(int newSelectedQty)
{
    selectedQty_ = newSelectedQty;
}

bool EntityLayer::Product::valid() const
{
    return productId_ > 0 && price_ > 0;
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
}
}

QDebug operator<<(QDebug dbg, const EntityLayer::Product &p) {
    dbg.nospace() << QStringLiteral("Product(name=%1, sellPrice=%2, productId=%3)")
                         .arg(p.name(),
                              QString::number(p.price()),
                              QString::number(p.productId()));
    return dbg;
}
