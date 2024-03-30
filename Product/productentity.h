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
    Q_PROPERTY(int price READ price WRITE setPrice FINAL)
    Q_PROPERTY(int productId READ productId WRITE setProductId FINAL)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl FINAL)
    Q_PROPERTY(int buyPrice READ buyPrice WRITE setBuyPrice FINAL)
    Q_PROPERTY(int maximumStockLevel READ maximumStockLevel WRITE setMaximumStockLevel FINAL)
    Q_PROPERTY(int minimumStockLevel READ minimumStockLevel WRITE setMinimumStockLevel FINAL)
    Q_PROPERTY(int quantityAvailable READ quantityAvailable WRITE setQuantityAvailable FINAL)
    Q_PROPERTY(int reorderPoint READ reorderPoint WRITE setReorderPoint FINAL)
    Q_PROPERTY(int selectedQty READ selectedQty WRITE setSelectedQty FINAL)
public:
    Product();

    static Product fromMap(const QVariantMap &map);
    static Product fromJson(const QByteArray &data);

    bool operator==(const Product &rhs) const;
    bool operator!=(const Product &rhs) const;

    QString name() const;
    QString description() const;
    QString imageUrl() const;

    void setPrice(int price);
    int price() const;
    QBindable<int> bindablePrice() const;

    int productId() const;

    Q_INVOKABLE QByteArray toJson() const;

    int buyPrice() const;
    void setBuyPrice(int newBuyPrice);

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

public slots:
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setImageUrl(const QString &url);

    void setProductId(int productId);

private:
    QString name_;
    QString description_;
    int price_;
    int buyPrice_;
    int productId_;

    QString imageUrl_;
    int maximumStockLevel_;
    int minimumStockLevel_;
    int quantityAvailable_;
    int reorderPoint_;
    int selectedQty_;
};
}

QDebug operator<<(QDebug dbg, const EntityLayer::Product &p);

#endif // PRODUCTENTITY_H
