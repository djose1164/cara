#include "catalogproductrepository.h"

using namespace Qt::StringLiterals;

namespace
{
// constexpr auto resourceID{"/%1"_L1};
// constexpr auto pictureEndpoint{"/picture"_L1};
// constexpr auto imageUrlField{"imageUrl"_L1};
constexpr auto productsField{"products"_L1};
}

namespace ProductInventory
{

CatalogProductRepository::CatalogProductRepository(QObject *parent)
    : IProductRepository{parent}
{
    setPath("products/"_L1);
}

void CatalogProductRepository::add(const EntityLayer::Product &product)
{
}

void CatalogProductRepository::update()
{
    const auto request{api_->createRequest(path())};

    const auto reply = manager_->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        auto &&data{reply->readAll()};

        if (const auto json{QJsonDocument::fromJson(data)};
            json.isObject() && json[productsField].isArray())
        {
            const auto productArray = json[productsField].toArray();
            QList<EntityLayer::Product> productList{productArray.size()};

            std::ranges::transform(productArray.begin(),
                                   productArray.end(),
                                   productList.begin(),
                                   [](const auto &jsonValue) -> EntityLayer::Product
                                   {
                                        return EntityLayer::Product::fromJson(
                                            QJsonDocument{jsonValue.toObject()}
                                                .toJson()
                                        );
                                   });
            emit fetched(productList);

        }
    });
}

void CatalogProductRepository::remove(ProductId productId)
{
}

} // namespace ProductInventory
