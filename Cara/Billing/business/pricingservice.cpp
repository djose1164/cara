#include "pricingservice.h"
#include "abstractproductmodel.h"

namespace Billing
{

PricingService::PricingService(ProductInventory::AbstractProductModel *newProductModel,
                               QObject *parent)
    : QObject{parent}
    , productModel_{newProductModel}
{
}

double PricingService::sellPriceForProduct(ProductId productId) const
{
    return productModel_->getProductById(productId).price();
}

double PricingService::buyPriceForProduct(ProductId productId) const
{
    return productModel_->getProductById(productId).buyPrice();
}

} // namespace Billing
