#include "productentity.h"
#include "restconfig.h"

using namespace Qt::StringLiterals;

namespace
{
const auto imageUrlField{"image_url"_L1};
const auto imageUrlProperty{"imageUrl"_L1};
const auto imagesField{"images"_L1};
const auto nameField{"name"_L1};
const auto sellPriceField{"sell_price"_L1};
const auto buyPriceField{"buy_price"_L1};
const auto idField{"id"_L1};
const auto productField{"product"_L1};
const auto descriptionField{"description"_L1};
constexpr auto maximumStockLevelField{"maximum_stock_level"_L1};
constexpr auto minimumStockLevelField{"minimum_stock_level"_L1};
constexpr auto reorderPointField{"reorder_point"_L1};
constexpr auto quantityAvailableField{"quantity_available"_L1};
#ifdef QT_DEBUG
constexpr auto picIdentificator{LOCAL_API_URL "/uploads/%1"_L1};
#else
constexpr auto picIdentificator{REMOTE_API_URL "/uploads/%1"_L1};
#endif
}

namespace EntityLayer {
Product::Product()
    : name_{}
    , description_{}
    , price_{}
    , buyPrice_{}
    , productId_{}
    , images_{}
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
    product.setName(map[nameField].toString());
    product.setPrice(map[sellPriceField].toInt());
    product.setProductId(map[idField].toInt());

    product.setupPictures(map[imagesField].toJsonArray());

    Q_ASSERT(product.name().isEmpty() == false);
    Q_ASSERT(product.price_ > 0);

    return product;
}

Product Product::fromJson(const QByteArray &data)
{
    const auto inventoryObj{QJsonDocument::fromJson(data).object()};
    const auto productObj = inventoryObj.value(productField).toObject();

    Product product{};
    product.setProductId(productObj.value(idField).toInt());
    product.setName(productObj.value(nameField).toString());
    product.setDescription(productObj.value(descriptionField).toString());
    product.setupPictures(productObj[imagesField].toArray());

    product.setPrice(productObj.value(sellPriceField).toInt());
    product.setBuyPrice(productObj.value(buyPriceField).toInt());

    product.setMaximumStockLevel(inventoryObj.value(maximumStockLevelField).toInt());
    product.setMinimumStockLevel(inventoryObj.value(minimumStockLevelField).toInt());
    product.setReorderPoint(inventoryObj.value(reorderPointField).toInt());
    product.setQuantityAvailable(inventoryObj.value(quantityAvailableField).toInt());

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
    obj["product_id"_L1] = productId_;
    obj[nameField] = name_;
    obj[descriptionField] = description_;
    obj["price"_L1] = price();

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

void Product::setupPictures(const QJsonArray &pics)
{
    QJsonArray imageList;
    for (const auto &pic : pics)
    {
        const auto &&picObj{pic.toObject()};
        const QString imgUrl{picObj.value(imageUrlField).toString()};
        imageList.append(QJsonObject{ {imageUrlProperty, picIdentificator.arg(imgUrl)} });
    }
    setImageUrl(imageList.first().toObject()[imageUrlProperty].toString());
    setImages(std::move(imageList));
}

QString Product::imageUrl() const
{
    return imageUrl_;
}

void Product::setImageUrl(const QString &newCoverImage)
{
    if (imageUrl_ == newCoverImage)
        return;
    imageUrl_ = newCoverImage;
}

QJsonArray Product::images() const
{
    return images_;
}

void Product::setImages(const QJsonArray &newImages)
{
    if (images_ == newImages)
        return;
    images_ = newImages;
}

void Product::setImages(QJsonArray &&newImages)
{
    images_ = std::move(newImages);
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
