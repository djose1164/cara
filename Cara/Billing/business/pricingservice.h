#ifndef PRICINGSERVICE_H
#define PRICINGSERVICE_H

#include <QObject>

namespace ProductInventory { class AbstractProductModel; }
namespace Billing
{

class PricingService : public QObject
{
    Q_OBJECT
public:
    using ProductId = int;

    PricingService(ProductInventory::AbstractProductModel *newProductModel,
                   QObject *parent = nullptr);

    double sellPriceForProduct(ProductId productId) const;

    double buyPriceForProduct(ProductId productId) const;
signals:

private:
    ProductInventory::AbstractProductModel *productModel_;
};

} // namespace Billing

#endif // PRICINGSERVICE_H
