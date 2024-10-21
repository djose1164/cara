#ifndef SALE_H
#define SALE_H

#include <QQmlEngine>

namespace Billing
{

class SaleItem
{
    Q_GADGET
public:
    SaleItem();

    int productId() const;
    void setProductId(int newProductId);

    int quantity() const;
    void setQuantity(int newQuantity);

    QJsonObject toJson() const;

    bool isValid() const;

    static SaleItem fromJson(const QJsonObject &json);

private:
    int productId_;
    int quantity_;
};


class Sale
{
    Q_GADGET
    Q_PROPERTY(int saleId READ saleId WRITE setSaleId FINAL)
    Q_PROPERTY(int orderId READ orderId WRITE setOrderId FINAL)
    Q_PROPERTY(double totalAmount READ totalAmount WRITE setTotalAmount FINAL)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt FINAL)
    Q_PROPERTY(QList<SaleItem> saleItems READ saleItems WRITE setSaleItems NOTIFY saleItemsChanged FINAL)
public:  
    Sale();

    int saleId() const;
    void setSaleId(int newSaleId);

    int orderId() const;
    void setOrderId(int newOrderId);

    double totalAmount() const;
    void setTotalAmount(double newTotalAmount);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &newCreatedAt);

    QJsonObject toJson() const;

    static Sale fromJson(const QJsonObject &json);

    int customerId() const;
    void setCustomerId(int newCustomerId);

    QList<SaleItem> saleItems() const;
    void setSaleItems(const QList<SaleItem> &newSaleItems);

    bool hasItems() const;

private:
    int saleId_;
    int orderId_;
    int customerId_;
    double totalAmount_;
    QList<SaleItem> saleItems_;
    QDateTime createdAt_;
};

} // namespace Billing

#endif // SALE_H
