#include "productentity.h"
#include "restconfig.h"
#include "Network/apienvironment.h"

using namespace Qt::StringLiterals;

namespace
{

consteval QLatin1StringView getPicIdentificator()
{
    switch (Api::getApiEnvironment())
    {
        using enum Api::ApiEnvironment;
    case LocalTesting:
        return LOCAL_API_URL "/uploads/%1"_L1;
    case LocalNetworkTesting:
        return LOCAL_NETWORK_API_URL "/uploads/%1"_L1;
    case RemoteTesting:
        return REMOTE_TESTING_API_URL "/uploads/%1"_L1;
    case Production:
        return REMOTE_API_URL "/uploads/%1"_L1;
    default:
        throw;
    }
}

constexpr auto imageUrlField{"image_url"_L1};
constexpr auto imageUrlProperty{"imageUrl"_L1};
constexpr auto imagesField{"images"_L1};
constexpr auto nameField{"name"_L1};
constexpr auto priceField{"sell_price"_L1};
constexpr auto supplierPriceHistoryField{"supplier_price_history"_L1};
constexpr auto priceHistoryField{"price_history"_L1};
constexpr auto idField{"id"_L1};
constexpr auto productField{"product"_L1};
constexpr auto descriptionField{"description"_L1};
constexpr auto maximumStockLevelField{"maximum_stock_level"_L1};
constexpr auto minimumStockLevelField{"minimum_stock_level"_L1};
constexpr auto reorderPointField{"reorder_point"_L1};
constexpr auto quantityAvailableField{"quantity_available"_L1};
constexpr auto categoryIdField{"category_id"_L1};
constexpr auto picIdentificator{getPicIdentificator()};
}

namespace EntityLayer
{

Product::Product()
    : Product{0, 0, 0}
{
}

Product::Product(int newProductId, double newPrice, int newSelectedQty)
    : name_{}
    , description_{}
    , price_{newPrice}
    , buyPrice_{}
    , productId_{newProductId}
    , images_{}
    , imageUrl_{}
    , maximumStockLevel_{}
    , minimumStockLevel_{}
    , quantityAvailable_{}
    , reorderPoint_{}
    , selectedQty_{newSelectedQty}
    , categoryId_{}
{
}

Product Product::fromMap(const QVariantMap &map)
{
    Product product{};
    product.setName(map[nameField].toString());
    product.setPrice(map[priceField].toDouble());
    product.setProductId(map[idField].toInt());

    product.setupPictures(map[imagesField].toJsonArray());

    Q_ASSERT(product.name().isEmpty() == false);
    Q_ASSERT(product.price_ > 0);

    return product;
}

Product Product::fromJson(const QByteArray &data)
{
    const auto inventoryObj{QJsonDocument::fromJson(data).object()};
    QJsonObject productObj;
    bool isFromInventory{};

    if (const auto json{QJsonDocument::fromJson(data)};
        json.isObject() and json.object().contains(productField))
    {
        productObj = std::move(inventoryObj.value(productField).toObject());
        isFromInventory = true;
    }
    else
    {
        productObj = std::move(json.object());
    }

    Product product{};
    product.setProductId(productObj.value(idField).toInt());
    product.setName(productObj.value(nameField).toString());
    product.setDescription(productObj.value(descriptionField).toString());
    product.setCategoryId(productObj[categoryIdField].toInt());

    product.setupPictures(productObj[imagesField].toArray());
    product.setupPricing(productObj);

    Q_ASSERT(productObj[categoryIdField].toInt());
    Q_ASSERT(product.price() > 0);
    Q_ASSERT(product.categoryId() > 0);

    if (not isFromInventory)
        return product;

    product.setMaximumStockLevel(inventoryObj.value(maximumStockLevelField).toInt());
    product.setMinimumStockLevel(inventoryObj.value(minimumStockLevelField).toInt());
    product.setReorderPoint(inventoryObj.value(reorderPointField).toInt());
    product.setQuantityAvailable(inventoryObj.value(quantityAvailableField).toInt());

    return product;
}

Product *Product::fromJson(const QJsonObject &data)
{
    QJsonObject productObj = std::move(data);

    Q_ASSERT(productObj.value(idField).toInt());

    const auto product{new Product};
    product->setProductId(productObj.value(idField).toInt());
    product->setName(productObj.value(nameField).toString());
    product->setDescription(productObj.value(descriptionField).toString());
    product->setCategoryId(productObj[categoryIdField].toInt());

    product->setupPictures(productObj[imagesField].toArray());
    product->setupPricing(productObj);

    return product;
}

bool Product::operator==(const Product &rhs) const
{
    return productId_ == rhs.productId() && images_ == rhs.images_;
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

double Product::price() const
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
    //obj["product_id"_L1] = productId_;
    obj[nameField] = name_;
    obj[descriptionField] = description_;
    obj[priceHistoryField] = QJsonArray{QJsonObject{{"price"_L1, price_}}};
    obj[imagesField] = images_;
    obj[categoryIdField] = categoryId_;

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

void Product::setupPricing(const QJsonObject &product)
{
    if (product.contains(priceHistoryField))
    {
        setPriceHistory(product[priceHistoryField].toArray());
    }
    else
    {
        qWarning() << "Product::setupPricing: %1 not present in QJsonObject."_L1.arg(
            priceHistoryField);
    }
}

void Product::setPrice(double newPrice)
{
    price_ = newPrice;
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
        imageList.append(QJsonObject{{imageUrlProperty, picIdentificator.arg(imgUrl)},
                                     {idField, picObj[idField].toInt()}});
        Q_ASSERT(imageList.last()[idField].toInt() > 0);
    }
    setImageUrl(imageList.first().toObject()[imageUrlProperty].toString());
    setImages(std::move(imageList));
}

void Product::setPriceHistory(const QJsonArray &history)
{
    if (history.isEmpty())
    {
        qWarning() << "Product::setupPrice: priceHistory is empty.";
        return;
    }
    std::ranges::for_each(history,
                          [this](const QJsonValue &jsonValue)
                          {
                              if (const auto entity{jsonValue.toObject()};
                                  entity["thru_date"_L1].isNull())
                              {
                                  const double newPrice{entity["price"_L1].toString().toDouble()};
                                  setPrice(newPrice);
                              }
                          });
}

double Product::buyPrice() const
{
    return buyPrice_;
}

void Product::setBuyPrice(double newBuyPrice)
{
    buyPrice_ = newBuyPrice;
}

QString Product::makeUrlToPicture(QAnyStringView filename)
{
    return getPicIdentificator().arg(filename.toString());
}

int Product::categoryId() const
{
    return categoryId_;
}

void Product::setCategoryId(int newCategoryId)
{
    categoryId_ = newCategoryId;
}

bool Product::hasImages() const
{
    return !images_.isEmpty();
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

QJsonArray &Product::images()
{
    return images_;
}

const QJsonArray &Product::images() const
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

void Product::appendImage(const QUrl &filename)
{
    images_.append(QJsonObject{{imageUrlProperty, filename.toString()}});
}

void Product::clearImages()
{
    while (images_.count())
        images_.pop_back();
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

}

QDebug operator<<(QDebug dbg, const EntityLayer::Product &p)
{
    dbg.nospace() << QStringLiteral("Product(name=%1, sellPrice=%2, productId=%3)")
                         .arg(p.name(),
                              QString::number(p.price()),
                              QString::number(p.productId()));
    return dbg;
}
