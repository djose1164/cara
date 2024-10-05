#include "supplierproductrepository.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceID{"/%1"_L1};
constexpr auto pictureEndpoint{"/picture"_L1};
constexpr auto productsField{"products"_L1};
constexpr auto imageUrlField{"imageUrl"_L1};
}

namespace ProductInventory
{

SupplierProductRepository::SupplierProductRepository(QObject *parent)
    : IProductRepository{parent}
    , productId_{}
{
    setPath("products"_L1);
}

void SupplierProductRepository::add(const EntityLayer::Product &product)
{
}

void SupplierProductRepository::update()
{
    const QString identifier{QString::number(productId_)};
    if (productId_ < 1)
    {
        const auto errStr{"Invalid productId: %1"_L1.arg(identifier)};
        qWarning() << "SupplierProductRepository::fetch: " << errStr;
        emit errorOccurred(Error::FailedToFetch, errStr);
        return;
    }

    manager_->get(
        api_->createRequest(path() + resourceID.arg(identifier)),
        this,
        [this](QRestReply &reply)
        {
            if (!reply.isSuccess())
            {
                const QString errString{(*reply.readJson())["error"].toString()};
                emit errorOccurred(Error::FailedToFetch, errString);
                return;
            }

            if (const auto json{reply.readJson()};
                json and json->isObject() and json->object().contains(productsField))
            {
                if (not (*json)[productsField].isArray())
                {
                    emit errorOccurred(Error::FailedToFetch, "%1 is not an array."_L1.arg(productsField));
                    return;
                }

                const auto productsArr = (*json)[productsField].toArray();
                const auto productList{EntityLayer::Product::fromJsonArray(productsArr)};

                emit fetched(productList);
            }
        }
    );
}

void SupplierProductRepository::remove(ProductId productId)
{
}

IProductRepository::ProductId SupplierProductRepository::productId() const
{
    return productId_;
}

void SupplierProductRepository::setProductId(ProductId newProductId)
{
    if (productId_ == newProductId)
        return;
    productId_ = newProductId;
    emit productIdChanged();
}

} // namespace ProductInventory
