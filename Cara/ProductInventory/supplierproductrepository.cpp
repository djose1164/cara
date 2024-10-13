#include "supplierproductrepository.h"
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
    , supplierId_{}
{
    setPath("products"_L1);
}

void SupplierProductRepository::add(const EntityLayer::Product &product)
{
}

void SupplierProductRepository::update()
{
    const QString identifier{QString::number(supplierId_)};
    if (supplierId_ < 1)
    {
        const auto errStr{"Invalid supplierId: %1"_L1.arg(identifier)};
        qWarning() << "SupplierProductRepository::fetch: " << errStr;
        emit errorOccurred(Error::FailedToFetch, errStr);
        return;
    }

    const QUrlQuery query{ {"supplier_id"_L1, identifier} };
    manager_->get(
        api_->createRequest(path(), query),
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

void SupplierProductRepository::remove(SupplierId supplierId)
{
}

SupplierProductRepository::SupplierId SupplierProductRepository::supplierId() const
{
    return supplierId_;
}

void SupplierProductRepository::setSupplierId(SupplierId newSupplierId)
{
    if (supplierId_ == newSupplierId)
        return;
    supplierId_ = newSupplierId;
    emit supplierIdChanged();
}

} // namespace ProductInventory
