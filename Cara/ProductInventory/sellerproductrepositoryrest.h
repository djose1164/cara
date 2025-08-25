#ifndef SELLERPRODUCTREPOSITORYREST_H
#define SELLERPRODUCTREPOSITORYREST_H

#include "iproductrepository.h"
#include <QQmlEngine>

namespace ProductInventory
{

class SellerProductRepositoryREST : public IProductRepository
{
    Q_OBJECT
    Q_PROPERTY(SellerId sellerId READ sellerId WRITE setSellerId NOTIFY sellerIdChanged FINAL)
    QML_ELEMENT
public:
    using SellerId = int;

    explicit SellerProductRepositoryREST(QObject *parent = nullptr);
    SellerProductRepositoryREST(SellerId sellerId, QObject *parent = nullptr);

    SellerId sellerId() const;
    void setSellerId(SellerId newSellerId);

public slots:
    // IProductRepository interface
    void add(const EntityLayer::Product &product) override;
    void remove(ProductId productId) override;

signals:
    void sellerIdChanged();

private:
    void update() override;

    SellerId sellerId_;
};

} // namespace ProductInventory

#endif // SELLERPRODUCTREPOSITORYREST_H
