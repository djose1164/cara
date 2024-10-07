#ifndef PRODUCTENTITY_H
#define PRODUCTENTITY_H

#include <QObject>
#include <QtQml>

namespace EntityLayer
{
class Product
{
    Q_GADGET
    QML_VALUE_TYPE(product)
    QML_STRUCTURED_VALUE
    Q_PROPERTY(QString name READ name WRITE setName FINAL)
    Q_PROPERTY(QString description MEMBER description_ FINAL)
    Q_PROPERTY(double price READ price WRITE setPrice FINAL)
    Q_PROPERTY(int productId READ productId WRITE setProductId FINAL)
    Q_PROPERTY(int maximumStockLevel READ maximumStockLevel WRITE setMaximumStockLevel FINAL)
    Q_PROPERTY(int minimumStockLevel READ minimumStockLevel WRITE setMinimumStockLevel FINAL)
    Q_PROPERTY(int quantityAvailable READ quantityAvailable WRITE setQuantityAvailable FINAL)
    Q_PROPERTY(int reorderPoint READ reorderPoint WRITE setReorderPoint FINAL)
    Q_PROPERTY(int selectedQty READ selectedQty WRITE setSelectedQty FINAL)
    Q_PROPERTY(QJsonArray images READ images WRITE setImages FINAL)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl FINAL)
    Q_PROPERTY(bool hasImages READ hasImages FINAL)
    Q_PROPERTY(bool postable READ isPostable FINAL)
    Q_PROPERTY(int categoryId READ categoryId WRITE setCategoryId FINAL)
    Q_PROPERTY(QJsonObject supplierCatalog READ supplierCatalog WRITE setSupplierCatalog FINAL)
    Q_PROPERTY(QJsonArray buyPriceHistory READ buyPriceHistory WRITE setBuyPriceHistory FINAL)
    Q_PROPERTY(QString warehouseName READ warehouseName WRITE setWarehouseName FINAL)
public:
    Product();
    Product(int newProductId, double newPrice, int newSelectedQty);

    /**
     * @brief fromMap Construct a product from the map passed.
     * @param map Must contain the property name as key.
     * @return A invalid product if map is invalid, otherwise the properties
     *         initialize to equivalie key-value found in the map.
     */
    static Product fromMap(const QVariantMap &map);

    static Product fromJson(const QByteArray &data);

    static Product *fromJson(const QJsonObject &data);

    static QList<Product> fromJsonArray(const QJsonArray &array);

    bool operator==(const Product &rhs) const;
    bool operator!=(const Product &rhs) const;

    QString name() const;
    QString description() const;

    void setupPricing(const QJsonObject &product);
    void setPrice(double newPrice);
    double price() const;

    int productId() const;

    Q_INVOKABLE QByteArray toJson() const;

    int maximumStockLevel() const;
    void setMaximumStockLevel(int newMaximumStockLevel);

    int minimumStockLevel() const;
    void setMinimumStockLevel(int newMinimumStockLevel);

    int quantityAvailable() const;
    void setQuantityAvailable(int newQuantityAvailable);

    int reorderPoint() const;
    void setReorderPoint(int newReorderPoint);

    int selectedQty() const;
    void setSelectedQty(int newSelectedQty);

    bool valid() const;

    QJsonArray &images();
    const QJsonArray &images() const;
    void setImages(const QJsonArray &newImages);
    void setImages(QJsonArray &&newImages);

    Q_INVOKABLE void appendImage(const QUrl &filename);
    Q_INVOKABLE void clearImages();

    QString imageUrl() const;
    void setImageUrl(const QString &newCoverImage);

    bool hasImages() const;

    int categoryId() const;
    void setCategoryId(int newCategoryId);

    void setName(const QString &name);
    void setDescription(const QString &description);

    void setProductId(int productId);

    double buyPrice() const;
    void setBuyPrice(double newBuyPrice);

    QJsonObject supplierCatalog() const;
    void setSupplierCatalog(const QJsonObject &newSupplierCatalog);

    static QString makeUrlToPicture(QAnyStringView filename);

    QJsonArray buyPriceHistory() const;
    void setBuyPriceHistory(const QJsonArray &newBuyPriceHistory);

    bool isPostable() const;

    QString warehouseName() const;
    void setWarehouseName(const QString &newWarehouseName);

private:
    void setupPictures(const QJsonArray &pics);
    void setPriceHistory(const QJsonArray &history);

private:
    QString name_;
    QString description_;
    double price_;
    double buyPrice_;
    int productId_;

    // TODO: Change to QList<>
    QJsonArray images_;
    QString imageUrl_;
    QString warehouseName_;
    int maximumStockLevel_;
    int minimumStockLevel_;
    int quantityAvailable_;
    int reorderPoint_;
    int selectedQty_;
    bool hasImages_;
    int categoryId_;
    // TODO: Turn into a class
    QJsonObject supplierCatalog_;
    QJsonArray buyPriceHistory_;
};
}

QDebug operator<<(QDebug dbg, const EntityLayer::Product &p);

#endif // PRODUCTENTITY_H
