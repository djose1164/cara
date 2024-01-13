#ifndef PRODUCTENTITY_H
#define PRODUCTENTITY_H

#include "jsonexportable.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer
{
class Product : public QObject, public JsonExportable
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description MEMBER description_ NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(int price READ price WRITE setPrice NOTIFY priceChanged FINAL)
    Q_PROPERTY(int productId READ productId WRITE setProductId NOTIFY productIdChanged FINAL)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged FINAL)
    Q_PROPERTY(int buyPrice READ buyPrice WRITE setBuyPrice NOTIFY buyPriceChanged FINAL)
    Q_PROPERTY(int maximumStockLevel READ maximumStockLevel WRITE setMaximumStockLevel NOTIFY maximumStockLevelChanged FINAL)
    Q_PROPERTY(int minimumStockLevel READ minimumStockLevel WRITE setMinimumStockLevel NOTIFY minimumStockLevelChanged FINAL)
    Q_PROPERTY(int quantityAvailable READ quantityAvailable WRITE setQuantityAvailable NOTIFY quantityAvailableChanged FINAL)
    Q_PROPERTY(int reorderPoint READ reorderPoint WRITE setReorderPoint NOTIFY reorderPointChanged FINAL)
    Q_PROPERTY(int selectedQty READ selectedQty WRITE setSelectedQty NOTIFY selectedQtyChanged FINAL)
public:
    explicit Product(QObject *parent = nullptr);

    static Product *fromMap(const QVariantMap &map);
    static QPointer<Product> fromJson(const QByteArray &data);

    bool operator==(Product *const rhs);
    bool operator!=(Product *const rhs);

    QString name() const;
    QString description() const;
    QString imageUrl() const;

    void setPrice(int price);
    int price() const;
    QBindable<int> bindablePrice() const;

    int productId() const;

    Q_INVOKABLE QByteArray toJson() const override;

    int buyPrice() const;
    void setBuyPrice(int newBuyPrice);
    QBindable<int> bindableBuyPrice() const;

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

signals:
    void nameChanged();
    void descriptionChanged();
    void priceChanged();
    void productIdChanged();
    void imageUrlChanged();

    void maximumStockLevelChanged();

    void minimumStockLevelChanged();

    void quantityAvailableChanged();

    void reorderPointChanged();

    void buyPriceChanged();

    void selectedQtyChanged();

public slots:
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setImageUrl(const QString &url);

    void setProductId(int productId);

private:
    QString name_;
    QString description_;
    QProperty<int> price_;
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

#endif // PRODUCTENTITY_H
